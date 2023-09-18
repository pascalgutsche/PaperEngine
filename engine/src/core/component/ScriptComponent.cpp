#include "Engine.h"
#include "ScriptComponent.h"

#include "scripting/ScriptEngine.h"


namespace Paper
{
	bool ScriptComponent::Serialize(YAML::Emitter& out)
	{
		try
		{
#ifndef DISABLE_SCRIPT_ENGINE
			out << YAML::Key << "ScriptComponent";
			out << YAML::BeginMap; // SpriteComponent

			out << YAML::Key << "Name" << YAML::Value << ScriptEngine::GetEntityInheritClass(scriptClassName)->fullClassName;

			const auto& entityFieldStorage = ScriptEngine::GetEntityFieldStorage(*serializedEntity);

			out << YAML::Key << "ScriptFields";
			out << YAML::BeginMap;

			for (const auto& [classID, fieldStorage] : entityFieldStorage)
			{
				out << YAML::Key << ScriptClass(classID).GetFullClassName();
				out << YAML::BeginMap;

				for (const auto& classFieldStorage : fieldStorage)
				{
					ScriptFieldType type = classFieldStorage->GetField()->fieldType;

					out << YAML::Key << classFieldStorage->GetField()->fieldName;
					out << YAML::BeginMap;

					out << YAML::Key << "FieldType" << YAML::Value << (int)type;
					out << YAML::Key << "Value" << YAML::Value;

					switch (type)
					{
						case ScriptFieldType::Bool:
							out << classFieldStorage->GetValue<bool>(true);
							break;
						case ScriptFieldType::Char:
							out << classFieldStorage->GetValue<char>(true);
							break;
						case ScriptFieldType::UChar:
							out << classFieldStorage->GetValue<unsigned char>(true);
							break;
						case ScriptFieldType::Int16:
							out << classFieldStorage->GetValue<int16_t>(true);
							break;
						case ScriptFieldType::UInt16:
							out << classFieldStorage->GetValue<uint16_t>(true);
							break;
						case ScriptFieldType::Int32:
							out << classFieldStorage->GetValue<int32_t>(true);
							break;
						case ScriptFieldType::UInt32:
							out << classFieldStorage->GetValue<uint32_t>(true);
							break;
						case ScriptFieldType::Int64:
							out << classFieldStorage->GetValue<int64_t>(true);
							break;
						case ScriptFieldType::UInt64:
							out << classFieldStorage->GetValue<uint64_t>(true);
							break;
						case ScriptFieldType::Float:
							out << classFieldStorage->GetValue<float>(true);
							break;
						case ScriptFieldType::Double:
							out << classFieldStorage->GetValue<double>(true);
							break;
						case ScriptFieldType::String:
							out << classFieldStorage->GetValue<std::string>(true);
							break;
						case ScriptFieldType::Vec2:
							out << classFieldStorage->GetValue<glm::vec2>(true);
							break;
						case ScriptFieldType::Vec3:
							out << classFieldStorage->GetValue<glm::vec3>(true);
							break;
						case ScriptFieldType::Vec4:
							out << classFieldStorage->GetValue<glm::vec4>(true);
							break;
						case ScriptFieldType::Entity:
							out << classFieldStorage->GetValue<PaperID>(true).toString();
							break;
						default:
							out << 0;
							break;
					}

					out << YAML::EndMap;

				}
				out << YAML::EndMap;
			}

			out << YAML::EndMap;
			out << YAML::EndMap; // SpriteComponent
#endif

			return true;
		}
		catch (YAML::EmitterException& e)
		{
			LOG_CORE_CRITICAL("[ScriptComponent]: Could not serialize component\n\t" + e.msg);
			return false;
		}
	}

	template <typename T>
	void SetFieldStorage(YAML::Node& yamlScriptField, Shr<ScriptFieldStorage>& storage)
	{
		T value = yamlScriptField["Value"].as<T>();
		storage->SetValue(value, true);
	}

	bool DeserializeClassScriptValues(const YAML::const_iterator& yamlScriptClass, const YAML::Node& yamlScriptFields, Entity deserializedEntity)
	{
		std::string scriptClassName = yamlScriptClass->first.as<std::string>();

		ManagedClass* managedClass = ScriptEngine::GetEntityInheritClass(scriptClassName);
		if (!managedClass) return false;

		auto yamlScriptClassFields = yamlScriptFields[scriptClassName];
		for (YAML::const_iterator yamlScriptClassFieldsIT = yamlScriptClassFields.begin(); yamlScriptClassFieldsIT != yamlScriptClassFields.end(); ++yamlScriptClassFieldsIT)
		{
			std::string scriptFieldName = yamlScriptClassFieldsIT->first.as<std::string>();
			ManagedField* managedField = ScriptClass(managedClass).GetManagedField(scriptFieldName);
			if (!managedField) continue;

			auto yamlScriptField = yamlScriptClassFields[scriptFieldName];
			auto& entityFieldStorage = ScriptEngine::GetEntityFieldStorage(deserializedEntity);

			auto& scriptClassFieldStorages = entityFieldStorage[managedClass->classID];

			int index = 0;
			for (; index < scriptClassFieldStorages.size(); index++)
				if (scriptClassFieldStorages[index]->GetField() == managedField) break;

			Shr<ScriptFieldStorage> fieldStorage = MakeShr<ScriptFieldStorage>(managedField);

			ScriptFieldType type = (ScriptFieldType)yamlScriptField["FieldType"].as<int>();

			switch (type)
			{
				case ScriptFieldType::Bool:
					SetFieldStorage<bool>(yamlScriptField, fieldStorage);
					break;
				case ScriptFieldType::Char:
					SetFieldStorage<char>(yamlScriptField, fieldStorage);
					break;
				case ScriptFieldType::UChar:
					SetFieldStorage<unsigned char>(yamlScriptField, fieldStorage);
					break;
				case ScriptFieldType::Int16:
					SetFieldStorage<int16_t>(yamlScriptField, fieldStorage);
					break;
				case ScriptFieldType::UInt16:
					SetFieldStorage<uint16_t>(yamlScriptField, fieldStorage);
					break;
				case ScriptFieldType::Int32:
					SetFieldStorage<int32_t>(yamlScriptField, fieldStorage);
					break;
				case ScriptFieldType::UInt32:
					SetFieldStorage<uint32_t>(yamlScriptField, fieldStorage);
					break;
				case ScriptFieldType::Int64:
					SetFieldStorage<int64_t>(yamlScriptField, fieldStorage);
					break;
				case ScriptFieldType::UInt64:
					SetFieldStorage<uint64_t>(yamlScriptField, fieldStorage);
					break;
				case ScriptFieldType::Float:
					SetFieldStorage<float>(yamlScriptField, fieldStorage);
					break;
				case ScriptFieldType::Double:
					SetFieldStorage<double>(yamlScriptField, fieldStorage);
					break;
				case ScriptFieldType::String:
					SetFieldStorage<std::string>(yamlScriptField, fieldStorage);
					break;
				case ScriptFieldType::Vec2:
					SetFieldStorage<glm::vec2>(yamlScriptField, fieldStorage);
					break;
				case ScriptFieldType::Vec3:
					SetFieldStorage<glm::vec3>(yamlScriptField, fieldStorage);
					break;
				case ScriptFieldType::Vec4:
					SetFieldStorage<glm::vec4>(yamlScriptField, fieldStorage);
					break;
				case ScriptFieldType::Entity:
				{
					PaperID entityID = yamlScriptField["Value"].as<std::string>();
					fieldStorage->SetValue(entityID.toUInt64(), true);
					break;
				}
			}

			scriptClassFieldStorages.emplace(scriptClassFieldStorages.begin() + index, fieldStorage);
		}
		return true;
	}

	bool ScriptComponent::Deserialize(YAML::Node& data)
	{
		try
		{
			scriptClassName = data["Name"].as<std::string>();
#ifndef DISABLE_SCRIPT_ENGINE
			auto yamlScriptFields = data["ScriptFields"];

			for (YAML::const_iterator yamlScriptClass = yamlScriptFields.begin(); yamlScriptClass != yamlScriptFields.end(); ++yamlScriptClass) {
				DeserializeClassScriptValues(yamlScriptClass, yamlScriptFields, *serializedEntity);
			}

			ScriptEngine::CreateScriptEntity(*serializedEntity);
#endif
		}
		catch (YAML::EmitterException& e)
		{
			LOG_CORE_CRITICAL("[ScriptComponent]: Could not deserialize component\n\t" + e.msg);
			return false;
		}
		return true;
	}
}
