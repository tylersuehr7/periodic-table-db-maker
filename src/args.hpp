#ifndef ARGS_HPP
#define ARGS_HPP

#include <string>
#include <unordered_map>

class Args {
public:
    explicit Args(const int argc, const char **argv);
    std::string getPathToInputFile() const;
    std::string getPathToOutputFile() const;
private:
    std::string _pathToInputFile;
    std::string _pathToOutputFile;
    void _parseKeywordArgs(const std::unordered_map<std::string, std::string>&);
};

#endif//ARGS_HPP