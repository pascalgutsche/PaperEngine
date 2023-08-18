#include "Engine.h"
#include "ScriptGlue.h"

#include <mono/jit/jit.h>

#include <glm/gtx/string_cast.hpp>

#include "ScriptEngine.h"
#include "generic/Scene.h"

#include "Components.h"
#include "ScriptAssembly.h"
#include "event/Input.h"
#include "renderer/Font.h"

namespace Paper
{
    static std::unordered_map<MonoType*, std::function<bool(Entity)>> entityHasComponentFns;

#define SCR_ADD_INTRERNAL_CALL(Name) mono_add_internal_call("Paper.InternalCalls::" #Name, Name)

#define MONO_STRING(Value) mono_string_new(mono_domain_get(), Value)

	//Input
    static bool IsKeyPressed(int code)
    {
        return Input::IsKeyPressed((Key)code);
    }

    static bool IsKeyHeld(int code)
    {
        return Input::IsKeyHeld((Key)code);
    }

    static bool IsKeyDown(int code)
    {
        return Input::IsKeyDown((Key)code);
    }

    static bool IsKeyReleased(int code)
    {
        return Input::IsKeyReleased((Key)code);
    }

    static bool IsMouseButtonPressed(int button)
    {
        return Input::IsMouseButtonPressed((MouseButton)button);
    }

    static bool IsMouseButtonHeld(int button)
    {
        return Input::IsMouseButtonHeld((MouseButton)button);
    }

    static bool IsMouseButtonDown(int button)
    {
        return Input::IsMouseButtonDown((MouseButton)button);
    }

    static bool IsMouseButtonReleased(int button)
    {
        return Input::IsMouseButtonReleased((MouseButton)button);
    }

    //Render
    struct TextureData
    {
        MonoString* filepath;
        uint32_t width;
        uint32_t height;
        uint32_t ID;
    };
    static void GetTexture(MonoString* filePath, TextureData* data)
    {
        char* filePath_c = mono_string_to_utf8(filePath);
        std::string filePath_std(filePath_c);
        mono_free(filePath_c);

        Shr<Texture> texture = DataPool::GetAssetTexture(filePath_std, true);
        TextureData localData;
        if (texture)
        {
            localData.filepath = mono_string_new(mono_domain_get(), texture->GetFilePath().string().c_str());
            localData.ID = texture->GetID();
            localData.width = texture->GetWidth();
            localData.height = texture->GetHeight();
        }
        else
        {
            localData.filepath = mono_string_new(mono_domain_get(), "");
            localData.ID = 0;
            localData.width = 0;
            localData.height = 0;
        }
        
        *data = localData;
    }

    static bool Entity_HasComponent(EntityID entityUUID, MonoReflectionType* componentType)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CORE_ASSERT(scene, "");
        Entity entity = scene->GetEntity(entityUUID);
        CORE_ASSERT(entity, "");
        MonoType* monoType = mono_reflection_type_get_type(componentType);

        CORE_ASSERT(entityHasComponentFns.find(monoType) != entityHasComponentFns.end(), "");
    	return entityHasComponentFns.at(monoType)(entity);

    }

    static uint64_t Entity_GetEntityByName(MonoString* name)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CORE_ASSERT(scene, "");
        std::string entityName = ScriptUtils::MonoStringToStdString(name);

        Entity entity = scene->GetEntityByName(entityName);
        if (entity)
			return entity.GetEntityID().toUInt64();
        return 0;
    }

    static MonoObject* Entity_GetScriptInstance(EntityID entityUUID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CORE_ASSERT(scene, "");
        Entity entity = scene->GetEntity(entityUUID);
        CORE_ASSERT(entity, "");

        MonoObject* instance = ScriptEngine::GetEntityScriptInstance(entity.GetEntityID())->GetMonoInstance();

        return instance;

    }

    //Components
    static void TransformComponent_GetPosition(EntityID entityUUID, glm::vec3* outPosition)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        *outPosition = scene->GetEntity(entityUUID).GetComponent<TransformComponent>().position;
    }

    static void TransformComponent_SetPosition(EntityID entityUUID, glm::vec3* inPosition)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        scene->GetEntity(entityUUID).GetComponent<TransformComponent>().position = *inPosition;
    }

    static void TransformComponent_GetRotation(EntityID entityUUID, glm::vec3* outRotation)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        *outRotation = scene->GetEntity(entityUUID).GetComponent<TransformComponent>().rotation;
    }

    static void TransformComponent_SetRotation(EntityID entityUUID, glm::vec3* inRotation)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        scene->GetEntity(entityUUID).GetComponent<TransformComponent>().rotation = *inRotation;
    }

    static void TransformComponent_GetScale(EntityID entityUUID, glm::vec3* outScale)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        *outScale = scene->GetEntity(entityUUID).GetComponent<TransformComponent>().scale;
    }

    static void TransformComponent_SetScale(EntityID entityUUID, glm::vec3* inScale)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        scene->GetEntity(entityUUID).GetComponent<TransformComponent>().scale = *inScale;
    }

    static MonoString* DataComponent_GetName(EntityID entityUUID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& dc = scene->GetEntity(entityUUID).GetComponent<DataComponent>();
        return mono_string_new(mono_domain_get(), dc.name.c_str());
    }

    static void DataComponent_SetName(EntityID entityUUID, MonoString* name)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& dc = scene->GetEntity(entityUUID).GetComponent<DataComponent>();
        char* name_c = mono_string_to_utf8(name);
        dc.name = name_c;
        mono_free(name_c);
    }

    static MonoString* DataComponent_GetTags(EntityID entityUUID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& dc = scene->GetEntity(entityUUID).GetComponent<DataComponent>();
        std::string tagList;
    	for (const std::string tag : dc.tags)
        {
            tagList += tag;
            tagList += ',';
        }
        if (tagList.length() > 1)
            tagList.erase(tagList.begin() + tagList.find_last_of(','));
        return mono_string_new(mono_domain_get(), tagList.c_str());
    }

    static void DataComponent_SetTags(EntityID entityUUID, MonoString* name)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& dc = scene->GetEntity(entityUUID).GetComponent<DataComponent>();
        char* name_c = mono_string_to_utf8(name);
        std::stringstream tagList(name_c);
        mono_free(name_c);
        dc.tags.clear();
        std::string temp;
        while (getline(tagList, temp, ',')) {
            dc.tags.push_back(temp);
        }
    }

    static void SpriteComponent_GetColor(EntityID entityUUID, glm::vec4* outColor)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityUUID).GetComponent<SpriteComponent>();
        *outColor = sc.color;
    }

    static void SpriteComponent_SetColor(EntityID entityUUID, glm::vec4* inColor)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityUUID).GetComponent<SpriteComponent>();
        sc.color = *inColor;
    }

    static void SpriteComponent_GetTexture(EntityID entityUUID, TextureData* outTextureData)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityUUID).GetComponent<SpriteComponent>();
    	TextureData data;
        if (sc.texture)
        {
            data.filepath = MONO_STRING(sc.texture->GetFilePath().string().c_str());
            data.ID = sc.texture->GetID();
            data.width = sc.texture->GetWidth();
            data.height = sc.texture->GetHeight();
        }
        else
        {
            data.filepath = MONO_STRING("");
            data.ID = 0;
            data.width = 0;
            data.height = 0;
        }
        *outTextureData = data;
    }

    static void SpriteComponent_SetTexture(EntityID entityUUID, MonoString* inTextureFilePath)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityUUID).GetComponent<SpriteComponent>();
        sc.texture = DataPool::GetAssetTexture(ScriptUtils::MonoStringToStdString(inTextureFilePath), true);
    }

    static float SpriteComponent_GetTilingFactor(EntityID entityUUID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityUUID).GetComponent<SpriteComponent>();
        return sc.tiling_factor;
    }

    static void SpriteComponent_SetTilingFactor(EntityID entityUUID, float tilingFactor)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityUUID).GetComponent<SpriteComponent>();
        sc.tiling_factor = tilingFactor;
    }

    static void SpriteComponent_GetUV0(EntityID entityUUID, glm::vec2* outUV0)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityUUID).GetComponent<SpriteComponent>();

        *outUV0 = sc.tex_coords[0];
    }

    static void SpriteComponent_SetUV0(EntityID entityUUID, glm::vec2* inUV0)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityUUID).GetComponent<SpriteComponent>();

        sc.tex_coords[0] = *inUV0;
        sc.tex_coords[1].y = (*inUV0).y;
        sc.tex_coords[3].x = (*inUV0).x;
    }

    static void SpriteComponent_GetUV1(EntityID entityUUID, glm::vec2* outUV1)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityUUID).GetComponent<SpriteComponent>();

        *outUV1 = sc.tex_coords[2];
    }

    static void SpriteComponent_SetUV1(EntityID entityUUID, glm::vec2* inUV1)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityUUID).GetComponent<SpriteComponent>();

        sc.tex_coords[1].x = (*inUV1).x;
        sc.tex_coords[2] = *inUV1;
        sc.tex_coords[3].y = (*inUV1).y;
    }

    static int SpriteComponent_GetGeometry(EntityID entityUUID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityUUID).GetComponent<SpriteComponent>();

        return (int)sc.geometry;
    }

    static void SpriteComponent_SetGeometry(EntityID entityUUID, int geometry)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityUUID).GetComponent<SpriteComponent>();

        sc.geometry = (Geometry)geometry;
    }

    static float SpriteComponent_GetThickness(EntityID entityUUID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityUUID).GetComponent<SpriteComponent>();

        return sc.thickness;
    }

    static void SpriteComponent_SetThickness(EntityID entityUUID, float thickness)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityUUID).GetComponent<SpriteComponent>();

        sc.thickness = thickness;
    }

    static float SpriteComponent_GetFade(EntityID entityUUID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityUUID).GetComponent<SpriteComponent>();

        return sc.fade;
    }

    static void SpriteComponent_SetFade(EntityID entityUUID, float fade)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityUUID).GetComponent<SpriteComponent>();

        sc.fade = fade;
    }

    static void LineComponent_GetColor(EntityID entityUUID, glm::vec4* outColor)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& lc = scene->GetEntity(entityUUID).GetComponent<LineComponent>();
        *outColor = lc.color;
    }

    static void LineComponent_SetColor(EntityID entityUUID, glm::vec4* inColor)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& lc = scene->GetEntity(entityUUID).GetComponent<LineComponent>();
        lc.color = *inColor;
    }

    static float LineComponent_GetThickness(EntityID entityUUID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& lc = scene->GetEntity(entityUUID).GetComponent<LineComponent>();

        return lc.thickness;
    }

    static void LineComponent_SetThickness(EntityID entityUUID, float thickness)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& lc = scene->GetEntity(entityUUID).GetComponent<LineComponent>();

        lc.thickness = thickness;
    }

    static void TextComponent_GetColor(EntityID entityUUID, glm::vec4* outColor)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& tc = scene->GetEntity(entityUUID).GetComponent<TextComponent>();
        *outColor = tc.color;
    }

    static void TextComponent_SetColor(EntityID entityUUID, glm::vec4* inColor)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& tc = scene->GetEntity(entityUUID).GetComponent<TextComponent>();
        tc.color = *inColor;
    }

    static MonoString* TextComponent_GetText(EntityID entityUUID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& tc = scene->GetEntity(entityUUID).GetComponent<TextComponent>();
        return MONO_STRING(tc.text.c_str());
    }

    static void TextComponent_SetText(EntityID entityUUID, MonoString* text)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& tc = scene->GetEntity(entityUUID).GetComponent<TextComponent>();
        tc.text = ScriptUtils::MonoStringToStdString(text);
    }

    static MonoString* TextComponent_GetFontPath(EntityID entityUUID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& tc = scene->GetEntity(entityUUID).GetComponent<TextComponent>();
        return MONO_STRING(tc.font->GetFilePath().c_str());
    }

    static void TextComponent_SetFontPath(EntityID entityUUID, MonoString* fontPath)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& tc = scene->GetEntity(entityUUID).GetComponent<TextComponent>();
        tc.font = DataPool::GetFont(ScriptUtils::MonoStringToStdString(fontPath), true);
    }

	void ScriptGlue::RegisterFunctions()
	{
        //Input
        SCR_ADD_INTRERNAL_CALL(IsKeyPressed);
        SCR_ADD_INTRERNAL_CALL(IsKeyHeld);
        SCR_ADD_INTRERNAL_CALL(IsKeyDown);
        SCR_ADD_INTRERNAL_CALL(IsKeyReleased);
        SCR_ADD_INTRERNAL_CALL(IsMouseButtonPressed);
        SCR_ADD_INTRERNAL_CALL(IsMouseButtonHeld);
        SCR_ADD_INTRERNAL_CALL(IsMouseButtonDown);
        SCR_ADD_INTRERNAL_CALL(IsMouseButtonReleased);

        //Render
        SCR_ADD_INTRERNAL_CALL(GetTexture);

        //Entity
        SCR_ADD_INTRERNAL_CALL(Entity_HasComponent);
        SCR_ADD_INTRERNAL_CALL(Entity_GetEntityByName);
        SCR_ADD_INTRERNAL_CALL(Entity_GetScriptInstance);

    	//Components
        SCR_ADD_INTRERNAL_CALL(TransformComponent_GetPosition);
        SCR_ADD_INTRERNAL_CALL(TransformComponent_SetPosition);
        SCR_ADD_INTRERNAL_CALL(TransformComponent_GetRotation);
        SCR_ADD_INTRERNAL_CALL(TransformComponent_SetRotation);
        SCR_ADD_INTRERNAL_CALL(TransformComponent_GetScale);
        SCR_ADD_INTRERNAL_CALL(TransformComponent_SetScale);

        SCR_ADD_INTRERNAL_CALL(DataComponent_GetName);
        SCR_ADD_INTRERNAL_CALL(DataComponent_SetName);
        SCR_ADD_INTRERNAL_CALL(DataComponent_GetTags);
        SCR_ADD_INTRERNAL_CALL(DataComponent_SetTags);

    	SCR_ADD_INTRERNAL_CALL(SpriteComponent_GetColor);
    	SCR_ADD_INTRERNAL_CALL(SpriteComponent_SetColor);
    	SCR_ADD_INTRERNAL_CALL(SpriteComponent_GetTexture);
    	SCR_ADD_INTRERNAL_CALL(SpriteComponent_SetTexture);
    	SCR_ADD_INTRERNAL_CALL(SpriteComponent_GetTilingFactor);
    	SCR_ADD_INTRERNAL_CALL(SpriteComponent_SetTilingFactor);
    	SCR_ADD_INTRERNAL_CALL(SpriteComponent_GetUV0);
    	SCR_ADD_INTRERNAL_CALL(SpriteComponent_SetUV0);
    	SCR_ADD_INTRERNAL_CALL(SpriteComponent_GetUV1);
    	SCR_ADD_INTRERNAL_CALL(SpriteComponent_SetUV1);
    	SCR_ADD_INTRERNAL_CALL(SpriteComponent_GetGeometry);
    	SCR_ADD_INTRERNAL_CALL(SpriteComponent_SetGeometry);
    	SCR_ADD_INTRERNAL_CALL(SpriteComponent_GetThickness);
    	SCR_ADD_INTRERNAL_CALL(SpriteComponent_SetThickness);
    	SCR_ADD_INTRERNAL_CALL(SpriteComponent_GetFade);
    	SCR_ADD_INTRERNAL_CALL(SpriteComponent_SetFade);

    	SCR_ADD_INTRERNAL_CALL(LineComponent_GetColor);
    	SCR_ADD_INTRERNAL_CALL(LineComponent_SetColor);
    	SCR_ADD_INTRERNAL_CALL(LineComponent_GetThickness);
    	SCR_ADD_INTRERNAL_CALL(LineComponent_SetThickness);

    	SCR_ADD_INTRERNAL_CALL(TextComponent_GetColor);
    	SCR_ADD_INTRERNAL_CALL(TextComponent_SetColor);
    	SCR_ADD_INTRERNAL_CALL(TextComponent_GetText);
    	SCR_ADD_INTRERNAL_CALL(TextComponent_SetText);
    	SCR_ADD_INTRERNAL_CALL(TextComponent_GetFontPath);
    	SCR_ADD_INTRERNAL_CALL(TextComponent_SetFontPath);
	}

    template<typename... Component>
    static void RegisterComponent()
    {
        ([]()
        {
            std::string_view typeName = typeid(Component).name();
            size_t index = typeName.find_last_of(':');
            std::string_view structName = typeName.substr(++index);
            std::string csStructName = std::format("Paper.{}", structName);

            MonoType* managedType = mono_reflection_type_from_name(csStructName.data(), ScriptEngine::GetCoreAssembly().GetMonoAssemblyImage());
            if (!managedType)
            {
                LOG_SCR_CRITICAL("Could not find Component '{}'", csStructName);
                return;
            }
            entityHasComponentFns[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
        }(), ...);
        
    }

    template<typename... Component>
    static void RegisterComponent(ComponentGroup<Component...>)
    {
        RegisterComponent<Component...>();
    }

	void ScriptGlue::RegisterComponents()
	{
        entityHasComponentFns.clear();
        RegisterComponent(AllComponents{});
	}
}

