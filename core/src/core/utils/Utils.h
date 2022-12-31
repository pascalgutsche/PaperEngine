#pragma once
#include "_Core.h"

class Utils {
public:
    static std::string getSystemDateInString();
    static std::string getSystemTimeInString();
    // random function
    static float randRange(int min, int max);

    struct XY {
        float width;
        float height;
    };
    static XY calculateAspectRatioFit(float srcWidth, float srcHeight, float maxWidth, float maxHeight);

    static std::string getClass(std::string object);
    static std::string getClass(const std::type_info& typeInfo);
};
