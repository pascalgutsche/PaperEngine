#pragma once
#include "Engine.h"

namespace Paper
{
	class PaperID
	{
	public:
		PaperID();
		PaperID(uint64_t uuid);
		PaperID(std::string uuid);
		PaperID(const PaperID&) = default;

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
	struct hash<Paper::PaperID>
	{
		std::size_t operator()(const Paper::PaperID& uuid) const
		{
			return (uint64_t)uuid;
		}
	};

}