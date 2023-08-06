PAPER_DIR = os.getenv("PAPER_DIR")

include (path.join(PAPER_DIR, "premake", "premake_customization", "solution_items.lua"))
include (path.join(PAPER_DIR, "PaperEditor", "resources", "lua-buildsystem", "PaperScripting.lua"))


workspace "SandboxProject"
	architecture "x64"
	startproject "Sandbox"
	configurations { "Debug", "Release" }

	flags
	{
		"MultiProcessorCompile"
	}

group "Paper-Lib"
project "scriptcore"
	location "%{PAPER_DIR}/scriptcore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("%{PAPER_DIR}/PaperEditor/resources/scripts")
	objdir ("%{PAPER_DIR}/PaperEditor/resources/scripts/ints")

	files {
		"%{PAPER_DIR}/scriptcore/Source/**.cs",
	}

	linkAppReferences()

	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

group ""

project "Sandbox"
	location "assets/scripts"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetname "Sandbox"
	targetdir ("%{prj.location}/bin")
	objdir ("%{prj.location}/int")

	files  {
		"assets/scripts/Source/**.cs", 
	}

	linkAppReferences()

	links 
	{
		"scriptcore"
	}

	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"
