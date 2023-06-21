#pragma once
#include "Engine.h"

#include <boost/uuid/uuid.hpp>

namespace engine
{
	class UUID {
	public:
		UUID();

		UUID(const std::string& uuidStr);

		std::string toString() const;

		bool operator==(const UUID& other) const;

		bool operator!=(const UUID& other) const;

	private:
		mutable boost::uuids::uuid uuid;
	};

	std::ostream& operator<<(std::ostream& os, const UUID& uuid);

}

namespace std {
	template <typename T> struct hash;

	template<>
	struct hash<engine::UUID>
	{
		size_t operator()(const engine::UUID& uuid) const noexcept
		{
			return std::hash<std::string_view>{}(uuid.toString());
		}
	};

}