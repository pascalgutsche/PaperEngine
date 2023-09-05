#pragma once
#include "Engine.h"

#include "serializer/YAMLSerializer.h"


namespace Paper {

    struct Serializable {
	    virtual ~Serializable() = default;

	    virtual bool Serialize(YAML::Emitter& out) = 0;
		virtual bool Deserialize(YAML::Node& data) = 0;

    protected:
		Entity* serializedEntity = nullptr;

		template <typename... Component>
		friend static void SerializeEntity(Entity entity, YAML::Emitter& out);

		template <typename... Component>
		friend static void DeserializeEntity(Entity entity, YAML::Node& data);
    };


}
