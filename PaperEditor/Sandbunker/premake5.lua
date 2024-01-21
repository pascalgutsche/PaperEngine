PAPER_DIR = os.getenv("PAPER_DIR")

include (path.join(PAPER_DIR, "premake", "premake_customization", "solution_items.lua"))
include (path.join(PAPER_DIR, "PaperEditor", "resources", "lua-buildsystem", "PaperScripting.lua"))


workspace "Sandbunker"
	configurations { "Debug", "Release" }
	targetdir "build/bin"
	objdir "build/int"

	flags
	{
		"MultiProcessorCompile"
	}

--group "Paper-Lib"
--project "scriptcore"
--	location "%{PAPER_DIR}/scriptcore"
--	kind "SharedLib"
--	language "C#"
--	dotnetframework "4.7.2"
--
--	targetdir ("%{PAPER_DIR}/PaperEditor/resources/scripts")
--	objdir ("%{PAPER_DIR}/PaperEditor/resources/scripts/ints")
--
--	files {
--		"%{PAPER_DIR}/scriptcore/Source/**.cs",
--	}
--
--	linkAppReferences()
--
--	filter "configurations:Debug"
--		optimize "Off"
--		symbols "Default"
--
--	filter "configurations:Release"
--		optimize "On"
--		symbols "Default"
--
--group ""

project "Sandbunker"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetname "Sandbunker"
	targetdir ("%{prj.location}/script_bins/bin")
	objdir ("%{prj.location}/script_bins/int")

	files  {
		"assets/**.cs", 
	}

	linkAppReferences()

	links 
	{
		"%{PAPER_DIR}/PaperEditor/resources/scripts/scriptcore.dll"
	}

	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"
