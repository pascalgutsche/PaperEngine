#pragma once
#include "_Core.h"

class Logger {
public:
    enum Level {
         Debug, Trace, Warn, Error
    };

    static Level logLevel;
    // save all loglevels in a map (bloat)
    static std::unordered_map<Level, int> callCount;


    static void print(std::string message, int logLevelColor);

    static std::string getLogLevelInString(Level levelToString);
    static int getLogLevelColor(Level levelToString);

    static void ChangeLevel(Level newLevel);

    static void Log(std::string message, Level level);
    static void Log(std::string message);

    static void PrintStats();
};