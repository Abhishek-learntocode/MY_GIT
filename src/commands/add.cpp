#include "commands.h"
#include "index.h"
#include "utils.h"
#include <iostream>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

// Helper to hash a file and write it to object store
std::string hashAndWriteFile(const std::string& filename) {
    std::string content = readFile(filename);
    std::string header = "blob " + std::to_string(content.size()) + '\0';
    std::string store = header + content;
    std::string sha1 = computeSHA1(store);
    
    // Write the compressed blob to .mygit/objects
    writeToFile(getObjectPath(sha1), compressData(store));
    return sha1;
}

void add(int argc, char* argv[]) {
    // Usage: ./mygit add <filename>
    if (argc < 3) {
        std::cerr << "Usage: ./mygit add <filename>" << std::endl;
        return;
    }

    std::string filename = argv[2];
    
    // Check if file exists
    if (!fs::exists(filename)) {
        std::cerr << "File not found: " << filename << std::endl;
        return;
    }

    try {
        Index index;
        // 1. Hash the file and save blob to objects/
        std::string sha1 = hashAndWriteFile(filename);
        
        // 2. Add entry to index (staging area)
        index.add(filename, sha1);
        
        // 3. Save index to disk
        index.save();
        
        std::cout << "Added " << filename << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}