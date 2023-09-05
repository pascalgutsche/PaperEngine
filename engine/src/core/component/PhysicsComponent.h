#pragma once
#include "Serializable.h"
#include "SpriteComponent.h"


namespace Paper
{
	

	struct Rigidbody2DComponent : public Serializable
	{
		enum class BodyType { Static, Kinematic, Dynamic, _SIZE };
		BodyType type = BodyType::Static;
		bool fixedRotation = false;

		bool Serialize(YAML::Emitter& out) override;
		bool Deserialize(YAML::Node& data) override;
	};

	struct Collider2DComponent : public Serializable
	{
		Geometry geometry = Geometry::NONE;
		glm::vec2 offset = { 0.0f, 0.0f };
		glm::vec2 size = { 0.5f, 0.5f };
		float radius = 0.5f;

		float density = 1.0f;
		float friction = 0.5f;
		float bounciness = 0.0f;
		float bouncinessThreshold = 0.5f;


		bool Serialize(YAML::Emitter& out) override;
		bool Deserialize(YAML::Node& data) override;
	};

	static std::string BodyTypeToString(Rigidbody2DComponent::BodyType type)
	{
		switch (type)
		{
			case Rigidbody2DComponent::BodyType::Static: return "Static";
			case Rigidbody2DComponent::BodyType::Kinematic: return "Kinematic";
			case Rigidbody2DComponent::BodyType::Dynamic: return "Dynamic";
		}

		CORE_ASSERT("", false);
		return "";
	}

	static Rigidbody2DComponent::BodyType StringToBodyType(std::string type)
	{
		if (type == "Static") return Rigidbody2DComponent::BodyType::Static;
		if (type == "Kinematic") return Rigidbody2DComponent::BodyType::Kinematic;
		if (type == "Dynamic") return Rigidbody2DComponent::BodyType::Dynamic;

		CORE_ASSERT("", false);
		return Rigidbody2DComponent::BodyType::Static;
	}
}

