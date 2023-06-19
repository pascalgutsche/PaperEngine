workspace "PaperEditor"
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
IncludeDir["MINIAUDIO"] = "%{wks.location}/ngine/lib/miniaudio"
IncludeDir["MSDFGEN"] = "%{wks.location}/engine/lib/msdf-atlas-gen/msdfgen"
IncludeDir["MSDF_ATLAS_GEN"] = "%{wks.location}/engine/lib/msdf-atlas-gen/msdf-atlas-gen"
IncludeDir["SPDLOG"] = "%{wks.location}/engine/lib/spdlog/include"
IncludeDir["STB_IMAGE"] = "%{wks.location}/engine/lib/stb_image"
IncludeDir["YAMLCPP"] = "%{wks.location}/engine/lib/yaml-cpp/include"
IncludeDir["STDUUID"] = "%{wks.location}/engine/lib/STDUUID/include"
IncludeDir["ENTT"] = "%{wks.location}/engine/lib/entt/src"

group "dependencies"
include "engine/lib/glad"
include "engine/lib/glfw"
include "engine/lib/imgui"
include "engine/lib/implot"
include "engine/lib/miniaudio"
include "engine/lib/msdf-atlas-gen"
include "engine/lib/spdlog"
include "engine/lib/stb_image"
include "engine/lib/yaml-cpp"
group ""

group "engine"
include "engine"
group ""

group "editor"
include "PaperEditor"
group ""