#include "Engine.h"
#include "ScriptComponent.h"

#include "scripting/ScriptCache.h"
#include "scripting/ScriptEngine.h"


namespace Paper
{
	template<typename T>
	bool IsValueInitial(ScriptFieldStorage& storage)
	{
		ManagedField* managedField = storage.GetField();
		T currentValue = storage.GetValue<T>(true);
		T initialValue = managedField->initialFieldValue.Read<T>();
		return currentValue == initialValue;
	}

	bool IsFieldValueInitial(ScriptFieldType type, ScriptFieldStorage& storage)
	{
		switch (type)
		{
			case ScriptFieldType::Bool: return IsValueInitial<bool>(storage);
			case ScriptFieldType::Char: return IsValueInitial<char>(storage);
			case ScriptFieldType::UChar: return IsValueInitial<unsigned char>(storage);
			case ScriptFieldType::Int16: return IsValueInitial<int16_t>(storage);
			case ScriptFieldType::UInt16: return IsValueInitial<uint16_t>(storage);
			case ScriptFieldType::Int32: return IsValueInitial<int32_t>(storage);
			case ScriptFieldType::UInt32: return IsValueInitial<uint32_t>(storage);
			case ScriptFieldType::Int64: return IsValueInitial<int64_t>(storage);
			case ScriptFieldType::UInt64: return IsValueInitial<uint64_t>(storage);
			case ScriptFieldType::Float: return IsValueInitial<float>(storage);
			case ScriptFieldType::Double: return IsValueInitial<double>(storage);
			case ScriptFieldType::String: return IsValueInitial<std::string>(storage);
			case ScriptFieldType::Vec2: return IsValueInitial<glm::vec2>(storage);
			case ScriptFieldType::Vec3: return IsValueInitial<glm::vec3>(storage);
			case ScriptFieldType::Vec4: return IsValueInitial<glm::vec4>(storage);
			case ScriptFieldType::Entity:
			case ScriptFieldType::DataComponent:
			case ScriptFieldType::TransformComponent: 
			case ScriptFieldType::SpriteComponent: 
			case ScriptFieldType::LineComponent: 
			case ScriptFieldType::TextComponent: 
			case ScriptFieldType::CameraComponent: 
			case ScriptFieldType::ScriptComponent: 
			case ScriptFieldType::PaperID: return IsValueInitial<PaperID>(storage);
		default: return true;
		}
	}

	template <typename T>
	void SerializeFieldValue(YAML::Emitter& out, ScriptFieldStorage& storage)
	{
		ManagedField* managedField = storage.GetField();
		T currentValue = storage.GetValue<T>(true);
		T initialValue = managedField->initialFieldValue.Read<T>();
		if (currentValue != initialValue)
		{
			out << currentValue;
		}
	}

	void SerializePaperFieldValue(YAML::Emitter& out, ScriptFieldStorage& storage)
	{
		ManagedField* managedField = storage.GetField();
		PaperID currentValue = storage.GetValue<PaperID>(true).toString();
		PaperID initialValue = managedField->initialFieldValue.Read<PaperID>();
		if (currentValue != initialValue)
		{
			out << currentValue;
		}
	}

	bool ScriptComponent::Serialize(YAML::Emitter& out)
	{
		try
		{
#ifndef DISABLE_SCRIPT_ENGINE
			out << YAML::Key << "ScriptComponent";
			out << YAML::BeginMap; // SpriteComponent

			out << YAML::Key << "Name" << YAML::Value << ScriptEngine::GetEntityInheritClass(scriptClassName)->fullClassName;

			out << YAML::Key << "ScriptFields";
			out << YAML::BeginMap;

			out << YAML::Key << ScriptEngine::GetEntityInheritClass(scriptClassName)->fullClassName; // for legacy
			out << YAML::BeginMap;

			for (auto fieldID : ScriptEngine::GetEntityInheritClass(scriptClassName)->fieldIDs)
			{
				ManagedField* managedField = ScriptCache::GetManagedField(fieldID);

				ScriptFieldStorage fieldStorage(serializedEntity->GetPaperID(), managedField);

				ScriptFieldType type = managedField->fieldType;

				if (IsFieldValueInitial(type, fieldStorage)) continue;

				out << YAML::Key << managedField->fieldName;
				out << YAML::BeginMap;

				out << YAML::Key <<	 "FieldType" << YAML::Value << ScriptUtils::ScriptFieldTypeToString(type);
				out << YAML::Key << "Value" << YAML::Value;

				switch (type)
				{
					case ScriptFieldType::Bool:
						SerializeFieldValue<bool>(out, fieldStorage);
						break;
					case ScriptFieldType::Char:
						SerializeFieldValue<char>(out, fieldStorage);
						break;
					case ScriptFieldType::UChar:
						SerializeFieldValue<unsigned char>(out, fieldStorage);
						break;
					case ScriptFieldType::Int16:
						SerializeFieldValue<int16_t>(out, fieldStorage);
						break;
					case ScriptFieldType::UInt16:
						SerializeFieldValue<uint16_t>(out, fieldStorage);
						break;
					case ScriptFieldType::Int32:
						SerializeFieldValue<int32_t>(out, fieldStorage);
						break;
					case ScriptFieldType::UInt32:
						SerializeFieldValue<uint32_t>(out, fieldStorage);
						break;
					case ScriptFieldType::Int64:
						SerializeFieldValue<int64_t>(out, fieldStorage);
						break;
					case ScriptFieldType::UInt64:
						SerializeFieldValue<uint64_t>(out, fieldStorage);
						break;
					case ScriptFieldType::Float:
						SerializeFieldValue<float>(out, fieldStorage);
						break;
					case ScriptFieldType::Double:
						SerializeFieldValue<double>(out, fieldStorage);
						break;
					case ScriptFieldType::String:
						SerializeFieldValue<std::string>(out, fieldStorage);
						break;
					case ScriptFieldType::Vec2:
						SerializeFieldValue<glm::vec2>(out, fieldStorage);
						break;
					case ScriptFieldType::Vec3:
						SerializeFieldValue<glm::vec3>(out, fieldStorage);
						break;
					case ScriptFieldType::Vec4:
						SerializeFieldValue<glm::vec4>(out, fieldStorage);
						break;
					case ScriptFieldType::Entity:
						SerializePaperFieldValue(out, fieldStorage);
						break;
					default:
						out << 0;
						break;
				}

				out << YAML::EndMap;
			}

			out << YAML::EndMap;
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
	Buffer StoreValueInBuffer(YAML::Node& yamlScriptField, ScriptFieldType type)
	{
		T value = yamlScriptField["Value"].as<T>();
		uint32_t size = ScriptUtils::ScriptFieldTypeSize(type);

		Buffer buffer;
		buffer.Allocate(size);
		buffer.Nullify();
		buffer.Write(&value, size);
		return buffer;
	}

	Buffer StoreInBuffer(YAML::Node& yamlScriptField)
	{
		ScriptFieldType type = ScriptUtils::StringToScriptFieldType(yamlScriptField["FieldType"].as<std::string>());

		switch (type)
		{
			case ScriptFieldType::Bool:		return StoreValueInBuffer<bool>(yamlScriptField, type);
			case ScriptFieldType::Char:		return StoreValueInBuffer<char>(yamlScriptField, type);
			case ScriptFieldType::UChar:	return StoreValueInBuffer<unsigned char>(yamlScriptField, type);
			case ScriptFieldType::Int16:	return StoreValueInBuffer<int16_t>(yamlScriptField, type);
			case ScriptFieldType::UInt16:	return StoreValueInBuffer<uint16_t>(yamlScriptField, type);
			case ScriptFieldType::Int32:	return StoreValueInBuffer<int32_t>(yamlScriptField, type);
			case ScriptFieldType::UInt32:	return StoreValueInBuffer<uint32_t>(yamlScriptField, type);
			case ScriptFieldType::Int64:	return StoreValueInBuffer<int64_t>(yamlScriptField, type);
			case ScriptFieldType::UInt64:	return StoreValueInBuffer<uint64_t>(yamlScriptField, type);
			case ScriptFieldType::Float:	return StoreValueInBuffer<float>(yamlScriptField, type);
			case ScriptFieldType::Double:	return StoreValueInBuffer<double>(yamlScriptField, type);
			case ScriptFieldType::String:	return StoreValueInBuffer<std::string>(yamlScriptField, type);
			case ScriptFieldType::Vec2:		return StoreValueInBuffer<glm::vec2>(yamlScriptField, type);
			case ScriptFieldType::Vec3:		return StoreValueInBuffer<glm::vec3>(yamlScriptField, type);
			case ScriptFieldType::Vec4:		return StoreValueInBuffer<glm::vec4>(yamlScriptField, type);
			case ScriptFieldType::Entity:
			{
				uint64_t entityID = PaperID(yamlScriptField["Value"].as<std::string>()).toUInt64();
				uint32_t size = ScriptUtils::ScriptFieldTypeSize(type);

				Buffer buffer;
				buffer.Allocate(size);
				buffer.Nullify();
				buffer.Write(&entityID, size);
				return buffer;
			}
		}
		return Buffer();
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
			auto yamlScriptField = yamlScriptClassFields[scriptFieldName];

			ManagedField* managedField = ScriptClass(managedClass).GetManagedField(scriptFieldName);
			if (!managedField) continue; //field does not exist anymore

			ScriptFieldType type = ScriptUtils::StringToScriptFieldType(yamlScriptField["FieldType"].as<std::string>());
			if (managedField->fieldType != type) continue; //field exists but has another type

			Buffer valBuffer = StoreInBuffer(yamlScriptField);
			if (!valBuffer) continue; //value is invalid

			ScriptCache::GetFieldStorage(deserializedEntity.GetPaperID(), managedField) = valBuffer;
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
