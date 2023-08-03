project "scriptcore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"
	
	linkAppReferences(false)

	targetdir ("../PaperEditor/resources/scripts")
	objdir ("../PaperEditor/resources/scripts/ints")
	
	files
	{
		"Source/**.cs"
	}

	filter "configurations:Debug"
	optimize "Off"
	symbols "Default"

	filter "configurations:Release"
	optimize "On"
	symbols "Default"

		
		
 