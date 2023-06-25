#pragma once
#include "Engine.h"
#include "utility.h"

#include "YAMLSerializer.h"


namespace ppr {

    struct Serializable {
	    virtual ~Serializable() = default;

	    virtual bool Serialize(YAML::Emitter& out) = 0;
		virtual bool Deserialize(YAML::Node& data) = 0;
    };


}
