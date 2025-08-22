#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <unistd.h>

struct State
{
    std::string BuildFolder = "";
    std::string File;
    std::string RunPath = "";
    std::string BasePath = "";
};

enum StateValidationState
{
    OK,
    BadFileName,
    InvalidRP,
    InvalidBP,
    Bad,
    Last
};

#define INVALID_STR "-----"

static const char *SVSToStr[Last + 1] = {
    "OK",  "Bad File Name", "Invalid Run Path", "Invalid Base Path",
    "Bad", "Last",
};

bool CheckIndexArgc(int index, int argc, const char *arg)
{
    if (index + 1 < argc)
        return true;

    std::cout << "No argument after " << arg << std::endl;
    return false;
}

void HandleArgs(char **argv, int argc, State &state)
{
    for (int i = 0; i < argc; i++)
    {
        std::string str = std::string(argv[i]);
        if (str.size() >= 4 && str.compare(str.size() - 4, 4, ".cpp") == 0)
            state.File = str;

        if (str == "-rp")
            if (CheckIndexArgc(i, argc, "-rp"))
                state.RunPath = argv[++i];
            else
                state.RunPath = INVALID_STR;

        if (str == "-bp")
            if (CheckIndexArgc(i, argc, "-bp"))
                state.BasePath = argv[++i];
            else
                state.BasePath = INVALID_STR;
    }
}

StateValidationState IsStateValid(const State &state)
{
    if (state.File.empty())
        return BadFileName;

    if (state.RunPath == INVALID_STR)
        return InvalidRP;

    if (state.BasePath == INVALID_STR)
        return InvalidBP;

    return OK;
}

bool ExecuteState(State &state)
{
    namespace fs = std::filesystem;

    if (state.BuildFolder.empty())
        state.BuildFolder = "./crypt-obj";

    std::string currentPath = fs::absolute(".").string();

    // prepare file path
    std::string filePath = state.File;
    if (!state.BasePath.empty())
        filePath = fs::absolute(fs::path(state.BasePath) / state.File).string();

    std::string binaryName = fs::path(state.File).stem().string();
    std::string output =
        fs::absolute(fs::path(state.BuildFolder) / binaryName).string();

    bool build = true;
    if (fs::exists(output) && fs::exists(filePath))
    {
        if (fs::last_write_time(output) >= fs::last_write_time(filePath))
            build = false;
    }

    system("clear");

    if (build)
    {
        system((std::string("mkdir -p ") + state.BuildFolder).c_str());
        std::cout << filePath << std::endl;
        std::string command = "g++ -g -o " + output + " " + filePath;
        if (system(command.c_str()) != 0)
            return false;
    }

    // change to RunPath if specified
    if (!state.RunPath.empty())
        chdir(fs::absolute(state.RunPath).c_str());

    // run binary
    system(output.c_str());

    // restore working directory
    chdir(currentPath.c_str());

    return true;
}

void PrintUse()
{
    std::cout << "  Welcome to CRYPT! " << std::endl;
    std::cout << "This tool lets you run C scripts from the terminal"
              << std::endl;
    std::cout << "Usage: crypt FILE.cpp [ARGS...]" << std::endl;
    std::cout << "-------------------------------------------------"
              << std::endl;
    std::cout << "ARGS: " << std::endl;
    std::cout << "  -rp - Run path. Indicates where to run the file from;"
              << std::endl;
    std::cout << "  -bp - Base path. Indicates to get the file from;"
              << std::endl;
}

int main(int argc, char **argv)
{
    State state;

    if (argc < 2)
    {
        PrintUse();
        return 0;
    }

    HandleArgs(argv, argc, state);

    StateValidationState valid = IsStateValid(state);

    if (valid == OK)
        ExecuteState(state);
    else
    {
        std::cout << "Invalid state: " << std::endl;
        std::cout << "    " << SVSToStr[valid] << std::endl;
        return -1;
    }

    return 0;
}
