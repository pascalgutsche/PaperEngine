project "PaperEditor"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "Editor.h"
	pchsource "%{wks.location}/PaperEditor/src/Editor.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"src",
		"%{wks.location}/engine/src",
		"%{wks.location}/engine/src/core",
		"%{wks.location}/engine/lib",
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
		"%{IncludeDir.YAMLCPP}",
		"%{IncludeDir.ENTT}",
		"%{IncludeDir.BOOST}"
	}

	defines 
	{
		"_CRT_SECURE_NO_WARNINGS",
		"CORE_ENABLE_ASSERTS"
	}

	links
	{
		"engine"
	}

	disablewarnings
	{
		"4251",
		"4267",
		"4305",
		"4244"
	}
	
	filter "system:windows"
		cppdialect "C++20"
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
