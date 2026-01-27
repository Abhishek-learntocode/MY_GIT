#ifndef UTILS_H
#define UTILS_H

#include <string>

// Computes the SHA-1 hash of a string and returns the hex string
std::string computeSHA1(const std::string& data);

// Compresses data using Zlib
std::string compressData(const std::string& data);

// Reads the entire content of a file into a string
std::string readFile(const std::string& filePath);

// Writes data to a file (creating directories if needed)
void writeToFile(const std::string& filePath, const std::string& data);

#endif