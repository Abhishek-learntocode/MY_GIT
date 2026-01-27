#include "commands.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <vector>

void hashObject(int argc, char* argv[]) {
    // Usage: ./mygit hash-object [-w] <file>
    if (argc < 3) {
        std::cerr << "Usage: ./mygit hash-object [-w] <file>" << std::endl;
        return;
    }

    bool writeFlag = false;
    std::string filename;

    // logic to parse flags
    if (std::string(argv[2]) == "-w") {
        if (argc < 4) {
            std::cerr << "Error: Filename missing after -w" << std::endl;
            return;
        }
        writeFlag = true;
        filename = argv[3];
    } else {
        filename = argv[2];
    }

    try {
        // 1. Read file content
        std::string content = readFile(filename);

        // 2. Create Header: "blob <size>\0"
        std::string header = "blob " + std::to_string(content.size()) + '\0';
        
        // 3. Create the "Store": Header + Content
        // This is what gets hashed and compressed
        std::string store = header + content;

        // 4. Compute SHA-1
        std::string sha1 = computeSHA1(store);

        // 5. If -w is set, write to object database
        if (writeFlag) {
            std::string compressed = compressData(store);
            
            // Path structure: .mygit/objects/aa/bbcc...
            std::string dir = sha1.substr(0, 2);
            std::string file = sha1.substr(2);
            std::string path = ".mygit/objects/" + dir + "/" + file;
            
            writeToFile(path, compressed);
        }

        // 6. Print the Hash
        std::cout << sha1 << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}