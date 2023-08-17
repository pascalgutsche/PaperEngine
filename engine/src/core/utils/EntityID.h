#pragma once
#include "Engine.h"

namespace Paper
{
	class EntityID
	{
	public:
		EntityID();
		EntityID(uint64_t uuid);
		EntityID(std::string uuid);
		EntityID(const EntityID&) = default;

		void Set(std::string id);
		void Set(uint64_t id);

		uint64_t toUInt64() const;
		std::string toString() const;

		operator uint64_t() const { return uuid; }
		operator std::string() const
		{
			std::ostringstream ss;
			ss << std::setfill('0') << std::setw(16) << std::hex << uuid;
			return ss.str();
		}
	private:
		uint64_t uuid;
	};

}

namespace std {
	template <typename T> struct hash;

	template<>
	struct hash<Paper::EntityID>
	{
		std::size_t operator()(const Paper::EntityID& uuid) const
		{
			return (uint64_t)uuid;
		}
	};

}