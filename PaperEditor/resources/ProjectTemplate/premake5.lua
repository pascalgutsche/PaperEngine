PAPER_DIR = os.getenv("PAPER_DIR")

include (path.join(PAPER_DIR, "premake", "premake_customization", "solution_items.lua"))
include (path.join(PAPER_DIR, "PaperEditor", "resources", "lua-buildsystem", "PaperScripting.lua"))


workspace "$PROJ_NAME$"
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

project "$PROJ_NAME$"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetname "$PROJ_NAME$"
	targetdir ("%{prj.location}/assets/scripts/bin")
	objdir ("%{prj.location}/assets/scripts/int")

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
