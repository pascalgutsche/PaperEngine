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

	}
}