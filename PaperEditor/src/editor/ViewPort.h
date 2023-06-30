#pragma once

class PELayer;

struct ViewPort
{
public:
	Shr<EditorCamera> camera;
	Shr<Framebuffer> framebuffer;

	std::string name;

	ViewPort(const std::string& name)
		: camera(MakeShr<EditorCamera>()), name(name), viewport_size(), viewport_bounds{}
	{
		FramebufferSpecification spec;
		spec.attachment = {FramebufferTexFormat::RGBA8, FramebufferTexFormat::RED_INTEGER, FramebufferTexFormat::Depth};
		spec.width = Application::GetWindow()->GetWidth();
		spec.height = Application::GetWindow()->GetHeight();
		framebuffer = Framebuffer::CreateBuffer(spec);
	}

	void Panel(PELayer* peLayer);

	glm::vec2 viewport_pos_abs{};

	glm::vec2 viewport_size;
	glm::vec2 viewport_bounds[2];

	bool viewport_focused = false;
	bool viewport_hovered = false;

	bool last_viewport_focused = false;

	bool is_visible = false;
};
