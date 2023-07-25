#include "Editor.h"
#include "PELayer.h"


#include "WindowsOpen.h"

#include <imgui/misc/cpp/imgui_stdlib.h>

static void FillNameCol(const std::string& name)
{
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::AlignTextToFramePadding();
	ImGui::TextUnformatted(name.c_str());
	ImGui::TableSetColumnIndex(1);
}

static void SingleFloatControl(const std::string& label, float& value, float min, float max, float speed, float resetValue, float content_avail_x, int count)
{
	float padding = 3.0f;
	float line_height = GImGui->Font->FontSize + padding * 2;
	ImVec2 button_size = { line_height + 3.0f, line_height };
	float input_item_width = (content_avail_x - 5.0f) / (float)count - button_size.x;

	UI::ScopedID id(ImGui::TableGetRowIndex());

	if (ImGui::Button(label.c_str(), button_size))
	{
		value = resetValue;
	};
	ImGui::SameLine();
	ImGui::SetNextItemWidth(input_item_width);
	ImGui::DragFloat(("##" + label).c_str(), &value, 0.1f, min, max, "%.2f", 0);
}

static void Draw3FloatControl(const std::string& name, glm::vec3& val, glm::vec3 min = glm::vec3(0.0f), glm::vec3 max = glm::vec3(0.0f), glm::vec3 speed = glm::vec3(1.0f), glm::vec3 reset_value = glm::vec3(0.0f), std::initializer_list<std::string> format = { "X", "Y", "Z" })
{
	FillNameCol(name);

	UI::ScopedStyle item_spacing(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });
	UI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 2.0f });

	float content_avail_x = ImGui::GetContentRegionAvail().x;

	ASSERT(format.size() == 3, "")

	SingleFloatControl(*(format.begin() + 0), val.x, min.x, max.x, speed.x, reset_value.x, content_avail_x, 3);
	ImGui::SameLine();
	SingleFloatControl(*(format.begin() + 1), val.y, min.y, max.y, speed.y, reset_value.y, content_avail_x, 3);
	ImGui::SameLine();
	SingleFloatControl(*(format.begin() + 2), val.z, min.z, max.z, speed.z, reset_value.z, content_avail_x, 3);
}

static void Draw2FloatControl(const std::string& name, glm::vec2& val, glm::vec2 min = glm::vec2(0.0f), glm::vec2 max = glm::vec2(0.0f), glm::vec2 speed = glm::vec2(1.0f), glm::vec2 reset_value = glm::vec2(0.0f), std::initializer_list<std::string> format = { "X", "Y" })
{
	FillNameCol(name);

	UI::ScopedStyle item_spacing(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });
	UI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 2.0f });

	float content_avail_x = ImGui::GetContentRegionAvail().x;

	ASSERT(format.size() == 2, "")

	SingleFloatControl(*(format.begin() + 0), val.x, min.x, max.x, speed.x,  reset_value.x, content_avail_x, 2);
	ImGui::SameLine();
	SingleFloatControl(*(format.begin() + 1), val.y, min.y, max.y, speed.y,  reset_value.y, content_avail_x, 2);
}

static void DrawFloatControl(const std::string& name, float& val, float min = 0.0f, float max = 0.0f, float speed = 1.0f, float reset_value = 0.0f, std::initializer_list<std::string> format = { "X" })
{
	FillNameCol(name);

	UI::ScopedStyle item_spacing(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });
	UI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 2.0f });

	float content_avail_x = ImGui::GetContentRegionAvail().x;

	ASSERT(format.size() == 1, "")

	SingleFloatControl(*(format.begin() + 0), val, min, max, speed, reset_value, content_avail_x, 1);
}

static void DrawCheckbox(const std::string& name, bool& val)
{
	FillNameCol(name);

	UI::ScopedStyle item_spacing(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });
	UI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 2.0f });

	ImGui::Checkbox(("##" + name).c_str(), &val);
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
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::TreePop();
	}

}


struct ContentTable
{
	ContentTable(const ContentTable&) = delete;
	ContentTable& operator=(const ContentTable&) = delete;
	ContentTable(int space_name_column = 0)
	{
		ImGui::BeginTable(CONST_UI_ID, 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_NoClip);
		ImGui::TableSetupColumn("name", 0, space_name_column);
		float space_left = space_name_column > 0 ? ImGui::GetContentRegionAvail().x - space_name_column : 0;
		ImGui::TableSetupColumn("value", ImGuiTableColumnFlags_IndentEnable | ImGuiTableColumnFlags_NoClip, space_left);
	}
	~ContentTable()
	{
		ImGui::EndTable();
	}
};


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
		DrawComponentToAddPopup<LineComponent>(this, "Line Component");
		DrawComponentToAddPopup<TextComponent>(this, "Text Component");
		ImGui::EndPopup();
	}

	DrawComponent<DataComponent>(this, "Tag Component", false, [](DataComponent& dc, Entity entity)
		{
			int i = 0;
			for (auto& tag : dc.tags)
			{
				ImGui::InputText(("##" + std::to_string(i)).c_str(), &tag);
				i++;
			}
			if (dc.tags.size() == 0)
			{
				ImGui::Text("No tags...");
			}
			if (ImGui::Button("Add Tag"))
			{
				dc.tags.emplace_back("");
			};
		});


	DrawComponent<TransformComponent>(this, "Transform Component", false, 
		[](TransformComponent& tc, Entity entity)
	{
		
		ContentTable transform_section(100);

		Draw3FloatControl("Position", tc.position);
		Draw3FloatControl("Rotation", tc.rotation);
		Draw3FloatControl("Scale", tc.scale);

	});

	DrawComponent<SpriteComponent>(this, "Sprite Component", true,
		[](SpriteComponent& sc, Entity entity)
	{
		{
			std::string name = "Geometry Type";
			ContentTable geometry_section(ImGui::CalcTextSize(name.c_str()).x);
			FillNameCol(name);

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImGui::BeginCombo(CONST_UI_ID, GeometryToString(sc.geometry).c_str()))
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
		}

		if (sc.geometry == Geometry::CIRCLE)
		{
			ContentTable pixelregister_section(ImGui::CalcTextSize("Thickness").x);
			DrawFloatControl("Thickness", sc.thickness, 0, 0, 0.1f, 1.0f, { "TH" });
			DrawFloatControl("Fade", sc.fade, 0, 0, 0.001f, 0.005f, { "FA" });
		}
		

		{
			std::string name = "RegisterAlphaPixels[temp]";
			ContentTable pixelregister_section(ImGui::CalcTextSize(name.c_str()).x);
			DrawCheckbox(name, sc.register_alpha_pixels_to_event);
		}

		{
			ContentTable texture_section(ImGui::CalcTextSize("Texture").x);
			FillNameCol("Texture");

			ImVec2 size(50, 50);

			if (sc.texture)
				ImGui::ImageButton((ImTextureID)sc.texture->GetID(), size, ImVec2(0, 1), ImVec2(1, 0));
			else
				ImGui::Button("null", size);
		}

		{
			std::string name = "Tiling Factor";
			ContentTable tilingfactor_section(ImGui::CalcTextSize(name.c_str()).x);

			DrawFloatControl(name, sc.tiling_factor, 0, 0, 1.0f, 1.0f, { "TF" });
		}

		{
			ContentTable uv_section(ImGui::CalcTextSize("UV_0").x);

			glm::vec2 uv0 = sc.tex_coords[0];
			glm::vec2 uv1 = sc.tex_coords[2];

			Draw2FloatControl("UV_0", uv0, {0.0f, 0.0f}, {1.0f, 1.0f}, { 1.0f, 1.0f }, { 0.0f, 0.0f });
			Draw2FloatControl("UV_1", uv1, {0.0f, 0.0f}, {1.0f, 1.0f}, { 1.0f, 1.0f }, { 1.0f, 1.0f });

			if (uv0 != sc.tex_coords[0] || uv1 != sc.tex_coords[2])
			{
				sc.tex_coords[0] = uv0;
				sc.tex_coords[1] = { uv1.x, uv0.y };
				sc.tex_coords[2] = uv1;
				sc.tex_coords[3] = { uv0.x, uv1.y };
			}
		}

		ImGui::ColorPicker4("##picker4", &sc.color.x, ImGuiColorEditFlags_AlphaPreviewHalf);
	});

	DrawComponent<LineComponent>(this, "Line Component", true, [](LineComponent& ln, Entity entity)
		{
			{
				ContentTable thickness_section(ImGui::CalcTextSize("Thickness").x);
				DrawFloatControl("Thickness", ln.thickness, 0.1f, 10.0f, 0.05f, 1.0f, { "TH" });
			}

			ImGui::ColorPicker4("##picker4", &ln.color.x, ImGuiColorEditFlags_AlphaPreviewHalf);
		});

	ImGui::End();

	properties_panel_first = false;
}