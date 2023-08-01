#include "Engine.h"
#include "ScriptGlue.h"

#include <mono/jit/jit.h>

#include <glm/gtx/string_cast.hpp>

#include "ScriptEngine.h"
#include "generic/Scene.h"

#include "Components.h"
#include "event/Input.h"

namespace Paper
{
#define SCR_ADD_INTRERNAL_CALL(Name) mono_add_internal_call("Paper.InternalCalls::" #Name, Name)

    static void NativeLog(MonoString* string, int level)
    {
        char* cstr = mono_string_to_utf8(string);
        std::string str(cstr);
        mono_free(cstr);

        switch (level)
        {
            case 0: LOG_SCR_DEBUG(str);
                break;
            case 1: LOG_SCR_TRACE(str);
                break;
            case 2: LOG_SCR_WARN(str);
                break;
            case 3: LOG_SCR_ERROR(str);
                break;
        }
    }

    static void NativeLog_Vector(glm::vec3* parameter, glm::vec3* outResult)
    {
        LOG_SCR_TRACE("Value: {0}", glm::to_string(*parameter));

        //*outResult = glm::cross(*parameter, glm::vec3(parameter->x, parameter->y, -parameter->z));
        *outResult = *parameter;
    }

    static void IsKeyPressed(int code, bool* pressed)
    {
        *pressed = Input::IsKeyPressed((Key)code);
    }

    static void Entity_GetPosition(UUID entityUUID, glm::vec3* outPosition)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        *outPosition = scene->GetEntity(entityUUID).GetComponent<TransformComponent>().position;
    }

    static void Entity_SetPosition(UUID entityUUID, glm::vec3* inPosition)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        scene->GetEntity(entityUUID).GetComponent<TransformComponent>().position = *inPosition;
    }

    static void Entity_GetRotation(UUID entityUUID, glm::vec3* outRotation)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        *outRotation = scene->GetEntity(entityUUID).GetComponent<TransformComponent>().rotation;
    }

    static void Entity_SetRotation(UUID entityUUID, glm::vec3* inRotation)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        scene->GetEntity(entityUUID).GetComponent<TransformComponent>().rotation = *inRotation;
    }

    static void Entity_GetScale(UUID entityUUID, glm::vec3* outScale)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        *outScale = scene->GetEntity(entityUUID).GetComponent<TransformComponent>().scale;
    }

    static void Entity_SetScale(UUID entityUUID, glm::vec3* inScale)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        scene->GetEntity(entityUUID).GetComponent<TransformComponent>().scale = *inScale;
    }

	void ScriptGlue::RegisterFunctions()
	{
        SCR_ADD_INTRERNAL_CALL(NativeLog);
        SCR_ADD_INTRERNAL_CALL(NativeLog_Vector);

        SCR_ADD_INTRERNAL_CALL(IsKeyPressed);

        SCR_ADD_INTRERNAL_CALL(Entity_GetPosition);
        SCR_ADD_INTRERNAL_CALL(Entity_SetPosition);
        SCR_ADD_INTRERNAL_CALL(Entity_GetRotation);
        SCR_ADD_INTRERNAL_CALL(Entity_SetRotation);
        SCR_ADD_INTRERNAL_CALL(Entity_GetScale);
        SCR_ADD_INTRERNAL_CALL(Entity_SetScale);
	}
}

