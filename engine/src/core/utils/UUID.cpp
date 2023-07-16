#include "Engine.h"
#include "UUID.h"

#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/lexical_cast.hpp>


namespace Paper
{
    UUID::UUID()
	{
        boost::uuids::random_generator generator;
        uuid = generator();
    }

    UUID::UUID(const std::string& uuidStr) {
        uuid = boost::lexical_cast<boost::uuids::uuid>(uuidStr);
    }

    std::string UUID::toString() const {
        return boost::uuids::to_string(uuid);
    }

    void UUID::Set(const std::string& id) const
    {
        uuid = boost::lexical_cast<boost::uuids::uuid>(id);
    }

    bool UUID::operator==(const UUID& other) const {
        return uuid == other.uuid;
    }

    bool UUID::operator!=(const UUID& other) const {
        return uuid != other.uuid;
    }

    std::ostream& operator<<(std::ostream& os, const UUID& uuid)
    {
        return os << uuid.toString();
    }
}
