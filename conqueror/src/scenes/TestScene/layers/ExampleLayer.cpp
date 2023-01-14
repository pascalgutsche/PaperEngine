#include "ExampleLayer.h"

void ExampleLayer::attach()
{

}

void ExampleLayer::detach()
{

}

void ExampleLayer::update(const float dt)
{

}

void ExampleLayer::imgui(const float dt)
{
}

bool ExampleLayer::OnMouseScrolled(core::MouseScrolledEvent& e)
{
    if (core::SCENE().getCamera()->position.z > 1)
		core::SCENE().getCamera()->position.z += 10.0f * core::Application::GetDT() * e.getYOffset();
    else if (e.getYOffset() > 0)
        core::SCENE().getCamera()->position.z += 10.0f * core::Application::GetDT() * e.getYOffset();
    core::SCENE().getCamera()->fov += 1.0f * core::Application::GetDT() * e.getXOffset();
    return true;
}


