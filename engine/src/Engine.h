#pragma once
#pragma comment(lib, "rpcrt4.lib")

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#define GLFW_INCLUDE_NONE

//C++ Libraries
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstring>
#include <ctime>
#include <exception>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

//C Libraries
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent/include/dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

//GLM
//glm itself
#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
//vectors
#include <GLM/vec2.hpp>
#include <GLM/vec3.hpp>
#include <GLM/vec4.hpp>
#include <GLM/vector_relational.hpp>
//matrixes
#include <GLM/mat2x2.hpp>
#include <GLM/mat2x3.hpp>
#include <GLM/mat2x4.hpp>
#include <GLM/mat3x2.hpp>
#include <GLM/mat3x3.hpp>
#include <GLM/mat3x4.hpp>
#include <GLM/mat4x2.hpp>
#include <GLM/mat4x3.hpp>
#include <GLM/mat4x4.hpp>
#include <GLM/matrix.hpp>

//entt
#include <entt/entt.hpp>

//Core
#include "core/utils/Log.h"


#ifdef CORE_PLATFORM_WINDOWS
	#include <Windows.h>
#endif