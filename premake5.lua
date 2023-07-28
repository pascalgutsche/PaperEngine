workspace "PaperEngine"
	architecture "x64"
	startproject "PaperEditor"
	staticruntime "off"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["DIRENT"] = "%{wks.location}/engine/lib/dirent/include"
IncludeDir["GLAD"] = "%{wks.location}/engine/lib/glad/include"
IncludeDir["GLFW"] = "%{wks.location}/engine/lib/glfw/include"
IncludeDir["GLM"] = "%{wks.location}/engine/lib/glm"
IncludeDir["IMGUI"] = "%{wks.location}/engine/lib/imgui"
IncludeDir["IMPLOT"] = "%{wks.location}/engine/lib/implot"
IncludeDir["IMGUIZMO"] = "%{wks.location}/engine/lib/imguizmo/include"
IncludeDir["MINIAUDIO"] = "%{wks.location}/ngine/lib/miniaudio"
IncludeDir["MSDFGEN"] = "%{wks.location}/engine/lib/msdf-atlas-gen/msdfgen"
IncludeDir["MSDF_ATLAS_GEN"] = "%{wks.location}/engine/lib/msdf-atlas-gen/msdf-atlas-gen"
IncludeDir["SPDLOG"] = "%{wks.location}/engine/lib/spdlog/include"
IncludeDir["STB_IMAGE"] = "%{wks.location}/engine/lib/stb_image"
IncludeDir["YAMLCPP"] = "%{wks.location}/engine/lib/yaml-cpp/include"
IncludeDir["ENTT"] = "%{wks.location}/engine/lib/entt/src"
IncludeDir["BOOST"] = "%{wks.location}/<engine/lib"
IncludeDir["MONO"] = "%{wks.location}/engine/lib/mono/include"

LibraryDir = {}
LibraryDir["MONO"] = "%{wks.location}/engine/lib/mono/lib/%{cfg.buildcfg}"

Library = {}
Library["ASSIMP_DEBUG"] = "%{wks.location}/engine/lib/assimp/assimp/bin/Debug/assimp-vc143-mtd.lib"
Library["ASSIMP_RELEASE"] = "%{wks.location}/engine/lib/assimp/assimp/bin/Release/assimp-vc143-mt.lib"
Library["MONO"] = "%{LibraryDir.MONO}/libmono-static-sgen.lib"

--Windows
Library["WINSOCK"] = "Ws2_32.lib"
Library["WINMM"] = "Winmm.lib"
Library["WINVERSION"] = "Version.lib"
Library["BCRYPT"] = "Bcrypt.lib"


Binaries = {}
Binaries["Assimp_Debug"] = "%{wks.location}/engine/lib/assimp/bin/Debug/assimp-vc143-mtd.dll"
Binaries["Assimp_Release"] = "%{wks.location}/engine/lib/assimp/assimp/bin/Release/assimp-vc143-mt.dll"



group "dependencies"
include "engine/lib/glad"
include "engine/lib/glfw"
include "engine/lib/imgui"
include "engine/lib/implot"
include "engine/lib/imguizmo"
include "engine/lib/miniaudio"
include "engine/lib/msdf-atlas-gen"
include "engine/lib/spdlog"
include "engine/lib/stb_image"
include "engine/lib/yaml-cpp"
group ""

group "engine"
include "engine"
include "scriptcore"
group ""

group "editor"
include "PaperEditor"
group ""