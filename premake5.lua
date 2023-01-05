workspace "Client"
	architecture "x64"
	startproject "conqueror"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--GLFW
IncludeDir = {}
IncludeDir["GLFW"] = "core/lib/GLFW/include"
IncludeDir["GLAD"] = "core/lib/GLAD/include"


include "core/lib/GLFW"
include "core/lib/GLAD"


project "core"
	location "core"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "_Core.h"
	pchsource "core/src/core/_Core.cpp"

	files
	{
		"%{prj.name}/lib/**.h",
		"%{prj.name}/lib/**.gch",
		"%{prj.name}/lib/**.hpp",
		"%{prj.name}/lib/**.cpp",
		"%{prj.name}/lib/**.c",
		"%{prj.name}/lib/**.lib",
		"%{prj.name}/lib/**.dll",
		"%{prj.name}/lib/**.a",

		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"lib",
		"%{prj.name}/lib",
		"%{prj.name}/src",
		"%{prj.name}/src/core",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}"

	}

	links 
	{
		"GLFW",
		"GLAD",
		"opengl32.lib"
	}

	disablewarnings 
	{
		"4005",
		"4244",
		"4312",
		"4267"

	}
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"CORE_PLATFORM_WINDOWS",
			"CORE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands 
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/conqueror/")
		}

	filter "configurations:Debug"
		defines "CORE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "CORE_RELEASE"
		optimize "On"


project "conqueror"
	location "conqueror"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/lib/**.h",
		"%{prj.name}/lib/**.gch",
		"%{prj.name}/lib/**.hpp",
		"%{prj.name}/lib/**.cpp",
		"%{prj.name}/lib/**.c",
		"%{prj.name}/lib/**.lib",
		"%{prj.name}/lib/**.dll",
		"%{prj.name}/lib/**.a",

		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"lib",
		"%{prj.name}/lib",
		"%{prj.name}/src",
		"%{prj.name}/src/core",
		"core/src",
		"core/lib"
	}

	links
	{
		"core"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"CORE_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "CORE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "CORE_RELEASE"
		optimize "On"