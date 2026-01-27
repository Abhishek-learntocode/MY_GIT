#include <iostream>
#include <string>
#include "commands.h"
#include "utils.h" // We might need this for some setups, safe to include

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
    else if (command == "cat-file") {
        catFile(argc, argv);
    }
    // --- NEW LINES START HERE ---
    else if (command == "commit") {
        commit(argc, argv);
    }
    else if (command == "log") {
        log(argc, argv);
    }
    else if (command == "add") {
        add(argc, argv);
    }
    // --- NEW LINES END HERE ---
    else {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }

    return 0;
}