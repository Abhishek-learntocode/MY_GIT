#include "commands.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <vector>

void catFile(int argc, char* argv[]) {
    // Usage: ./mygit cat-file -p|-s|-t <sha1>
    if (argc < 4) {
        std::cerr << "Usage: ./mygit cat-file -p|-s|-t <object_sha>" << std::endl;
        return;
    }

    std::string flag = argv[2];
    std::string sha1 = argv[3];

    if (sha1.length() != 40) {
        std::cerr << "Error: Invalid SHA-1 length." << std::endl;
        return;
    }

    try {
        // 1. Get Path using our new helper
        std::string path = getObjectPath(sha1);

        // 2. Read & Decompress
        std::string compressed = readFile(path);
        std::string raw = decompressData(compressed);

        // 3. Parse Header
        // Format: "type size\0content"
        auto nullPos = raw.find('\0');
        if (nullPos == std::string::npos) {
            throw std::runtime_error("Invalid object format");
        }

        std::string header = raw.substr(0, nullPos);
        std::string content = raw.substr(nullPos + 1);

        auto spacePos = header.find(' ');
        std::string type = header.substr(0, spacePos);
        std::string sizeStr = header.substr(spacePos + 1);

        // 4. Handle Flags
        if (flag == "-p") {
            std::cout << content;
        } 
        else if (flag == "-s") {
            std::cout << sizeStr << std::endl;
        } 
        else if (flag == "-t") {
            std::cout << type << std::endl;
        } 
        else {
            std::cerr << "Unknown flag: " << flag << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}