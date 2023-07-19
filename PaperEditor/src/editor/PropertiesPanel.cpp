#include "Editor.h"
#include "PELayer.h"


#include "WindowsOpen.h"

#include <imgui/misc/cpp/imgui_stdlib.h>

static void DrawFloatControl(const std::string& label, float& value, float resetValue, float content_avail_x)
{
	float padding = 3.0f;
	float line_height = GImGui->Font->FontSize + padding * 2;
	ImVec2 button_size = { line_height + 3.0f, line_height };
	float input_item_width = (content_avail_x - 5.0f) / 3.0f - button_size.x;

	UI::ScopedID id(ImGui::TableGetRowIndex());

	if (ImGui::Button(label.c_str(), button_size))
	{
		value = resetValue;
	};
	ImGui::SameLine();
	ImGui::SetNextItemWidth(input_item_width);
	ImGui::DragFloat(("##" + label).c_str(), &value, 0.1f, 0, 0, "%.2f", 0);
}

static void Draw3FloatControl(const std::string& name, glm::vec3& val, float reset_value = 0.0f, std::initializer_list<std::string> format = { "X", "Y" })
{
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::AlignTextToFramePadding();
	ImGui::TextUnformatted(name.c_str());
	ImGui::TableSetColumnIndex(1);
	UI::ScopedStyle item_spacing(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });
	UI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 2.0f });

	float content_avail_x = ImGui::GetContentRegionAvail().x;

	ASSERT(format.size() == 3, "")

	DrawFloatControl(*(format.begin() + 0), val.x, reset_value, content_avail_x);
	ImGui::SameLine();
	DrawFloatControl(*(format.begin() + 1), val.y, reset_value, content_avail_x);
	ImGui::SameLine();
	DrawFloatControl(*(format.begin() + 2), val.z, reset_value, content_avail_x);
}

static void Draw2FloatControl(const std::string& name, glm::vec2& val, float reset_value = 0.0f, std::initializer_list<std::string> format = { "X", "Y" })
{
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::AlignTextToFramePadding();
	ImGui::TextUnformatted(name.c_str());
	ImGui::TableSetColumnIndex(1);
	UI::ScopedStyle item_spacing(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });
	UI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 2.0f });

	float content_avail_x = ImGui::GetContentRegionAvail().x;

	ASSERT(format.size() == 2, "")

	DrawFloatControl(*(format.begin() + 0), val.x, reset_value, content_avail_x);
	ImGui::SameLine();
	DrawFloatControl(*(format.begin() + 1), val.y, reset_value, content_avail_x);
}

template<typename ComponentType>
using AddedComponentCallbackFn = std::function<void(Entity, ComponentType&)>;

template<typename ComponentType>
void DrawComponentToAddPopup(PELayer* _instance, const std::string& name, AddedComponentCallbackFn<ComponentType> on_added_component = nullptr)
{
	if (_instance->active_entity.HasComponent<ComponentType>()) return;

	if (ImGui::Selectable(name.c_str(), false))
	{
		ComponentType& component = _instance->active_entity.AddComponent<ComponentType>();
		if (on_added_component)
			on_added_component(_instance->active_entity, component);

		ImGui::CloseCurrentPopup();
	}
}

template<typename ComponentType>
using DrawComponentFn = std::function<void(ComponentType&, Entity)>;

template<typename ComponentType>
bool DrawRemoveButton(PELayer* _instance)
{
	ImGui::SameLine(ImGui::GetContentRegionAvail().x - 50);
	{
		UI::ScopedStyle padding(ImGuiStyleVar_ItemInnerSpacing, ImVec2(2.0f, 2.0f));
		if (ImGui::Button("Remove"))
		{
			_instance->active_entity.RemoveComponent<ComponentType>();
			ImGui::TreePop();
			return true;
		}
		return false;
	}
}

template<typename ComponentType>
void DrawComponent(PELayer* _instance, const std::string& name, bool canRemove, DrawComponentFn<ComponentType> draw_components_fn)
{
	if (!_instance->active_entity.HasComponent<ComponentType>()) return;

	bool tree_open = ImGui::TreeNode(name.c_str());

	if (canRemove && DrawRemoveButton<ComponentType>(_instance)) return;

	if (tree_open)
	{
		draw_components_fn(_instance->active_entity.GetComponent<ComponentType>(), _instance->active_entity);
		ImGui::TreePop();
	}

}

void PELayer::PropertiesPanel()
{
	if (!active_entity) return;
	if (properties_panel_first)
		DockPanel("Properties", dock_id_right_bottom);

	UI::ScopedStyle min_width(ImGuiStyleVar_WindowMinSize, ImVec2(400.0f, 0.0f));

	ImGui::Begin("Properties", &show_property_panel);
	ImGui::Text("Name:");
	ImGui::SameLine();
	std::string name = active_entity.GetName();
	ImGui::InputText(("##" + active_entity.GetUUID().toString()).c_str(), &name);
	active_entity.SetName(name);

	ImGui::SameLine(ImGui::GetContentRegionAvail().x - 50);
	{
		UI::ScopedStyle padding(ImGuiStyleVar_ItemInnerSpacing, ImVec2(2.0f, 2.0f));
		if (ImGui::Button("Add"))
		{
			ImGui::OpenPopup("components_add_popup");
		}
	}

	

	if (ImGui::BeginPopup("components_add_popup"))
	{
		DrawComponentToAddPopup<SpriteComponent>(this, "Sprite Component");
		DrawComponentToAddPopup<CircleComponent>(this, "Circle Component");
		DrawComponentToAddPopup<LineComponent>(this, "Line Component");
		DrawComponentToAddPopup<TextComponent>(this, "Text Component");
		ImGui::EndPopup();
	}


	DrawComponent<TransformComponent>(this, "Transform Component", false, 
		[](TransformComponent& tc, Entity entity)
	{
		ImGui::BeginTable("##dummy_id_transform", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_NoClip);
		ImGui::TableSetupColumn("name", 0, 100.0f);
		ImGui::TableSetupColumn("value", ImGuiTableColumnFlags_IndentEnable | ImGuiTableColumnFlags_NoClip, ImGui::GetContentRegionAvail().x - 100.0f);




		Draw3FloatControl("Position", tc.position);
		Draw3FloatControl("Rotation", tc.rotation);
		Draw3FloatControl("Scale", tc.scale);

		ImGui::EndTable();
	});

	DrawComponent<SpriteComponent>(this, "Sprite Component", true,
		[](SpriteComponent& sc, Entity entity)
	{
		if (ImGui::BeginCombo("Geometry Type", GeometryToString(sc.geometry).c_str()))
		{
			for (int i = 0; i <= (int)Geometry::_LAST; i++)
			{
				if (ImGui::Selectable(GeometryToString((Geometry)i).c_str(), sc.geometry == (Geometry)i))
				{
					sc.geometry = (Geometry)i;
				}
			}
			ImGui::EndCombo();
		}

		ImGui::Checkbox("RegisterAlphaPixels[temp]", &sc.register_alpha_pixels_to_event);

		ImGui::BeginTable("##dummy_id_uv", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_NoClip);
		ImGui::TableSetupColumn("name", 0, 100.0f);
		ImGui::TableSetupColumn("value", ImGuiTableColumnFlags_IndentEnable | ImGuiTableColumnFlags_NoClip, ImGui::GetContentRegionAvail().x - 100.0f);

		glm::vec2 uv0 = sc.tex_coords[0];
		glm::vec2 uv1 = sc.tex_coords[2];

		Draw2FloatControl("UV_0", uv0);
		Draw2FloatControl("UV_1", uv1);

		if (uv0 != sc.tex_coords[0] || uv1 != sc.tex_coords[1])
		{
			sc.tex_coords[0] = uv0;
			sc.tex_coords[1] = { uv1.x, uv0.y };
			sc.tex_coords[2] = uv1;
			sc.tex_coords[3] = { uv0.x, uv1.y };
		}

		ImGui::EndTable();
	});

	ImGui::End();

	properties_panel_first = false;
}