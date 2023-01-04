#include "_Core.h"

#include "generic/Window.h"

//using namespace Logger;
// why what the fu
class Runtime {
public:
    Runtime(const char* args) {
        setLogger(args);
    }

    ~Runtime() {
        Logger::PrintStats();
    }
    // oh my god what the
    void setLogger(const char* args) const {
        if (args == nullptr) {
            Logger::ChangeLevel(Logger::Debug);
            return;
        }
        if (strcmp(args, "-ll=Error") == 0) {
            Logger::ChangeLevel(Logger::Error);
            return;
        }
        if (strcmp(args, "-ll=Warn") == 0) {
            Logger::ChangeLevel(Logger::Warn);
            return;
        }
        if (strcmp(args, "-ll=Trace") == 0) {
            Logger::ChangeLevel(Logger::Trace);
            return;
        }
        if (strcmp(args, "-ll=Debug") == 0) {
            Logger::ChangeLevel(Logger::Debug);
            return;
        }
        Logger::ChangeLevel(Logger::Trace);
    }
};

void lookForExit() {
    std::cin.get();
    core::Window::quit();
}

int main(int argc, char* argv[])
{
    // useless magic, will be removed later on
    std::unique_ptr<Runtime> runtime = std::make_unique<Runtime>(argv[1]);

    std::thread cliExit(lookForExit);

    int var = core::Window::run();

    cliExit.detach();

    return var;
}