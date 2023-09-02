#include "Engine.h"
#include "PaperID.h"

#include "scene/Entity.h"

namespace Paper
{
	static std::random_device randomDevice;
	static std::mt19937_64 engine(randomDevice());
	static std::uniform_int_distribution<uint64_t> distribution;

	PaperID::PaperID()
		: uuid(distribution(engine))
	{
	}

	PaperID::PaperID(uint64_t uuid)
		: uuid(uuid)
	{
	}

	PaperID::PaperID(std::string id)
	{
		std::stringstream ss;
		ss << std::hex << id;
		ss >> uuid;
	}

	void PaperID::Set(std::string id)
	{
		std::stringstream ss;
		ss << std::hex << id;
		ss >> uuid;
	}

	void PaperID::Set(uint64_t id)
	{
		uuid = id;
	}

	bool PaperID::Empty() const
	{
		return !uuid;
	}

	uint64_t PaperID::toUInt64() const
	{
		return operator uint64_t();
	}

	std::string PaperID::toString() const
	{
		return operator std::string();
	}

	Entity PaperID::ToEntity() const
	{
		if (Scene::GetActive())
			return Scene::GetActive()->GetEntity(*this);
		return {};
	}
}
