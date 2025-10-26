#include "PluginManager.h"
#include "SharedLibrary.h"
#include <iostream>
#include <stdexcept>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dirent.h>
#endif

namespace calculator {

PluginManager::PluginManager() = default;

PluginManager::~PluginManager() = default;

bool PluginManager::load(const std::string& dirPath) {
    int loaded = 0;
    
#ifdef _WIN32
    WIN32_FIND_DATAA findData;
    std::string searchPath = dirPath + "\\*.dll";
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);
    
    if (hFind == INVALID_HANDLE_VALUE) return false;

    do {
        std::string filename = findData.cFileName;
        try {
            auto lib = std::make_unique<SharedLibrary>(dirPath + "\\" + filename);
            functions[lib->getName()] = lib->getFunc();
            // std::cout << "Loaded: " << lib->getName() << "\n";
            libraries.push_back(std::move(lib));
            loaded++;
        } catch (const std::exception& e) {
            std::cerr << "Error loading " << filename << ": " << e.what() << "\n";
        }
    } while (FindNextFileA(hFind, &findData));
    
    FindClose(hFind);
#else
    DIR* dir = opendir(dirPath.c_str());
    if (!dir) return false;

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string filename = entry->d_name;
        if (filename.find(".so") != std::string::npos) {
            try {
                auto lib = std::make_unique<SharedLibrary>(dirPath + "/" + filename);
                functions[lib->getName()] = lib->getFunc();
                // std::cout << "Loaded: " << lib->getName() << "\n";
                libraries.push_back(std::move(lib));
                loaded++;
            } catch (const std::exception& e) {
                std::cerr << "Error loading " << filename << ": " << e.what() << "\n";
            }
        }
    }
    
    closedir(dir);
#endif

    return loaded > 0;
}

bool PluginManager::hasFunction(const std::string& name) const {
    return functions.find(name) != functions.end();
}

double PluginManager::call(const std::string& name, double arg) {
    auto it = functions.find(name);
    if (it == functions.end()) {
        throw std::runtime_error("Unknown function: " + name);
    }
    return it->second(arg);
}

}