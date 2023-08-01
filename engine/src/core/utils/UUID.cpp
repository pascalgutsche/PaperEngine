#include "Engine.h"
#include "UUID.h"


namespace Paper
{
	static std::random_device randomDevice;
	static std::mt19937_64 engine(randomDevice());
	static std::uniform_int_distribution<uint64_t> distribution;

	UUID::UUID()
		: uuid(distribution(engine))
	{
	}

	UUID::UUID(uint64_t uuid)
		: uuid(uuid)
	{
	}

	UUID::UUID(std::string id)
	{
		std::stringstream ss;
		ss << std::hex << id;
		ss >> uuid;
	}

	void UUID::Set(std::string id)
	{
		std::stringstream ss;
		ss << std::hex << id;
		ss >> uuid;
	}

	void UUID::Set(uint64_t id)
	{
		uuid = id;
	}

	uint64_t UUID::toUInt64() const
	{
		return operator uint64_t();
	}

	std::string UUID::toString() const
	{
		return operator std::string();
	}
}
