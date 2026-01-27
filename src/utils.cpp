#include "utils.h"
#include <openssl/sha.h>
#include <zlib.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <filesystem>

// ---------------------------------------------------
// CRYPTO & COMPRESSION
// ---------------------------------------------------

std::string computeSHA1(const std::string& data) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);

    std::stringstream ss;
    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

std::string compressData(const std::string& data) {
    uLongf compressedSize = compressBound(data.size());
    std::vector<char> buffer(compressedSize);

    if (compress(reinterpret_cast<Bytef*>(buffer.data()), &compressedSize, 
                 reinterpret_cast<const Bytef*>(data.c_str()), data.size()) != Z_OK) {
        throw std::runtime_error("Failed to compress data");
    }

    return std::string(buffer.data(), compressedSize);
}

std::string decompressData(const std::string& data) {
    uLongf originalSize = data.size() * 10; // Simple estimate
    std::vector<char> buffer(originalSize);

    // Try to decompress
    int result = uncompress(reinterpret_cast<Bytef*>(buffer.data()), &originalSize, 
                            reinterpret_cast<const Bytef*>(data.c_str()), data.size());

    if (result != Z_OK) {
        // In a real app, we would handle Z_BUF_ERROR (buffer too small) by resizing
        throw std::runtime_error("Failed to decompress data (Zlib error: " + std::to_string(result) + ")");
    }

    return std::string(buffer.data(), originalSize);
}

// ---------------------------------------------------
// FILE OPERATIONS
// ---------------------------------------------------

std::string readFile(const fs::path& filePath) {
    if (!fs::exists(filePath)) {
        throw std::runtime_error("File not found: " + filePath.string());
    }
    std::ifstream file(filePath, std::ios::binary);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void writeToFile(const fs::path& filePath, const std::string& data) {
    // Create parent directories if they don't exist (e.g., .mygit/objects/aa/)
    if (filePath.has_parent_path()) {
        fs::create_directories(filePath.parent_path());
    }

    std::ofstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot write to file: " + filePath.string());
    }
    file.write(data.data(), data.size());
}

// ---------------------------------------------------
// GIT SPECIFIC UTILS
// ---------------------------------------------------

fs::path findGitRoot() {
    fs::path current = fs::current_path();
    // Search up the directory tree for .mygit
    while (true) {
        if (fs::exists(current / ".mygit")) {
            return current / ".mygit";
        }
        if (current.has_parent_path() && current != current.parent_path()) {
            current = current.parent_path();
        } else {
            throw std::runtime_error("Not a git repository (or any of the parent directories)");
        }
    }
}

std::string getObjectPath(const std::string& sha1) {
    fs::path root = findGitRoot();
    return (root / "objects" / sha1.substr(0, 2) / sha1.substr(2)).string();
}