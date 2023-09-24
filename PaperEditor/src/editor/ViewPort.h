#pragma once

class PaperLayer;

struct ViewPort
{
public:
	Shr<EditorCamera> camera;
	Shr<Framebuffer> framebuffer;
	Shr<Framebuffer> previewFramebuffer;

	PaperID previewEntityID = 0;

	std::string name;

	static inline ImVec2 previewPadding = ImVec2(5.0f, 5.0f);
	static inline ImVec2 previewSize = ImVec2(400, 200);

	ViewPort(const std::string& name)
		: camera(MakeShr<EditorCamera>()), name(name), viewport_size(), viewport_bounds{}
	{
		FramebufferSpecification spec;
		spec.attachment = {FramebufferTexFormat::RGBA8, FramebufferTexFormat::RED_INTEGER, FramebufferTexFormat::Depth};
		spec.width = Application::GetWindow()->GetWidth();
		spec.height = Application::GetWindow()->GetHeight();
		framebuffer = Framebuffer::CreateBuffer(spec);

		spec.width = previewSize.x;
		spec.height = previewSize.y;
		previewFramebuffer = Framebuffer::CreateBuffer(spec);
	}

	void Panel(PaperLayer* peLayer);

	void Resize();

	glm::vec2 viewport_pos_abs{};

	glm::vec2 viewport_size;
	glm::vec2 viewport_bounds[2];

	bool viewport_focused = false;
	bool viewport_hovered = false;
	bool viewport_active = false;

	bool last_viewport_active = false;

	bool is_visible = false;
};
