#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

// Core Crypto/Compression
std::string computeSHA1(const std::string& data);
std::string compressData(const std::string& data);
std::string decompressData(const std::string& data);

// File Operations
std::string readFile(const fs::path& filePath);
void writeToFile(const fs::path& filePath, const std::string& data);

// Git Specific
fs::path findGitRoot(); 
std::string getObjectPath(const std::string& sha1);

#endif