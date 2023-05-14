project "implot"
	kind "StaticLib"
	language "C++"
    staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"implot.h",
		"implot.cpp",
		"implot_demo.cpp",
		"implot_internal.h",
		"implot_items.cpp"
	}

	includedirs
	{
		"%{IncludeDir.IMGUI}"
	}

	disablewarnings
	{
		"26495"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"

	filter "configurations:Debug"
		symbols "on"

	filter "configurations:Release"
		optimize "on"
