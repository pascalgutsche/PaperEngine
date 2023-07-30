#include "Engine.h"
#include "ScriptGlue.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#include <glm/gtx/string_cast.hpp>

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

	void ScriptGlue::RegisterFunctions()
	{
        SCR_ADD_INTRERNAL_CALL(NativeLog);
        SCR_ADD_INTRERNAL_CALL(NativeLog_Vector);
	}
}

