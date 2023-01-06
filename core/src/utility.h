#pragma once

#ifdef CORE_PLATFORM_WINDOWS
	#ifdef CORE_BUILD_DLL
		#define CORE_API //__declspec(dllexport)
	#else
		#define CORE_API //__declspec(dllimport)
	#endif
#else
	#error This Engine only supports Windows!
#endif


#ifdef CORE_DEBUG
	#define CORE_ASSERT(x, y) std::string("../../../" + std::string(x)).c_str()
#else
	#define DEBUG_PATH(x) x
#endif

