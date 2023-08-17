#include "Editor.h"
#include "PaperLayer.h"


#include "WindowsOpen.h"

#include <imgui/misc/cpp/imgui_stdlib.h>

#include "renderer/Font.h"
#include "scripting/ScriptEngine.h"

static ImVec2 GetButtonSize()
{
	float padding = 3.0f;
	float line_height = GImGui->Font->FontSize + padding * 2;
	return { line_height + 3.0f, line_height };
}

static void FillNameCol(const std::string& name)
{
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::AlignTextToFramePadding();
	ImGui::TextUnformatted(name.c_str());
	ImGui::TableSetColumnIndex(1);
}

static bool SingleFloatControl(const std::string& label, float& value, float min, float max, float speed, float resetValue, float content_avail_x, int count)
{
	ImVec2 button_size = GetButtonSize();
	float input_item_width = (content_avail_x - 5.0f) / (float)count - button_size.x;

	UI::ScopedID id(ImGui::TableGetRowIndex());

	if (ImGui::Button(label.c_str(), button_size))
	{
		value = resetValue;
	};
	ImGui::SameLine();
	ImGui::SetNextItemWidth(input_item_width);
	return ImGui::DragFloat(("##" + label).c_str(), &value, 0.1f, min, max, "%.2f", 0);
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

static bool DrawFloatControl(const std::string& name, float& val, float min = 0.0f, float max = 0.0f, float speed = 1.0f, float reset_value = 0.0f, std::initializer_list<std::string> format = { "X" })
{
	FillNameCol(name);

	UI::ScopedStyle item_spacing(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });
	UI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 2.0f });

	float content_avail_x = ImGui::GetContentRegionAvail().x;

	ASSERT(format.size() == 1, "")

	return SingleFloatControl(*(format.begin() + 0), val, min, max, speed, reset_value, content_avail_x, 1);
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
void DrawComponentToAddPopup(PaperLayer* _instance, const std::string& name, AddedComponentCallbackFn<ComponentType> on_added_component = nullptr)
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
bool DrawRemoveButton(PaperLayer* _instance, bool treeOpen)
{
	ImGui::SameLine(ImGui::GetContentRegionAvail().x - 50);
	{
		UI::ScopedStyle padding(ImGuiStyleVar_ItemInnerSpacing, ImVec2(2.0f, 2.0f));
		if (ImGui::SmallButton("Remove"))
		{
			_instance->active_entity.RemoveComponent<ComponentType>();
			if (treeOpen) ImGui::TreePop();
			return true;
		}
		return false;
	}
}

template<typename ComponentType>
void DrawComponent(PaperLayer* _instance, const std::string& name, bool canRemove, DrawComponentFn<ComponentType> draw_components_fn)
{
	if (!_instance->active_entity.HasComponent<ComponentType>()) return;

	bool tree_open = ImGui::TreeNode(name.c_str());

	if (canRemove && DrawRemoveButton<ComponentType>(_instance, tree_open)) return;

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


void PaperLayer::PropertiesPanel()
{
	if (!active_entity) return;
	if (properties_panel_first)
		DockPanel("Properties", dock_id_right_bottom);

	UI::ScopedStyle min_width(ImGuiStyleVar_WindowMinSize, ImVec2(400.0f, 0.0f));

	ImGui::Begin("Properties", &show_property_panel);
	ImGui::Text("Name:");
	ImGui::SameLine();
	std::string name = active_entity.GetName();
	ImGui::InputText(("##" + active_entity.GetEntityID().toString()).c_str(), &name);
	active_entity.SetName(name);

	ImGui::Text(("UUID: " + active_entity.GetEntityID().toString()).c_str());

	if (ImGui::BeginPopup("components_add_popup"))
	{
		DrawComponentToAddPopup<CameraComponent>(this, "Camera Component");
		DrawComponentToAddPopup<SpriteComponent>(this, "Sprite Component");
		DrawComponentToAddPopup<LineComponent>(this, "Line Component");
		DrawComponentToAddPopup<TextComponent>(this, "Text Component");
		DrawComponentToAddPopup<ScriptComponent>(this, "Script Component");
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
			if (dc.tags.empty())
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
		Draw3FloatControl("Scale", tc.scale, glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec3(1));

	});

	DrawComponent<CameraComponent>(this, "Camera Component", true,
		[](CameraComponent& cc, Entity entity)
	{
		EntityCamera& camera = cc.camera;
		{
			ContentTable projection_section(ImGui::CalcTextSize("Projection").x);
			FillNameCol("Projection");

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImGui::BeginCombo(CONST_UI_ID, ProjectionModeToString(camera.GetProjectionMode()).c_str()))
			{
				for (int i = 0; i <= (int)ProjectionMode::_LAST; i++)
				{
					if (ImGui::Selectable(ProjectionModeToString((ProjectionMode)i).c_str(), camera.GetProjectionMode() == (ProjectionMode)i))
					{
						camera.SetProjectionMode((ProjectionMode)i);
					}
				}
				ImGui::EndCombo();
			}
		}

		if (camera.GetProjectionMode() == ProjectionMode::Perspective)
		{
			ContentTable projection_section(ImGui::CalcTextSize("Near Plane").x);

			float fov = camera.GetPerspectiveFOV();
			DrawFloatControl("FOV", fov, 0.001f, 360.0f, 1.0f, 45.0f, { "FV" });
			if (fov != camera.GetPerspectiveFOV()) camera.SetPerspectiveFOV(fov);

			float nearPlane = camera.GetPerspectiveNearClip();
			DrawFloatControl("Near Plane", nearPlane, 0.01f, 0, 1.0f, 0.01, { "NP" });
			if (nearPlane != camera.GetPerspectiveNearClip()) camera.SetPerspectiveNearClip(nearPlane);

			float farPlane = camera.GetPerspectiveFarClip();
			DrawFloatControl("Far Plane", farPlane, 0.02f, 0, 1.0f, 1000.0f, { "FP" });
			if (farPlane != camera.GetPerspectiveFarClip()) camera.SetPerspectiveFarClip(farPlane);
		}
		else
		{
			ContentTable orthographic_section(ImGui::CalcTextSize("Near Plane").x);

			float size = camera.GetOrthographicSize();
			DrawFloatControl("Size", size, 0, 0, 1.0f, 10.0f, { "SI" });
			if (size != camera.GetPerspectiveFOV()) camera.SetPerspectiveFOV(size);

			float nearPlane = camera.GetOrthographicNearClip();
			DrawFloatControl("Near Plane", nearPlane, 0, 0, 1.0f, -1.0f, { "NP" });
			if (nearPlane != camera.GetOrthographicNearClip()) camera.SetOrthographicNearClip(nearPlane);

			float farPlane = camera.GetOrthographicFarClip();
			DrawFloatControl("Far Plane", farPlane, 0, 0, 1.0f, 1000.0f, { "FP" });
			if (farPlane != camera.GetOrthographicFarClip()) camera.SetOrthographicFarClip(farPlane);
		}

		{
			ContentTable component_section(ImGui::CalcTextSize("FixedAspectRatio").x);
			DrawCheckbox("FixedAspectRatio", cc.fixedAspectRatio);
			DrawCheckbox("Primary", cc.primary);
		}

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

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM_TEXTURE"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::string file = std::filesystem::path(path).string();
					
					sc.texture = DataPool::GetAssetTexture(file, true);
				}
				ImGui::EndDragDropTarget();
			}
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

		{
			ContentTable color_section(ImGui::CalcTextSize("Color").x);

			FillNameCol("Color");

			ImGui::ColorPicker4("##picker4", &sc.color.x, ImGuiColorEditFlags_AlphaPreviewHalf);
		}
		
	});

	DrawComponent<LineComponent>(this, "Line Component", true, [](LineComponent& ln, Entity entity)
		{
			{
				ContentTable thickness_section(ImGui::CalcTextSize("Thickness").x);
				DrawFloatControl("Thickness", ln.thickness, 0.1f, 10.0f, 0.05f, 1.0f, { "TH" });
			}

			{
				ContentTable color_section(ImGui::CalcTextSize("Color").x);

				FillNameCol("Color");

				ImGui::ColorPicker4("##picker4", &ln.color.x, ImGuiColorEditFlags_AlphaPreviewHalf);
			}
		});

	DrawComponent<TextComponent>(this, "Text Component", true, [](TextComponent& texc, Entity entity)
		{
			{
				ContentTable text_section(ImGui::CalcTextSize("Text").x);
				FillNameCol("Text");

				ImGui::InputText(CONST_UI_ID, &texc.text);
			}

			{
				std::string name = "RegisterAlphaPixels[temp]";
				ContentTable pixelregister_section(ImGui::CalcTextSize(name.c_str()).x);
				DrawCheckbox(name, texc.register_alpha_pixels_to_event);
			}

			{
				ContentTable font_section(ImGui::CalcTextSize("Font").x);
				FillNameCol("Font");

				
				ImGui::Button(texc.font->GetFontName().c_str());

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM_FONT"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::string file = std::filesystem::path(path).string();

						texc.font = DataPool::GetFont(file, true);
					}
					ImGui::EndDragDropTarget();
				}
			}

			{
				ContentTable color_section(ImGui::CalcTextSize("Color").x);

				FillNameCol("Color");

				ImGui::ColorPicker4("##picker4", &texc.color.x, ImGuiColorEditFlags_AlphaPreviewHalf);
			}

		
		});

	DrawComponent<ScriptComponent>(this, "Script Component", true, [this](ScriptComponent& scrc, Entity entity)
		{
			Shr<ScriptClass> scriptClass = ScriptEngine::GetEntityClass(scrc.scriptClassName);
			{
				ContentTable scriptClassSection(ImGui::CalcTextSize("C#-Class").x);
				FillNameCol("C#-Class");


				if (ImGui::Button(((scriptClass ? scriptClass->GetFullClassName() : "") + CONST_UI_ID).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
				{
					ImGui::OpenPopup("select_entity_scriptclass");
				}

				if (ImGui::BeginPopup("select_entity_scriptclass"))
				{
					const auto& classList = ScriptEngine::GetEntityClasses();
					for (const auto [name, lscriptClass] : classList)
					{
						if (ImGui::Selectable(name.c_str(), scriptClass == lscriptClass) && scriptClass != lscriptClass)
						{
							scrc.scriptClassName = lscriptClass->GetFullClassName();
							ScriptEngine::CreateScriptEntity(entity);
						}
					}
					ImGui::EndPopup();
				}
			}
			{
				
				//Fields
				Shr<EntityInstance> entityInstance = ScriptEngine::GetEntityScriptInstance(entity.GetEntityID());
				if (scriptClass)
				{
					const auto& storageFields = ScriptEngine::GetActiveEntityFieldStorage(entity);
					for (const auto& fieldStorage : storageFields)
					{
						const auto& field = fieldStorage->GetField();
						if (!field.HasFlag(ScriptFieldFlag::Public)) continue;
						if (field.type == ScriptFieldType::Invalid) continue;
					
						std::string varName = std::format("{} {}", ScriptUtils::ScriptFieldTypeToString(field.type), field.name);
						ContentTable fieldSection(ImGui::CalcTextSize(varName.c_str()).x);
					
						switch (field.type)
						{
							case ScriptFieldType::String:
							{
								std::string dataString = fieldStorage->GetValue<std::string>();
								FillNameCol(varName);
								if (ImGui::InputText(CONST_UI_ID, &dataString) && field.IsWritable())
								{
									fieldStorage->SetValue(dataString);
								};
								break;
							}

							case ScriptFieldType::Float:
							{
								float dataFloat = fieldStorage->GetValue<float>();
								if (DrawFloatControl(varName, dataFloat) && field.IsWritable())
								{
									fieldStorage->SetValue(dataFloat);
								};
								break;
							}

							case ScriptFieldType::Char:
							{
								char dataChar = fieldStorage->GetValue<char>();
								char data[] = { dataChar, 0 };
								FillNameCol(varName);
								if (ImGui::InputText(CONST_UI_ID, data, 2) && field.IsWritable())
								{
									fieldStorage->SetValue(data[0]);
								};
								break;
							}

							case ScriptFieldType::Entity:
							{
								EntityID entityID = fieldStorage->GetValue<uint64_t>();
								FillNameCol(varName);

								if (entityID)
									ImGui::Button(activeScene->GetEntity(entityID).GetName().c_str());
								else
									ImGui::Button("null");

								if (ImGui::BeginDragDropTarget())
								{
									if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_DRAG"))
									{
										EntityID payloadEntityID = *(uint64_t*)payload->Data;
										fieldStorage->SetValue(payloadEntityID.toUInt64());

										drag_entity = Entity();
									}
									ImGui::EndDragDropTarget();
								}

								ImGui::SameLine();
								if (ImGui::Button("X", GetButtonSize()))
								{
									fieldStorage->SetValue((uint64_t)0);
								};

								break;
							}
						}
					}
				}
			}
		});

	{
		UI::ScopedStyle padding(ImGuiStyleVar_ItemInnerSpacing, ImVec2(2.0f, 2.0f));
		if (ImGui::Button("Add"))
		{
			ImGui::OpenPopup("components_add_popup");
		}
	}

	ImGui::End();

	properties_panel_first = false;
}
