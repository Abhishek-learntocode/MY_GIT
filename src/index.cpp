#include "index.h"
#include "utils.h" // We need findGitRoot() from here
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

Index::Index() {
    try {
        indexPath = findGitRoot() / "index";
        load();
    } catch (...) {
        // If not in a repo yet, do nothing (safe fallback)
    }
}

void Index::add(const std::string& filename, const std::string& sha1) {
    entries[filename] = sha1;
}

void Index::save() {
    std::ofstream file(indexPath);
    for (const auto& [name, sha] : entries) {
        file << sha << " " << name << "\n";
    }
}

void Index::load() {
    if (!fs::exists(indexPath)) return;
    std::ifstream file(indexPath);
    std::string sha, name;
    while (file >> sha >> name) {
        entries[name] = sha;
    }
}

std::map<std::string, std::string> Index::getEntries() const {
    return entries;
}