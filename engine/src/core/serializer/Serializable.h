#pragma once
#include "Engine.h"
#include "utility.h"


namespace ppr {

    struct Serializable {
	    virtual ~Serializable() = default;

	    virtual bool Serialize() = 0;
		virtual bool Deserialize() = 0;
		
    };


}