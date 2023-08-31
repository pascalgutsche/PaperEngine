#include "Engine.h"

#include "utils/Utils.h"

#include "Timer.h"

namespace Paper {
    namespace Utils {
        std::string GetSystemDateInString() {
            std::time_t end_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            char buf[26];
            ctime_s(buf, sizeof(buf), &end_time);
            return std::string(buf);
        }

        std::string GetSystemTimeInString() {
            time_t now = time(0);
            struct tm tstruct;
            localtime_s(&tstruct, &now);
            char buf[80];
            // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
            // for more information about date/time format
            strftime(buf, sizeof(buf), "%X", &tstruct);

            return std::string(buf);
        }

        float randRange(int min, int max)
        {
            int diff = max - min;
            // stackoverflow magic
            return (((double)(diff + 1) / RAND_MAX) * rand() + min); // math is powerful(!!)
        }

        Size CalculateAspectRatioFit(float srcWidth, float srcHeight, float maxWidth, float maxHeight) {

            float ratio = fmin(maxWidth / srcWidth, maxHeight / srcHeight);
            Size xy;
            xy.width = srcWidth * ratio;
            xy.height = srcHeight * ratio;
            return xy;
        }

        //http://nilssondev.com/mono-guide/book/first-steps/loading-assemblies.html#c-code-1
        char* ReadBytes(const std::filesystem::path& filepath, uint32_t* outSize)
        {
            std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

            if (!stream)
            {
                // Failed to open the file
                return nullptr;
            }

            std::streampos end = stream.tellg();
            stream.seekg(0, std::ios::beg);
            uint32_t size = end - stream.tellg();

            if (size == 0)
            {
                // File is empty
                return nullptr;
            }

            char* buffer = new char[size];
            stream.read((char*)buffer, size);
            stream.close();

            *outSize = size;
            return buffer;
        }

        void ReplaceToken(std::string& string, const std::string& token, const std::string& value)
        {
            Timer timer;
            size_t pos = 0;
            while((pos = string.find(token, pos)) != std::string::npos)
            {
                string.replace(pos, token.length(), value);
                if (timer.GetElapsedMillis() > 250.0f) break;
            }
        }

        void ReplaceTokenInFile(const std::filesystem::path& filePath, const std::string& token,
	        const std::string& value)
        {
            std::ifstream istream(filePath);
            if (!istream.is_open())
            {
                LOG_CORE_ERROR("Could not find file '{}'", filePath.string());
                return;
            }

            std::stringstream ss;
            ss << istream.rdbuf();
            istream.close();

            std::string string = ss.str();
            ReplaceToken(string, token, value);
            std::ofstream ostream(filePath);
            ostream << string;
            ostream.close();
        }
    }
}
