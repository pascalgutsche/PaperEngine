#pragma once

#ifdef CORE_PLATFORM_WINDOWS
	#ifdef CORE_BUILD_DLL
		#define CORE_API __declspec(dllexport)
	#else
		#define CORE_API __declspec(dllimport)
	#endif
#else
	#error This Engine only supports Windows!
#endif


#ifdef DEBUG_CHANGE_PATH
	#define DEBUG_PATH(x) std::string("../../../" + std::string(x)).c_str()
#else
	#define DEBUG_PATH(x) x
#endif