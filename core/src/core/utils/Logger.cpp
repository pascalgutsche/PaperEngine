#include "_Core.h"

#include "utils/Utils.h"
#include "utils/Logger.h"


// declare map in order to use it as a static map
std::unordered_map<Logger::Level, int> Logger::callCount;
Logger::Level Logger::logLevel;

void Logger::print(std::string message, int logLevelColor)
{
    // \x1B[ == incoming color + ascii color number + m == ending + message + ending suffix
    std::cout << "\x1B[" << logLevelColor << "m" << message << "\033[0m" << std::endl;
}

void Logger::ChangeLevel(Level newLevel) {
    // change debugger level
    logLevel = Debug;
    Log("Changed Logger State to: '" + getLogLevelInString(newLevel) + "'", Trace);
    logLevel = newLevel;
}

void Logger::Log(std::string message, Level level) {
    // call print function that sets the color and needed prefixes and suffixes
    if (logLevel <= level)
    {
        print("[" + core::Utils::getSystemTimeInString() + "] [" + getLogLevelInString(level) + "] " + message, getLogLevelColor(level));
    }
    callCount[level]++;
}
void Logger::Log(std::string message) {
    // quick log message for debugging
    // defaulting
    Log(message, Debug);
}

std::string Logger::getLogLevelInString(Level levelToString) {
    // declare enums to string meanings
    switch (levelToString) {
    case Error:
        return "Error";
    case Warn:
        return "Warn";
    case Trace:
        return "Trace";
    case Debug:
        return "Debug";
    }
    return "";
}

int Logger::getLogLevelColor(Level levelToString) {
    // get text color in int from warning enum
    // color declaration (fg)
    // 31 = red
    // 33 = yellow
    // 32 = blue
    // 34 = green
    // 37 = white
    // for more information see ascii color table
    switch (levelToString) {
    case Error:
        return 31;
    case Warn:
        return 33;
    case Trace:
        return 32;
    case Debug:
        return 34;
    }
    return 37;
}

void Logger::PrintStats() {
    // print statistics at the end of the program, nobody cares anyways
    print("[" + core::Utils::getSystemTimeInString() + "] Logging results:", 37);

    // std::to_string is mandatory because an instant conversion is needed
    print("\tError: " + std::to_string(callCount[Error]), getLogLevelColor(Error));

    print("\tWarn: " + std::to_string(callCount[Warn]), getLogLevelColor(Warn));

    print("\tTrace: " + std::to_string(callCount[Trace]), getLogLevelColor(Trace));

    print("\tDebug: " + std::to_string(callCount[Debug]), getLogLevelColor(Debug));
}