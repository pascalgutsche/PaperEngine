#include "_Core.h"

#include "utils/Utils.h"

std::string Utils::getSystemDateInString() {
    std::time_t end_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char buf[26];
    ctime_s(buf, sizeof(buf), & end_time);
    return std::string(buf);
}

std::string Utils::getSystemTimeInString() {
    time_t now = time(0);
    struct tm tstruct;
    localtime_s(&tstruct, &now);
    char buf[80];
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%X", &tstruct);

    return std::string(buf);
}

float Utils::randRange(int min, int max)
{
    int diff = max - min;
    // stackoverflow magic
    return (((double)(diff+1)/RAND_MAX) * rand() + min); // math is powerful(!!)
}

Utils::XY Utils::calculateAspectRatioFit(float srcWidth, float srcHeight, float maxWidth, float maxHeight) {

    float ratio = fmin(maxWidth / srcWidth, maxHeight / srcHeight);
    XY xy;
    xy.width = srcWidth * ratio;
    xy.height = srcHeight * ratio;
    return xy;
}

std::string Utils::getClass(std::string object) {
    std::string edit_Component_type;
    int counter = 0;
    if (object[0] == 'P') {
        counter = 1;
        for (; counter < object.size(); counter++) {
            bool found = false;
            for (int i = 0; i < 10; i++) {
                char buff[1];
                //sprintf(buff, "%d", i);
                sprintf_s(buff, 1, "%d", i);
                const char* name = object.c_str();
                if (name[counter] == buff[0]) {
                    found = true;
                }
            }
            if (!found && counter == 1) {
                counter = 0;
                break;
            } else if (!found) {
                break;
            }
        }
    }
    else {
        for (; counter < object.size(); counter++) {
            bool found = false;
            for (int i = 0; i < 10; i++) {
                char buff[1];
                //sprintf(buff, "%d", i);
                sprintf_s(buff, 1, "%d", i);
                const char* name = object.c_str();
                if (name[counter] == buff[0]) {
                    found = true;
                }
            }
            if (!found) {
                break;
            }
        }
    }

    for (; counter < object.size(); counter++) {
        edit_Component_type += object[counter];
    }
    return edit_Component_type;
}

std::string Utils::getClass(const std::type_info& typeInfo) {
    return Utils::getClass(typeInfo.name());
}
