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
		"%{IncludeDir.IMGUIZMO}",
		"%{IncludeDir.MINIAUDIO}",
		"%{IncludeDir.MSDFGEN}",
		"%{IncludeDir.MSDF_ATLAS_GEN}",
		"%{IncludeDir.SPDLOG}",
		"%{IncludeDir.STB_IMAGE}",
		"%{IncludeDir.YAMLCPP}",
		"%{IncludeDir.STDUUID}",
		"%{IncludeDir.ENTT}",
		"%{IncludeDir.BOOST}"
	}

	defines 
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
		--"GLM_FORCE_XYZW_ONLY"
	}

	links 
	{
		"glfw",
		"glad",
		"imgui",
		"implot",
		"imguizmo",
		"miniaudio",
		"msdf-atlas-gen",
		"spdlog",
		"stb_image",
		"yaml-cpp",
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
		cppdialect "C++20"
		systemversion "latest"

		defines
		{
			"CORE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines { "BUILD_DEBUG", "CORE_ENABLE_ASSERTS" }
		symbols "On"

	filter "configurations:Release"
		defines "BUILD_RELEASE"
		optimize "On"
		
		
