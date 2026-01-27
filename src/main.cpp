#include <iostream>
#include <string>
#include "commands.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./mygit <command>" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "init") {
        initRepository();
    } 
    else if (command == "hash-object") {
        hashObject(argc, argv);
    }
    else {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }

    return 0;
}