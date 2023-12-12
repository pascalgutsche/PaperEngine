#pragma once
#include "Engine.h"

#include "AssetType.h"
#include "utils/PaperID.h"

namespace Paper
{
	enum class AssetFlag
	{
		None = 0,
		Missing = BIT(0),
		Invalid = BIT(1)
	};

	using AssetHandle = PaperID;

	struct AssetMetadata
	{
		AssetHandle handle = 0;
		AssetType type = AssetType::None;

		std::filesystem::path filePath;

		bool IsValid() const { return handle != AssetHandle(0); }
	};

	class Asset
	{
	public:
		AssetHandle handle;

		uint32_t flags = (uint32_t)AssetFlag::None;

		virtual ~Asset() {};

		virtual AssetType GetAssetType() const { return AssetType::None; }

		bool IsValid() const { return ((flags & (uint32_t)AssetFlag::Missing) | (flags & (uint32_t)AssetFlag::Invalid)) == 0; }

		bool IsFlagSet() const { return flags & (uint32_t)flags; }

		void SetFlag(AssetFlag flag)
		{
			flags |= (uint32_t)flag;
		}

		void RemoveFlag(AssetFlag flag)
		{
			flags &= ~(uint32_t)flag;
		}

		bool operator==(const Asset& other) const
		{
			return handle == other.handle;
		}

		bool operator!=(const Asset& other) const
		{
			return !(*this == other);
		}
	};
}