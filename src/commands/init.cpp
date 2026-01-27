#include "commands.h"
#include "utils.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void initRepository() {
    // 1. Check if already exists
    if (fs::exists(".mygit")) {
        std::cout << "Git repository already initialized." << std::endl;
        return;
    }

    // 2. Create Directory Structure
    try {
        fs::create_directories(".mygit/objects");
        fs::create_directories(".mygit/refs/heads");
        
        // 3. Create HEAD file (CRITICAL STEP)
        // This points to the "main" branch by default
        writeToFile(".mygit/HEAD", "ref: refs/heads/main\n");
        
        std::cout << "Initialized empty MyGit repository in " << fs::absolute(".mygit") << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error initializing repository: " << e.what() << std::endl;
    }
}