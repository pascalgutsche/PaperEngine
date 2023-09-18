#pragma once

namespace Paper
{
	//from Hazel https://github.com/StudioCherno/Hazel
	class Hash
	{
	public:
		static constexpr uint32_t GenerateFNVHash(std::string_view str)
		{
			constexpr uint32_t FNV_PRIME = 16777619u;
			constexpr uint32_t OFFSET_BASIS = 2166136261u;

			const size_t length = str.length();
			const char* data = str.data();

			uint32_t hash = OFFSET_BASIS;
			for (size_t i = 0; i < length; ++i)
			{
				hash ^= *data++;
				hash *= FNV_PRIME;
			}
			hash ^= '\0';
			hash *= FNV_PRIME;

			return hash;
		}

		static constexpr uint32_t GenerateRandonHash()
		{
			std::random_device device;
			std::mt19937 gen(device());
			std::uniform_int_distribution<uint32_t> distribution(0, std::numeric_limits<uint32_t>::max());
			return distribution(gen);
		}
	};
}