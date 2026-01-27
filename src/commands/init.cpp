#include "commands.h"
#include <iostream>
#include <filesystem> // C++17 standard library for file handling

namespace fs = std::filesystem;

void initRepository() {
    // Check if .mygit already exists
    if (fs::exists(".mygit")) {
        std::cout << "Reinitialized existing MyGit repository in .mygit/" << std::endl;
        return;
    }

    // Create the directory structure
    try {
        // 1. Create .mygit directory
        if (fs::create_directory(".mygit")) {
            // 2. Create subdirectories for objects and refs (future-proofing)
            fs::create_directory(".mygit/objects");
            fs::create_directory(".mygit/refs");
            fs::create_directory(".mygit/refs/heads");

            // 3. Create the HEAD file pointing to master
            // (We will write file handling helpers later, keeping it simple for now)
            std::cout << "Initialized empty MyGit repository in .mygit/" << std::endl;
        } else {
            std::cerr << "Error: Failed to create .mygit directory." << std::endl;
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
}