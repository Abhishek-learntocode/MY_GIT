#include <iostream>
#include <string>
#include "commands.h"

int main(int argc, char* argv[]) {
    // 1. Check if a command was provided
    if (argc < 2) {
        std::cerr << "Usage: ./mygit <command>" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    // 2. Route to the correct function
    if (command == "init") {
        initRepository();
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }

    return 0;
}