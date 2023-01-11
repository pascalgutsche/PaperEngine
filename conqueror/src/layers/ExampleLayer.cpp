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
	ImGui::Begin("Bunker1");
	ImGui::Text("bunker");
	ImGui::End();

	ImGui::Begin("Bunker2");
	ImGui::Text("bunker");
	ImGui::End();

	ImGui::Begin("Bunker3");
	ImGui::Text("bunker");
	ImGui::End();

	ImGui::Begin("Bunker4");
	ImGui::Text("bunker");
	ImGui::End();
}

void ExampleLayer::event(core::Event& event)
{

}
