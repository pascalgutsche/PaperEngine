#include "Engine.h"
#include "EntityID.h"


namespace Paper
{
	static std::random_device randomDevice;
	static std::mt19937_64 engine(randomDevice());
	static std::uniform_int_distribution<uint64_t> distribution;

	EntityID::EntityID()
		: uuid(distribution(engine))
	{
	}

	EntityID::EntityID(uint64_t uuid)
		: uuid(uuid)
	{
	}

	EntityID::EntityID(std::string id)
	{
		std::stringstream ss;
		ss << std::hex << id;
		ss >> uuid;
	}

	void EntityID::Set(std::string id)
	{
		std::stringstream ss;
		ss << std::hex << id;
		ss >> uuid;
	}

	void EntityID::Set(uint64_t id)
	{
		uuid = id;
	}

	uint64_t EntityID::toUInt64() const
	{
		return operator uint64_t();
	}

	std::string EntityID::toString() const
	{
		return operator std::string();
	}
}
