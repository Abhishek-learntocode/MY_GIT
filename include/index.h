#ifndef INDEX_H
#define INDEX_H

#include <string>
#include <map>
#include <filesystem>

class Index {
private:
    std::map<std::string, std::string> entries; // filename -> SHA1
    std::filesystem::path indexPath;

public:
    Index();
    void add(const std::string& filename, const std::string& sha1);
    void load();
    void save();
    std::map<std::string, std::string> getEntries() const;
};

#endif