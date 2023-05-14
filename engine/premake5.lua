project "engine"
	kind "StaticLib"
	language "C++"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "Engine.h"
	pchsource "%{wks.location}/engine/src/Engine.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"lib",
		"src",
		"src/core",
		"src/platform",
		"%{IncludeDir.DIRENT}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.IMGUI}",
		"%{IncludeDir.IMPLOT}",
		"%{IncludeDir.MINIAUDIO}",
		"%{IncludeDir.MSDFGEN}",
		"%{IncludeDir.MSDF_ATLAS_GEN}",
		"%{IncludeDir.SPDLOG}",
		"%{IncludeDir.STB_IMAGE}",
		"%{IncludeDir.YAMLCPP}"
	}

	defines 
	{
		"_CRT_SECURE_NO_WARNINGS",
		"CORE_ENABLE_ASSERTS",
		"GLFW_INCLUDE_NONE"
	}

	links 
	{
		"glfw",
		"glad",
		"imgui",
		"implot",
		"miniaudio",
		"msdf-atlas-gen",
		"spdlog",
		"stb_image",
		"opengl32.lib"
	}

	disablewarnings 
	{
		"4005",
		"4244",
		"4312",
		"4267",
		"4251"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"CORE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "BUILD_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "BUILD_RELEASE"
		optimize "On"
