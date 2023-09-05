project "box2d"
	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	includedirs
	{
		"include",
		"src"
	}

	files
	{
		"src/**.h",
		"src/**.cpp",
		"include/**.h"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "on"

	filter "configurations:Release"
		optimize "on"