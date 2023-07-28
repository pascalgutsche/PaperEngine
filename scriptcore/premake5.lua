project "scriptcore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("%{wks.location}/PaperEditor/resources/scripts")
	objdir ("%{wks.location}/PaperEditor/resources/scripts/ints")

	pchheader "Engine.h"
	pchsource "%{wks.location}/engine/src/Engine.cpp"

	files
	{
		"Source/**.cs",
		"Properties/**.cs"
	}

	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"
		
		
 