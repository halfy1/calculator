#include "PluginManager.h"
#include "SharedLibrary.h"
#include <dirent.h>
#include <iostream>
#include <stdexcept>

void PluginManager::load(const std::string & dirPath) {
    DIR* dir = opendir(dirPath.c_str());
    if (!dir) return;

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string filename = entry->d_name;
        if (filename.find(".so") != std::string::npos) {
            try {
                SharedLibrary lib(dirPath + "/" + filename);
                functions[lib.getName()] = lib.getFunc(); 
            } catch (const std::exception & e) {
                std::cerr << "Error loading" << filename << ": " << e.what() << std::endl;
            }
        }
    }
    
    closedir(dir);
}

bool PluginManager::hasFunction(const std::string & name) const {
    return functions.find(name) != functions.end();
}

double PluginManager::call(const std::string & name, double arg) {
    auto it = functions.find(name);
    if (it == functions.end()) {
        throw std::runtime_error("Unknown function:" + name);
    }
    return it->second(arg);
}