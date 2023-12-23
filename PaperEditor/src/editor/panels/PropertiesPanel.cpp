#include "Editor.h"
#include "PropertiesPanel.h"

#include "imgui/UI.h"

#include "editor/SelectionManager.h"

#include "editor/PaperLayer.h"
#include "renderer/Font.h"
#include "scripting/ScriptEngine.h"

#include "scene/Components.h"


namespace PaperED
{

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

			SingleFloatControl(*(format.begin() + 0), val.x, min.x, max.x, speed.x, reset_value.x, content_avail_x, 2);
		ImGui::SameLine();
		SingleFloatControl(*(format.begin() + 1), val.y, min.y, max.y, speed.y, reset_value.y, content_avail_x, 2);
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
	void DrawComponentToAddPopup(const std::string& name, AddedComponentCallbackFn<ComponentType> on_added_component = nullptr)
	{
		Entity selectedEntity = SelectionManager::GetSelections(SelectionManagerType::ViewPort)[0].ToEntity();
		if (selectedEntity.HasComponent<ComponentType>()) return;

		if (ImGui::Selectable(name.c_str(), false))
		{
			ComponentType& component = selectedEntity.AddComponent<ComponentType>();
			if (on_added_component)
				on_added_component(selectedEntity, component);

			ImGui::CloseCurrentPopup();
		}
	}

	template<typename ComponentType>
	using DrawComponentFn = std::function<void(ComponentType&, Entity)>;

	template<typename ComponentType>
	bool DrawRemoveButton(bool treeOpen)
	{
		Entity selectedEntity = SelectionManager::GetSelections(SelectionManagerType::ViewPort)[0].ToEntity();
		ImGui::SameLine(ImGui::GetContentRegionAvail().x - 50);
		{
			UI::ScopedStyle padding(ImGuiStyleVar_ItemInnerSpacing, ImVec2(2.0f, 2.0f));
			if (ImGui::SmallButton("Remove"))
			{
				selectedEntity.RemoveComponent<ComponentType>();
				if (treeOpen) ImGui::TreePop();
				return true;
			}
			return false;
		}
	}

	template<typename ComponentType>
	void DrawComponent(const std::string& name, bool canRemove, DrawComponentFn<ComponentType> draw_components_fn)
	{
		Entity selectedEntity = SelectionManager::GetSelections(SelectionManagerType::ViewPort)[0].ToEntity();
		if (!selectedEntity.HasComponent<ComponentType>()) return;

		bool tree_open = UI::BeginImageTreeNode(name.c_str());

		if (canRemove && DrawRemoveButton<ComponentType>(tree_open)) return;

		if (tree_open)
		{
			draw_components_fn(selectedEntity.GetComponent<ComponentType>(), selectedEntity);
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

	/**
	 * \brief 
	 * \param isOpen 
	 */
	void PropertiesPanel::OnImGuiRender(bool& isOpen)
	{

		UI::ScopedStyle min_width(ImGuiStyleVar_WindowMinSize, ImVec2(400.0f, 0.0f));

		ImGui::Begin("Properties", &isOpen);
		if (!SelectionManager::HasSelection(SelectionManagerType::ViewPort))
		{
			ImGui::End();
			return;
		}
		Entity selectedEntity = SelectionManager::GetSelections(SelectionManagerType::ViewPort)[0].ToEntity();

		ImGui::Text("Name:");
		ImGui::SameLine();
		std::string name = selectedEntity.GetName();
		ImGui::InputText(("##" + selectedEntity.GetPaperID().toString()).c_str(), &name);
		selectedEntity.SetName(name);

		ImGui::Text(("UUID: " + selectedEntity.GetPaperID().toString()).c_str());

		if (ImGui::BeginPopup("components_add_popup"))
		{
			DrawComponentToAddPopup<CameraComponent>("Camera Component");
			DrawComponentToAddPopup<SpriteComponent>("Sprite Component");
			DrawComponentToAddPopup<LineComponent>("Line Component");
			DrawComponentToAddPopup<TextComponent>("Text Component");
			DrawComponentToAddPopup<Rigidbody2DComponent>("Rigidbody2D Component");
			DrawComponentToAddPopup<Collider2DComponent>("Collider2D Component");
			DrawComponentToAddPopup<ScriptComponent>("Script Component");
			ImGui::EndPopup();
		}

		DrawComponent<DataComponent>("Tag", false, [](DataComponent& dc, Entity entity)
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


		DrawComponent<TransformComponent>("Transform", false,
			[](TransformComponent& tc, Entity entity)
			{
				UI::BeginPropertyGrid();

				UI::Property("Position", tc.position);
				UI::Property("Rotation", tc.rotation);
				UI::Property("Scale", tc.scale, glm::vec3(1.0f, 1.0f, 1.0f));

				UI::EndPropertyGrid();
			});

		DrawComponent<CameraComponent>("Camera", true,
			[](CameraComponent& cc, Entity entity)
			{
				UI::BeginPropertyGrid();

				EntityCamera& camera = cc.camera;

				ProjectionMode mode = camera.GetProjectionMode();
				UI::PropertyDropdown("Projection", { "Perspective", "Orthographic" }, mode);
				camera.SetProjectionMode(mode);

				if (camera.GetProjectionMode() == ProjectionMode::Perspective)
				{
					float fov = camera.GetPerspectiveFOV();
					UI::Property("FOV", fov, 45.0f, {1.0f, 0.001f, 360.0f });
					camera.SetPerspectiveFOV(fov);

					float nearPlane = camera.GetPerspectiveNearClip();
					UI::Property("Near Plane", nearPlane, 0.01f, {1.0f, 0.01f, FLT_MAX });
					camera.SetPerspectiveNearClip(nearPlane);

					float farPlane = camera.GetPerspectiveFarClip();
					UI::Property("Far Plane", farPlane, 1000.0f, {1.0f, 0.01f, FLT_MAX });
					camera.SetPerspectiveFarClip(farPlane);
				}
				else
				{
					float size = camera.GetOrthographicSize();
					UI::Property("Size", size, 10.0f);
					camera.SetOrthographicSize(size);

					float nearPlane = camera.GetOrthographicNearClip();
					UI::Property("Near Plane", nearPlane, -1.0f);
					camera.SetOrthographicNearClip(nearPlane);

					float farPlane = camera.GetOrthographicFarClip();
					UI::Property("Far Plane", farPlane, 1000.0f);
					camera.SetOrthographicFarClip(farPlane);
				}

				UI::Property("FixedAspectRatio", cc.fixedAspectRatio);
				UI::Property("Primary", cc.primary);

				UI::EndPropertyGrid();

			});

		DrawComponent<SpriteComponent>("Sprite", true,
			[](SpriteComponent& sc, Entity entity)
			{
				UI::BeginPropertyGrid();

				UI::PropertyDropdown("Geometry", { "None", "Triangle", "Rectangle", "Circle" }, sc.geometry);

				if (sc.geometry == Geometry::CIRCLE)
				{
					UI::Property("Thickness", sc.thickness, 1.0f, { 0.01f, 0.0f, 1.0f, "%.2f" });
					UI::Property("Fade", sc.fade, 0.005f, {0.001f, 0.0f, FLT_MAX, "%.3f"});
				}

				UI::Property("RegisterAlphaPixels", sc.register_alpha_pixels_to_event);


				UI::PropertyTexture("Texture", sc.texture, 100);
				UI::DragDropTarget(sc.texture);

				UI::Property("Tiling Factor", sc.tiling_factor, 1.0f, { .speed = 0.1f, .format = "%.1f" });

				glm::vec2 uv0 = sc.tex_coords[0];
				glm::vec2 uv1 = sc.tex_coords[2];

				UI::Property("UV0", uv0, { 0, 0 }, { glm::vec2(0.01f), glm::vec2(0), glm::vec2(1.0f), "%.2f"});
				UI::Property("UV1", uv1, { 1.0f, 1.0f }, { glm::vec2(0.01f), glm::vec2(0), glm::vec2(1.0f), "%.2f"});

				if (uv0 != sc.tex_coords[0] || uv1 != sc.tex_coords[2])
				{
					sc.tex_coords[0] = uv0;
					sc.tex_coords[1] = { uv1.x, uv0.y };
					sc.tex_coords[2] = uv1;
					sc.tex_coords[3] = { uv0.x, uv1.y };
				}

				UI::PropertyColor("Color", sc.color);

				UI::EndPropertyGrid();

			});

		DrawComponent<LineComponent>("Line", true, [](LineComponent& ln, Entity entity)
			{
				UI::BeginPropertyGrid();

				UI::Property("Thickness", ln.thickness, 1.0f, {0.05f, 0.1f, 10.0f}); //speed 0.05f

				UI::PropertyColor("Color", ln.color);

				UI::EndPropertyGrid();
			});

		DrawComponent<TextComponent>("Text", true, [](TextComponent& texc, Entity entity)
		{
			UI::BeginPropertyGrid();

			UI::PropertyMultiline("Text", texc.text, 100);

			UI::Property("RegisterAlphaPixel", texc.register_alpha_pixels_to_event);

			UI::PropertyFont("Font", texc.font);
			UI::DragDropTarget(texc.font);

			UI::PropertyColor("Color", texc.color);

			UI::EndPropertyGrid();

		});

		DrawComponent<Rigidbody2DComponent>("Rigidbody2D", true, [](Rigidbody2DComponent& rb2dc, Entity entity)
		{
				UI::BeginPropertyGrid();

				UI::PropertyDropdown("BodyType", { "Static", "Kinematic", "Dynamic" }, rb2dc.type);

				UI::Property("FixedRotation", rb2dc.fixedRotation);

				UI::EndPropertyGrid();

		});

		DrawComponent<Collider2DComponent>("Collider2D", true, [](Collider2DComponent& c2dc, Entity entity)
		{
				UI::BeginPropertyGrid();

				UI::PropertyDropdown("Geometry", { "NONE", "TRIANGLE", "RECTANGLE", "CIRCLE" }, c2dc.geometry);

				UI::Property("Offset", c2dc.offset);

				if (c2dc.geometry == Geometry::CIRCLE)
					UI::Property("Radius", c2dc.radius);
				else
					UI::Property("Size", c2dc.size);

				UI::Property("Density", c2dc.density);

				UI::Property("Friction", c2dc.friction);

				UI::Property("Bounciness", c2dc.bounciness);

				UI::Property("BouncinessThreshold", c2dc.bouncinessThreshold);

				UI::EndPropertyGrid();

		});

		DrawComponent<ScriptComponent>("Script", true, [this](ScriptComponent& scrc, Entity entity)
		{
				UI::BeginPropertyGrid();
#ifndef DISABLE_SCRIPT_ENGINE
			ManagedClass* managedClass = ScriptEngine::GetEntityInheritClass(scrc.scriptClassName);
			{

				

				if (UI::PropertyButton("C#-Class", managedClass ? managedClass->fullClassName : ""))
				{
					ImGui::OpenPopup("select_entity_scriptclass");
					
				}

				if (ImGui::BeginPopup("select_entity_scriptclass"))
				{
					const auto& classList = ScriptEngine::GetEntityInheritClasses();
					for (const auto lmanagedClass : classList)
					{
						if (UI::Selectable(lmanagedClass->fullClassName, managedClass == lmanagedClass) && managedClass != lmanagedClass)
						{
							scrc.scriptClassName = lmanagedClass->fullClassName;
							ScriptEngine::CreateScriptEntity(entity);
						}
					}
					ImGui::EndPopup();
				}
			}
			{
				//Fields
				EntityInstance* entityInstance = ScriptEngine::GetEntityScriptInstance(entity.GetPaperID());
				if (managedClass)
				{
					const auto& managedFields = ScriptClass(ScriptEngine::GetEntityInheritClass(scrc.scriptClassName)).GetManagedFields();
					for (auto* managedField : managedFields)
					{
						ScriptFieldStorage fieldStorage = ScriptFieldStorage(entity.GetPaperID(), managedField);

						if (!managedField->HasFlag(ScriptFieldFlag::Public)) continue;
						if (managedField->fieldType == ScriptFieldType::Invalid) continue;

						std::string varName = std::format("{} {}", ScriptUtils::ScriptFieldTypeToString(managedField->fieldType), managedField->fieldName);

						switch (managedField->fieldType)
						{
							case ScriptFieldType::String:
							{
								std::string dataString = fieldStorage.GetValue<std::string>();
								if (UI::Property(varName, dataString, fieldStorage.GetField()->initialFieldValue.Read<std::string>()) && managedField->IsWritable())
								{
									fieldStorage.SetValue(dataString);
								}
								break;
							}

							case ScriptFieldType::Float:
							{
								float dataFloat = fieldStorage.GetValue<float>();
								if (UI::Property(varName, dataFloat, fieldStorage.GetField()->initialFieldValue.Read<float>()))
								{
									fieldStorage.SetValue(dataFloat);
								}
								break;
							}

							case ScriptFieldType::Char:
							{
								char dataChar = fieldStorage.GetValue<char>();
								char data[] = { dataChar, 0 };
								FillNameCol(varName);
								if (UI::PropertyInputRaw(varName, data, 2) && managedField->IsWritable())
								{
									fieldStorage.SetValue(data[0]);
								};
								break;
							}

							case ScriptFieldType::Entity:
							{
								PaperID entityID = fieldStorage.GetValue<uint64_t>();
								
								UI::PropertyPaperID(varName, entityID);
								if (entityID)
									UI::PropertyButton(varName, Scene::GetActive()->GetEntity(entityID).GetName().c_str());
								else
									UI::PropertyButton(varName, "null");

								PaperID paperID;
								if (UI::DragDropTarget(paperID))
								{
									fieldStorage.SetValue(paperID.toUInt64());
									paperLayer->drag_entity = Entity();
								}

								//ImGui::SameLine();
								//if (ImGui::Button("X", GetButtonSize()))
								//{
								//	fieldStorage.SetValue((uint64_t)0);
								//};

								break;
							}
						}
					}
				}
			}

#endif
			UI::EndPropertyGrid();
		});

		{
			UI::ScopedStyle padding(ImGuiStyleVar_ItemInnerSpacing, ImVec2(2.0f, 2.0f));
			if (ImGui::Button("Add"))
			{
				ImGui::OpenPopup("components_add_popup");
			}
		}

		ImGui::End();
	}
}

