#pragma once
#include "Engine.h"

#include "utility.h"

namespace Paper {
	namespace Utils
	{
        std::string GetSystemDateInString();
        std::string GetSystemTimeInString();
        // random function
		float randRange(int min, int max);

        struct Size {
            float width;
            float height;
        };
		Size CalculateAspectRatioFit(float srcWidth, float srcHeight, float maxWidth, float maxHeight);

        char* ReadBytes(const std::filesystem::path& filepath, uint32_t* outSize);

        void ReplaceToken(std::string& string, const std::string& token, const std::string& value);
        void ReplaceTokenInFile(const std::filesystem::path& filePath, const std::string& token, const std::string& value);

        template <typename T>
        std::string TypeToStdString()
        {
            std::string name = typeid(T).name();
            size_t index = name.find_last_of(':');
            std::string typeName = name.substr(++index);
        	return typeName;
        };

        inline std::string ToUpper(std::string string)
        {
        	std::ranges::transform(string, string.begin(), ::toupper);
            return string;
        }

        inline std::string ToLower(std::string string)
        {
            std::ranges::transform(string, string.begin(), ::tolower);
            return string;
        }

	}
}