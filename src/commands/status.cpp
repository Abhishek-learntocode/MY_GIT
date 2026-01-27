#include "commands.h"
#include "index.h"
#include "utils.h"
#include <iostream>
#include <filesystem>
#include <set>
#include <vector>

namespace fs = std::filesystem;

void status(int argc, char* argv[]) {
    Index index;
    // Map: Filename -> SHA1 (Currently in Staging Area)
    std::map<std::string, std::string> indexEntries = index.getEntries();
    
    std::vector<std::string> untracked;
    std::vector<std::string> modified;
    std::set<std::string> filesOnDisk;

    // 1. Scan the real files on your hard drive
    for (const auto& entry : fs::directory_iterator(".")) {
        std::string filename = entry.path().filename().string();

        // Ignore internal files
        if (filename == ".mygit" || filename == ".git" || filename == ".gitignore") continue;
        if (filename == "mygit.exe" || filename == "Makefile") continue;
        if (filename.find(".o") != std::string::npos) continue;
        if (entry.is_directory()) continue; 

        filesOnDisk.insert(filename);

        // CASE A: File is NOT in the Index -> UNTRACKED
        if (indexEntries.find(filename) == indexEntries.end()) {
            untracked.push_back(filename);
        } 
        else {
            // CASE B: File IS in Index. Did it change?
            // We must hash the file on disk RIGHT NOW to see if it matches the Index.
            try {
                std::string content = readFile(filename);
                std::string header = "blob " + std::to_string(content.size()) + '\0';
                std::string store = header + content;
                std::string currentSha = computeSHA1(store);

                // If Hash on Disk != Hash in Index, it is MODIFIED
                if (currentSha != indexEntries[filename]) {
                    modified.push_back(filename);
                }
            } catch (...) {
                // Read error, skip
            }
        }
    }

    // CASE C: File is in Index, but NOT on disk -> DELETED
    std::vector<std::string> deleted;
    for (const auto& [name, sha] : indexEntries) {
        if (filesOnDisk.find(name) == filesOnDisk.end()) {
            deleted.push_back(name);
        }
    }

    // --- OUTPUT ---
    bool clean = true;

    if (!modified.empty()) {
        std::cout << "Changes not staged for commit (Modified):" << std::endl;
        for (const auto& f : modified) std::cout << "  \033[31m" << f << "\033[0m" << std::endl; // Red
        clean = false;
    }
    
    if (!deleted.empty()) {
        std::cout << "Deleted files:" << std::endl;
        for (const auto& f : deleted) std::cout << "  \033[31m" << f << "\033[0m" << std::endl; // Red
        clean = false;
    }

    if (!untracked.empty()) {
        std::cout << "Untracked files:" << std::endl;
        for (const auto& f : untracked) std::cout << "  \033[31m" << f << "\033[0m" << std::endl; // Red
        clean = false;
    }

    if (clean) {
        std::cout << "nothing to commit, working tree clean" << std::endl;
    } else {
        std::cout << "\n(Use './mygit add <file>' to stage changes)" << std::endl;
    }
}