#include "args.hpp"
#include <unordered_map>

Args::Args(const int argc, const char **argv) {
    std::unordered_map<std::string, std::string> kwargs;
    for (int i = 1; i < argc; i++) {
        const std::string argument = argv[i];
        const std::size_t pos = argument.find('=');
        if (pos != std::string::npos) {
            const std::string key = argument.substr(0, pos);
            const std::string value = argument.substr(pos + 1);
            kwargs[key] = value;
        }
    }
    _parseKeywordArgs(kwargs);
}

std::string Args::getPathToInputFile() const {
    return _pathToInputFile;
}

std::string Args::getPathToOutputFile() const {
    return _pathToOutputFile;
}

void Args::_parseKeywordArgs(const std::unordered_map<std::string, std::string> &kwargs) {
    if (kwargs.find("input_file_path") == kwargs.end()) {
        throw std::runtime_error("Missing `input_file_path` named command-line argument!");
    } else if (kwargs.find("output_file_path") == kwargs.end()) {
        throw std::runtime_error("Missing `output_file_path` named command-line argument!");
    }
    _pathToInputFile = kwargs.at("input_file_path");
    _pathToOutputFile = kwargs.at("output_file_path");
}
