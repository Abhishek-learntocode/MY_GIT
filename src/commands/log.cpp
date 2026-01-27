#include "commands.h"
#include "utils.h"
#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void log(int argc, char* argv[]) {
    std::string currentSha = "";

    // 1. Read HEAD to find the current commit
    try {
        fs::path headPath = findGitRoot() / "HEAD";
        if (!fs::exists(headPath)) {
            std::cerr << "Error: .mygit/HEAD not found." << std::endl;
            return;
        }

        std::string headContent = readFile(headPath);
        
        // Parse "ref: refs/heads/main\n"
        if (headContent.find("ref: ") == 0) {
            std::string refPathStr = headContent.substr(5);
            // Remove newline if present
            if (!refPathStr.empty() && refPathStr.back() == '\n') {
                refPathStr.pop_back();
            }
            
            // Read the branch file (e.g., .mygit/refs/heads/main)
            fs::path refPath = findGitRoot() / refPathStr;
            if (fs::exists(refPath)) {
                currentSha = readFile(refPath);
            } else {
                std::cout << "No commits yet." << std::endl;
                return;
            }
        } else {
            // Detached HEAD case (not implemented yet, but good to handle)
            currentSha = headContent; 
        }
    } catch (const std::exception& e) {
        std::cerr << "Error reading HEAD: " << e.what() << std::endl;
        return;
    }

    // 2. Traverse History (Linked List)
    while (!currentSha.empty()) {
        try {
            // Get the Commit Object
            std::string path = getObjectPath(currentSha);
            std::string raw = decompressData(readFile(path));
            
            // Parse Header: "commit <size>\0<content>"
            size_t nullPos = raw.find('\0');
            if (nullPos == std::string::npos) break; // Corrupt file
            
            std::string content = raw.substr(nullPos + 1);

            // Print Commit Info
            std::cout << "Commit: \033[33m" << currentSha << "\033[0m" << std::endl; // Yellow Color
            std::cout << "----------------------------------" << std::endl;
            std::cout << content << std::endl;
            std::cout << std::endl;

            // 3. Find Parent to continue the loop
            // Look for "parent <40-char-sha>"
            size_t parentPos = content.find("parent ");
            if (parentPos != std::string::npos) {
                // "parent " is 7 chars long. SHA-1 is 40 chars long.
                currentSha = content.substr(parentPos + 7, 40); 
            } else {
                currentSha = ""; // No parent means we reached the first commit
            }

        } catch (const std::exception& e) {
            std::cerr << "Error walking history at " << currentSha << ": " << e.what() << std::endl;
            break;
        }
    }
}