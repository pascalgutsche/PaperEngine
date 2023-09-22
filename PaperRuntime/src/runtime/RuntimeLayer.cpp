#include "Runtime.h"
#include "RuntimeLayer.h"

#include "project/ProjectSerializer.h"

#include "renderer/Renderer2D.h"
#include "scripting/ScriptEngine.h"
#include "utils/FileSystem.h"

#include <imgui/misc/cpp/imgui_stdlib.h>

RuntimeLayer::RuntimeLayer()
{
	FramebufferSpecification spec;
	spec.attachment = { FramebufferTexFormat::RGBA8, FramebufferTexFormat::RED_INTEGER, FramebufferTexFormat::Depth };
	spec.width = Application::GetWindow()->GetWidth();
	spec.height = Application::GetWindow()->GetHeight();
	framebuffer = Framebuffer::CreateBuffer(spec);
}

RuntimeLayer::~RuntimeLayer()
{
}

void RuntimeLayer::OnAttach()
{
	do
	{
		std::filesystem::path projPath = FileSystem::OpenFile({ {"Paper Project", "pproj"} });
		if (projPath.string().find("Sandbunker") != std::string::npos)
		{
			std::filesystem::current_path(std::filesystem::path("../") / "PaperEditor");
		}
		UseProject(projPath);
	} while (!Project::GetActive());
}

void RuntimeLayer::OnDetach()
{
}

void RuntimeLayer::Update(const float dt)
{
}

void RuntimeLayer::OnEvent(Event& event)
{
}

void RuntimeLayer::Imgui(const float dt)
{
	framebuffer->Bind();

	RenderCommand::ClearColor(glm::vec4(0.0f));
	RenderCommand::Clear();


	RenderCommand::ClearColor(glm::vec4(1.0f, 0.1f, 0.9f, 1.0f));
	RenderCommand::Clear();
	framebuffer->ClearAttachment(1, -1);

	RenderCommand::ClearStats();
	RenderCommand::Clear();

	RenderCommand::ClearStats();

	if (Scene::GetActive())
		Scene::GetActive()->OnRuntimeUpdate();

	framebuffer->Unbind();


	const Shr<Scene> activeScene = Scene::GetActive();
	glm::vec2 windowSize(Application::GetWindow()->GetWidth(), Application::GetWindow()->GetHeight());
	if (activeScene && windowSize.x > 0.0f && windowSize.y > 0.0f)
		activeScene->OnViewportResize(windowSize.x, windowSize.y);

	if (FramebufferSpecification spec = framebuffer->GetSpecification();
		windowSize.x > 0.0f && windowSize.y > 0.0f && // zero sized framebuffer is invalid
		(spec.width != windowSize.x || spec.height != windowSize.y))
	{
		const Shr<Scene> activeScene = Scene::GetActive();
		framebuffer->Resize((uint32_t)windowSize.x, (uint32_t)windowSize.y);

		//camera->aspect_ratio = windowSize.x / windowSize.y;
		//if (activeScene)
		//	activeScene->OnViewportResize(viewport_size.x, viewport_size.y);
	}

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
	window_flags |= ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	ImGuiViewport& viewport = *ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport.Pos);
	ImGui::SetNextWindowSize(viewport.Size);
	ImGui::SetNextWindowViewport(viewport.ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("docking", nullptr, window_flags);
	ImGui::PopStyleVar(3);
	

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	uint32_t textureID = framebuffer->GetColorID(0);
	ImGui::Image((void*)textureID, ImVec2(viewportPanelSize.x, viewportPanelSize.y), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	ImGui::End();
}

void RuntimeLayer::UseProject(const std::filesystem::path& projPath)
{
	if (!std::filesystem::exists(projPath))
	{
		LOG_CORE_ERROR("Project '{}' does not exist");
		return;
	}

	LOG_TRACE("Using project '{}'", projPath);

	std::filesystem::path projectFilePath = projPath;
	if (std::filesystem::is_directory(projectFilePath))
		projectFilePath = projectFilePath / "Project.pproj";

	if (!std::filesystem::exists(projPath))
	{
		LOG_CORE_ERROR("Project '{}' does not exist");
		return;
	}

	Shr<Project> project = ProjectSerializer::Deserialize(projectFilePath);

	UseProject(project);
}

void RuntimeLayer::UseProject(const Shr<Project>& project)
{
	if (Project::GetActive())
		CloseProject();

	Project::SetActive(project);

	if (std::filesystem::exists(Project::GetScriptBinaryFilePath()))
	{
		ScriptEngine::AddAppAssembly(Project::GetScriptBinaryFilePath());
	}

	OpenScene(Project::GetStartScene());

	ChangeWindowTitle();
}

void RuntimeLayer::CloseProject()
{
	Scene::SetActive(nullptr);

	ScriptEngine::ResetEngine();

	Project::SetActive(nullptr);
}

void RuntimeLayer::OpenScene(Shr<Scene> scene)
{
	if (!scene) return;

	if (Scene::GetActive())
	{
		CloseScene();
	}

	Scene::SetActive(scene);

	ChangeWindowTitle();
	sceneChanged = true;

	if (Scene::GetActive())
		Scene::GetActive()->OnRuntimeStart();
}

void RuntimeLayer::CloseScene()
{
	if (!Scene::GetActive()) return;

	Scene::GetActive()->OnRuntimeStop();

	Scene::SetActive(nullptr);
}

void RuntimeLayer::ChangeWindowTitle() const
{
	std::string title;
	if (Project::GetActive() && Scene::GetActive())
		title = fmt::format("PaperRuntime - {0} - {1} - RenderAPI: {2}", Project::GetProjectName(), Scene::GetActive()->GetName(), RenderAPIToString(RenderAPI::GetAPI()));
	else if (Project::GetActive())
		title = fmt::format("PaperRuntime - {0} - RenderAPI: {1}", Project::GetProjectName(), RenderAPIToString(RenderAPI::GetAPI()));
	else
		title = fmt::format("PaperRuntime - RenderAPI: {0}", RenderAPIToString(RenderAPI::GetAPI()));

	Application::GetInstance()->GetWindow()->SetTitle(title);
}