#include "commands.h"
#include "utils.h"
#include "index.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

namespace fs = std::filesystem;

std::string createTree() {
    Index index;
    std::string treeContent = "";
    
    // Convert Index list to Tree format: "100644 blob <sha1> <filename>"
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
    fs::path headPath = findGitRoot() / "HEAD";
    std::string headContent = readFile(headPath);
    
    // HEAD usually contains "ref: refs/heads/main\n"
    if (headContent.find("ref: ") == 0) {
        std::string refPath = headContent.substr(5);
        if (!refPath.empty() && refPath.back() == '\n') refPath.pop_back();
        
        fs::path branchPath = findGitRoot() / refPath;
        if (fs::exists(branchPath)) {
            return readFile(branchPath);
        }
    }
    return ""; // First commit has no parent
}

void commit(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: ./mygit commit <message>" << std::endl;
        return;
    }
    std::string message = argv[2];

    // 1. Create Tree
    std::string treeSha = createTree();
    // 2. Get Parent
    std::string parentSha = getParentCommit();

    // 3. Build Commit Content
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

    // 4. Write Commit Object
    writeToFile(getObjectPath(commitSha), compressData(store));

    // 5. Update HEAD (Move the branch pointer)
    fs::path headPath = findGitRoot() / "HEAD";
    std::string refPath = readFile(headPath).substr(5);
    if (!refPath.empty() && refPath.back() == '\n') refPath.pop_back();
    
    writeToFile(findGitRoot() / refPath, commitSha);

    std::cout << "[" << commitSha << "] " << message << std::endl;
}