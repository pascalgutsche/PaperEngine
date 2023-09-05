#include "Engine.h"
#include "ScriptGlue.h"

#include "ScriptEngine.h"
#include "ScriptAssembly.h"

#include "scene/Components.h"

#include "scene//Scene.h"
#include "renderer/Font.h"
#include "event/Input.h"

#include <mono/jit/jit.h>

#include <glm/gtx/string_cast.hpp>

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

    static float EntityCamera_GetPerspectiveFOV(PaperID entityID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CORE_ASSERT(scene, "");
        Entity entity = scene->GetEntity(entityID);
        CORE_ASSERT(entity, "");

        auto& cc = entity.GetComponent<CameraComponent>();
        return cc.camera.GetPerspectiveFOV();
    }

    static void EntityCamera_SetPerspectiveFOV(PaperID entityID, float fov)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CORE_ASSERT(scene, "");
        Entity entity = scene->GetEntity(entityID);
        CORE_ASSERT(entity, "");

        auto& cc = entity.GetComponent<CameraComponent>();
        cc.camera.SetPerspectiveFOV(fov);
    }

    static float EntityCamera_GetPerspectiveNearClip(PaperID entityID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CORE_ASSERT(scene, "");
        Entity entity = scene->GetEntity(entityID);
        CORE_ASSERT(entity, "");

        auto& cc = entity.GetComponent<CameraComponent>();
        return cc.camera.GetPerspectiveNearClip();
    }

    static void EntityCamera_SetPerspectiveNearClip(PaperID entityID, float nearClip)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CORE_ASSERT(scene, "");
        Entity entity = scene->GetEntity(entityID);
        CORE_ASSERT(entity, "");

        auto& cc = entity.GetComponent<CameraComponent>();
        cc.camera.SetPerspectiveNearClip(nearClip);
    }

    static float EntityCamera_GetPerspectiveFarClip(PaperID entityID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CORE_ASSERT(scene, "");
        Entity entity = scene->GetEntity(entityID);
        CORE_ASSERT(entity, "");

        auto& cc = entity.GetComponent<CameraComponent>();
        return cc.camera.GetPerspectiveFarClip();
    }

    static void EntityCamera_SetPerspectiveFarClip(PaperID entityID, float farClip)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CORE_ASSERT(scene, "");
        Entity entity = scene->GetEntity(entityID);
        CORE_ASSERT(entity, "");

        auto& cc = entity.GetComponent<CameraComponent>();
        cc.camera.SetPerspectiveFarClip(farClip);
    }

    static float EntityCamera_GetOrthographicSize(PaperID entityID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CORE_ASSERT(scene, "");
        Entity entity = scene->GetEntity(entityID);
        CORE_ASSERT(entity, "");

        auto& cc = entity.GetComponent<CameraComponent>();
        return cc.camera.GetOrthographicSize();
    }

    static void EntityCamera_SetOrthographicSize(PaperID entityID, float size)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CORE_ASSERT(scene, "");
        Entity entity = scene->GetEntity(entityID);
        CORE_ASSERT(entity, "");

        auto& cc = entity.GetComponent<CameraComponent>();
        cc.camera.SetOrthographicSize(size);
    }

    static float EntityCamera_GetOrthographicNearClip(PaperID entityID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CORE_ASSERT(scene, "");
        Entity entity = scene->GetEntity(entityID);
        CORE_ASSERT(entity, "");

        auto& cc = entity.GetComponent<CameraComponent>();
        return cc.camera.GetOrthographicNearClip();
    }

    static void EntityCamera_SetOrthographicNearClip(PaperID entityID, float nearClip)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CORE_ASSERT(scene, "");
        Entity entity = scene->GetEntity(entityID);
        CORE_ASSERT(entity, "");

        auto& cc = entity.GetComponent<CameraComponent>();
        cc.camera.SetOrthographicNearClip(nearClip);
    }

    static float EntityCamera_GetOrthographicFarClip(PaperID entityID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CORE_ASSERT(scene, "");
        Entity entity = scene->GetEntity(entityID);
        CORE_ASSERT(entity, "");

        auto& cc = entity.GetComponent<CameraComponent>();
        return cc.camera.GetOrthographicFarClip();
    }

    static void EntityCamera_SetOrthographicFarClip(PaperID entityID, float farClip)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CORE_ASSERT(scene, "");
        Entity entity = scene->GetEntity(entityID);
        CORE_ASSERT(entity, "");

        auto& cc = entity.GetComponent<CameraComponent>();
        cc.camera.SetOrthographicFarClip(farClip);
    }

    static void EntityCamera_SetViewportSize(PaperID entityID, glm::vec2* size)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CORE_ASSERT(scene, "");
        Entity entity = scene->GetEntity(entityID);
        CORE_ASSERT(entity, "");

        auto& cc = entity.GetComponent<CameraComponent>();
        cc.camera.SetViewportSize(size->x, size->y);
    }

    static bool Entity_HasComponent(PaperID entityID, MonoReflectionType* componentType)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CORE_ASSERT(scene, "");
        Entity entity = scene->GetEntity(entityID);
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
			return entity.GetPaperID().toUInt64();
        return 0;
    }

    static MonoObject* Entity_GetScriptInstance(PaperID entityID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CORE_ASSERT(scene, "");
        Entity entity = scene->GetEntity(entityID);
        CORE_ASSERT(entity, "");

        EntityInstance* entityInstance = ScriptEngine::GetEntityScriptInstance(entity.GetPaperID());

        if (entityInstance)
        {
			MonoObject* instance = entityInstance->GetMonoInstance();
            return instance;
        }
        return nullptr;
    }

    //Components
    static void TransformComponent_GetPosition(PaperID entityID, glm::vec3* outPosition)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        *outPosition = scene->GetEntity(entityID).GetComponent<TransformComponent>().position;
    }

    static void TransformComponent_SetPosition(PaperID entityID, glm::vec3* inPosition)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        scene->GetEntity(entityID).GetComponent<TransformComponent>().position = *inPosition;
    }

    static void TransformComponent_GetRotation(PaperID entityID, glm::vec3* outRotation)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        *outRotation = scene->GetEntity(entityID).GetComponent<TransformComponent>().rotation;
    }

    static void TransformComponent_SetRotation(PaperID entityID, glm::vec3* inRotation)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        scene->GetEntity(entityID).GetComponent<TransformComponent>().rotation = *inRotation;
    }

    static void TransformComponent_GetScale(PaperID entityID, glm::vec3* outScale)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        *outScale = scene->GetEntity(entityID).GetComponent<TransformComponent>().scale;
    }

    static void TransformComponent_SetScale(PaperID entityID, glm::vec3* inScale)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        scene->GetEntity(entityID).GetComponent<TransformComponent>().scale = *inScale;
    }

    static MonoString* DataComponent_GetName(PaperID entityID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& dc = scene->GetEntity(entityID).GetComponent<DataComponent>();
        return mono_string_new(mono_domain_get(), dc.name.c_str());
    }

    static void DataComponent_SetName(PaperID entityID, MonoString* name)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& dc = scene->GetEntity(entityID).GetComponent<DataComponent>();
        char* name_c = mono_string_to_utf8(name);
        dc.name = name_c;
        mono_free(name_c);
    }

    static MonoString* DataComponent_GetTags(PaperID entityID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& dc = scene->GetEntity(entityID).GetComponent<DataComponent>();
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

    static void DataComponent_SetTags(PaperID entityID, MonoString* name)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& dc = scene->GetEntity(entityID).GetComponent<DataComponent>();
        char* name_c = mono_string_to_utf8(name);
        std::stringstream tagList(name_c);
        mono_free(name_c);
        dc.tags.clear();
        std::string temp;
        while (getline(tagList, temp, ',')) {
            dc.tags.push_back(temp);
        }
    }

    static void SpriteComponent_GetColor(PaperID entityID, glm::vec4* outColor)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityID).GetComponent<SpriteComponent>();
        *outColor = sc.color;
    }

    static void SpriteComponent_SetColor(PaperID entityID, glm::vec4* inColor)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityID).GetComponent<SpriteComponent>();
        sc.color = *inColor;
    }

    static void SpriteComponent_GetTexture(PaperID entityID, TextureData* outTextureData)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityID).GetComponent<SpriteComponent>();
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

    static void SpriteComponent_SetTexture(PaperID entityID, MonoString* inTextureFilePath)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityID).GetComponent<SpriteComponent>();
        sc.texture = DataPool::GetAssetTexture(ScriptUtils::MonoStringToStdString(inTextureFilePath), true);
    }

    static float SpriteComponent_GetTilingFactor(PaperID entityID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityID).GetComponent<SpriteComponent>();
        return sc.tiling_factor;
    }

    static void SpriteComponent_SetTilingFactor(PaperID entityID, float tilingFactor)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityID).GetComponent<SpriteComponent>();
        sc.tiling_factor = tilingFactor;
    }

    static void SpriteComponent_GetUV0(PaperID entityID, glm::vec2* outUV0)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityID).GetComponent<SpriteComponent>();

        *outUV0 = sc.tex_coords[0];
    }

    static void SpriteComponent_SetUV0(PaperID entityID, glm::vec2* inUV0)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityID).GetComponent<SpriteComponent>();

        sc.tex_coords[0] = *inUV0;
        sc.tex_coords[1].y = (*inUV0).y;
        sc.tex_coords[3].x = (*inUV0).x;
    }

    static void SpriteComponent_GetUV1(PaperID entityID, glm::vec2* outUV1)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityID).GetComponent<SpriteComponent>();

        *outUV1 = sc.tex_coords[2];
    }

    static void SpriteComponent_SetUV1(PaperID entityID, glm::vec2* inUV1)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityID).GetComponent<SpriteComponent>();

        sc.tex_coords[1].x = (*inUV1).x;
        sc.tex_coords[2] = *inUV1;
        sc.tex_coords[3].y = (*inUV1).y;
    }

    static int SpriteComponent_GetGeometry(PaperID entityID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityID).GetComponent<SpriteComponent>();

        return (int)sc.geometry;
    }

    static void SpriteComponent_SetGeometry(PaperID entityID, int geometry)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityID).GetComponent<SpriteComponent>();

        sc.geometry = (Geometry)geometry;
    }

    static float SpriteComponent_GetThickness(PaperID entityID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityID).GetComponent<SpriteComponent>();

        return sc.thickness;
    }

    static void SpriteComponent_SetThickness(PaperID entityID, float thickness)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityID).GetComponent<SpriteComponent>();

        sc.thickness = thickness;
    }

    static float SpriteComponent_GetFade(PaperID entityID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityID).GetComponent<SpriteComponent>();

        return sc.fade;
    }

    static void SpriteComponent_SetFade(PaperID entityID, float fade)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& sc = scene->GetEntity(entityID).GetComponent<SpriteComponent>();

        sc.fade = fade;
    }

    static void LineComponent_GetColor(PaperID entityID, glm::vec4* outColor)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& lc = scene->GetEntity(entityID).GetComponent<LineComponent>();
        *outColor = lc.color;
    }

    static void LineComponent_SetColor(PaperID entityID, glm::vec4* inColor)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& lc = scene->GetEntity(entityID).GetComponent<LineComponent>();
        lc.color = *inColor;
    }

    static float LineComponent_GetThickness(PaperID entityID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& lc = scene->GetEntity(entityID).GetComponent<LineComponent>();

        return lc.thickness;
    }

    static void LineComponent_SetThickness(PaperID entityID, float thickness)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& lc = scene->GetEntity(entityID).GetComponent<LineComponent>();

        lc.thickness = thickness;
    }

    static void TextComponent_GetColor(PaperID entityID, glm::vec4* outColor)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& tc = scene->GetEntity(entityID).GetComponent<TextComponent>();
        *outColor = tc.color;
    }

    static void TextComponent_SetColor(PaperID entityID, glm::vec4* inColor)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& tc = scene->GetEntity(entityID).GetComponent<TextComponent>();
        tc.color = *inColor;
    }

    static MonoString* TextComponent_GetText(PaperID entityID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& tc = scene->GetEntity(entityID).GetComponent<TextComponent>();
        return MONO_STRING(tc.text.c_str());
    }

    static void TextComponent_SetText(PaperID entityID, MonoString* text)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& tc = scene->GetEntity(entityID).GetComponent<TextComponent>();
        tc.text = ScriptUtils::MonoStringToStdString(text);
    }

    static MonoString* TextComponent_GetFontPath(PaperID entityID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& tc = scene->GetEntity(entityID).GetComponent<TextComponent>();
        return MONO_STRING(tc.font->GetFilePath().c_str());
    }

    static void TextComponent_SetFontPath(PaperID entityID, MonoString* fontPath)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& tc = scene->GetEntity(entityID).GetComponent<TextComponent>();
        tc.font = DataPool::GetFont(ScriptUtils::MonoStringToStdString(fontPath), true);
    }

    static float CameraComponent_GetFixedAspectRatio(PaperID entityID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& cc = scene->GetEntity(entityID).GetComponent<CameraComponent>();
        return cc.fixedAspectRatio;
    }

    static void CameraComponent_SetFixedAspectRatio(PaperID entityID, bool fixedAspectRatio)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& cc = scene->GetEntity(entityID).GetComponent<CameraComponent>();
        cc.fixedAspectRatio = fixedAspectRatio;
    }

    static float CameraComponent_GetPrimary(PaperID entityID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& cc = scene->GetEntity(entityID).GetComponent<CameraComponent>();
        return cc.primary;
    }

    static void CameraComponent_SetPrimary(PaperID entityID, bool primary)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& cc = scene->GetEntity(entityID).GetComponent<CameraComponent>();
        cc.primary = primary;
    }

    static MonoString* ScriptComponent_GetScriptClassName(PaperID entityID)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& scrc = scene->GetEntity(entityID).GetComponent<ScriptComponent>();
        return ScriptUtils::StdStringToMonoString(scrc.scriptClassName);
    }

    static void ScriptComponent_SetScriptClassName(PaperID entityID, MonoString* scriptClassName)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        auto& scrc = scene->GetEntity(entityID).GetComponent<ScriptComponent>();
        scrc.scriptClassName = ScriptUtils::MonoStringToStdString(scriptClassName);
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
        SCR_ADD_INTRERNAL_CALL(EntityCamera_GetPerspectiveFOV);
        SCR_ADD_INTRERNAL_CALL(EntityCamera_SetPerspectiveFOV);
        SCR_ADD_INTRERNAL_CALL(EntityCamera_GetPerspectiveNearClip);
        SCR_ADD_INTRERNAL_CALL(EntityCamera_SetPerspectiveNearClip);
        SCR_ADD_INTRERNAL_CALL(EntityCamera_GetPerspectiveFarClip);
        SCR_ADD_INTRERNAL_CALL(EntityCamera_SetPerspectiveFarClip);
        SCR_ADD_INTRERNAL_CALL(EntityCamera_GetOrthographicSize);
        SCR_ADD_INTRERNAL_CALL(EntityCamera_SetOrthographicSize);
        SCR_ADD_INTRERNAL_CALL(EntityCamera_GetOrthographicNearClip);
        SCR_ADD_INTRERNAL_CALL(EntityCamera_SetOrthographicNearClip);
        SCR_ADD_INTRERNAL_CALL(EntityCamera_GetOrthographicFarClip);
        SCR_ADD_INTRERNAL_CALL(EntityCamera_SetOrthographicFarClip);
        SCR_ADD_INTRERNAL_CALL(EntityCamera_SetViewportSize);

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

    	SCR_ADD_INTRERNAL_CALL(CameraComponent_GetFixedAspectRatio);
    	SCR_ADD_INTRERNAL_CALL(CameraComponent_SetFixedAspectRatio);
    	SCR_ADD_INTRERNAL_CALL(CameraComponent_GetPrimary);
    	SCR_ADD_INTRERNAL_CALL(CameraComponent_SetPrimary);

    	SCR_ADD_INTRERNAL_CALL(ScriptComponent_GetScriptClassName);
    	SCR_ADD_INTRERNAL_CALL(ScriptComponent_SetScriptClassName);
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

            MonoType* managedType = mono_reflection_type_from_name(csStructName.data(), ScriptEngine::GetCoreAssembly()->GetMonoAssemblyImage());
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

