#include "commands.h"
#include "utils.h"
#include "index.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

namespace fs = std::filesystem;

std::string createTree() {
    // ... (Keep existing createTree logic) ...
    Index index;
    std::string treeContent = "";
    for (const auto& [name, sha] : index.getEntries()) {
        treeContent += "100644 blob " + sha + " " + name + "\n";
    }
    std::string header = "tree " + std::to_string(treeContent.size()) + '\0';
    std::string store = header + treeContent;
    std::string treeSha = computeSHA1(store);
    writeToFile(getObjectPath(treeSha), compressData(store));
    return treeSha;
}

std::string getParentCommit() {
    // ... (Keep existing getParentCommit logic) ...
    fs::path headPath = findGitRoot() / "HEAD";
    std::string headContent = readFile(headPath);
    
    if (headContent.find("ref: ") == 0) {
        std::string refPath = headContent.substr(5);
        if (!refPath.empty() && refPath.back() == '\n') refPath.pop_back();
        
        fs::path branchPath = findGitRoot() / refPath;
        if (fs::exists(branchPath)) {
            return readFile(branchPath);
        }
    }
    return "";
}

void commit(int argc, char* argv[]) {
    // --- ADD TRY CATCH HERE ---
    try {
        if (argc < 3) {
            std::cerr << "Usage: ./mygit commit <message>" << std::endl;
            return;
        }
        std::string message = argv[2];

        std::string treeSha = createTree();
        std::string parentSha = getParentCommit();

        std::stringstream commitData;
        commitData << "tree " << treeSha << "\n";
        if (!parentSha.empty()) commitData << "parent " << parentSha << "\n";
        
        std::time_t now = std::time(nullptr);
        commitData << "author User <user@git.com> " << now << " +0000\n";
        commitData << "committer User <user@git.com> " << now << " +0000\n\n";
        commitData << message << "\n";

        std::string content = commitData.str();
        std::string header = "commit " + std::to_string(content.size()) + '\0';
        std::string store = header + content;
        std::string commitSha = computeSHA1(store);

        writeToFile(getObjectPath(commitSha), compressData(store));

        fs::path headPath = findGitRoot() / "HEAD";
        std::string refPath = readFile(headPath).substr(5);
        if (!refPath.empty() && refPath.back() == '\n') refPath.pop_back();
        
        writeToFile(findGitRoot() / refPath, commitSha);

        std::cout << "[" << commitSha << "] " << message << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}