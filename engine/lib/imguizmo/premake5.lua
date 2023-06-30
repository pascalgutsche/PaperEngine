project "imguizmo"
	kind "StaticLib"
	language "C++"
    staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/ImGuizmo/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{IncludeDir.IMGUI}",
		"%{IncludeDir.IMGUIZMO}"
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
