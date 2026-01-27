#include "utils.h"
#include <openssl/sha.h>
#include <zlib.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <filesystem>
#include <vector>
#include <stdexcept>

namespace fs = std::filesystem;

std::string computeSHA1(const std::string& data) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    // Calculate SHA1 of the input data
    SHA1(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);

    // Convert binary hash to 40-character hex string
    std::stringstream ss;
    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

std::string compressData(const std::string& data) {
    // Calculate the maximum possible size of compressed data
    uLongf compressedSize = compressBound(data.size());
    std::vector<char> buffer(compressedSize);

    // Perform compression
    if (compress(reinterpret_cast<Bytef*>(buffer.data()), &compressedSize, 
                 reinterpret_cast<const Bytef*>(data.c_str()), data.size()) != Z_OK) {
        throw std::runtime_error("Failed to compress data");
    }

    // Return strictly the compressed bytes
    return std::string(buffer.data(), compressedSize);
}

std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filePath);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void writeToFile(const std::string& filePath, const std::string& data) {
    // Ensure the directory exists (e.g., .mygit/objects/aa/)
    fs::path path(filePath);
    if (path.has_parent_path()) {
        fs::create_directories(path.parent_path());
    }

    std::ofstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot write to file: " + filePath);
    }
    file.write(data.data(), data.size());
}