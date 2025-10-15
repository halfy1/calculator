#include "SharedLibrary.h"
#include <stdexcept>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

typedef double (*ExecFunc)(double);
typedef const char* (*GetNameFunc)();

SharedLibrary::SharedLibrary(const std::string& path) : handle(nullptr) {
    load(path);
}

SharedLibrary::~SharedLibrary() {
#ifdef _WIN32
    if (handle) FreeLibrary((HMODULE)handle);
#else
    if (handle) dlclose(handle);
#endif
}

SharedLibrary::SharedLibrary(SharedLibrary&& other) noexcept
    : handle(other.handle), name(std::move(other.name)), func(std::move(other.func)) {
    other.handle = nullptr;
}

SharedLibrary& SharedLibrary::operator=(SharedLibrary&& other) noexcept {
    if (this != &other) {
#ifdef _WIN32
        if (handle) FreeLibrary((HMODULE)handle);
#else
        if (handle) dlclose(handle);
#endif
        handle = other.handle;
        name = std::move(other.name);
        func = std::move(other.func);
        other.handle = nullptr;
    }
    return *this;
}

void SharedLibrary::load(const std::string& path) {
#ifdef _WIN32
    handle = LoadLibraryA(path.c_str());
    if (!handle) {
        throw std::runtime_error("Failed to load: " + path);
    }

    ExecFunc execFunc = (ExecFunc)GetProcAddress((HMODULE)handle, "execute");
    if (!execFunc) {
        FreeLibrary((HMODULE)handle);
        throw std::runtime_error("execute() not found in library");
    }

    GetNameFunc getNameFunc = (GetNameFunc)GetProcAddress((HMODULE)handle, "getName");
    if (!getNameFunc) {
        FreeLibrary((HMODULE)handle);
        throw std::runtime_error("getName() not found in library");
    }
#else
    handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle) {
        throw std::runtime_error(std::string("Failed to load: ") + dlerror());
    }

    ExecFunc execFunc = (ExecFunc)dlsym(handle, "execute");
    if (!execFunc) {
        dlclose(handle);
        throw std::runtime_error("execute() not found in library");
    }

    GetNameFunc getNameFunc = (GetNameFunc)dlsym(handle, "getName");
    if (!getNameFunc) {
        dlclose(handle);
        throw std::runtime_error("getName() not found in library");
    }
#endif

    name = getNameFunc();
    func = execFunc;
}