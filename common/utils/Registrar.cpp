//
// Created by Oz Zafar on 10/05/2020.
//

#include "Registrar.h"

Registrar Registrar::instance;

int Registrar::loadSO(const std::string &path) {
    std::cout << "entered loadSO " << path << std::endl;
#ifdef RUNNING_ON_NOVA
    std::unique_ptr<void, DlCloser> handle(dlopen(path.c_str(), RTLD_LAZY));
    if (!handle) {
        std::cout << "loadSO failed" << std::endl;
        return 1;
    }

    handles.push_back(std::move(handle));
    std::cout << "loadSO succeedded" << std::endl;
    names.push_back(extractFilenameFromPath(path, true));
    std::cout << names.size() << std::endl;
#endif

    (void) path;
    return (int)(Error::SUCCESS);

}

std::string Registrar::extractFilenameFromPath(const std::string &path, bool removeExtension) {
    std::string pathCopy = path;  // it works inplace so we create a copy
    // Remove directory if present
    // Do this before extension removal incase directory has a period character
    const size_t last_slash_idx = pathCopy.find_last_of("\\/");
    if (std::string::npos != last_slash_idx) {
        pathCopy.erase(0, last_slash_idx + 1);
    }
    // Remove extension if present
    if (removeExtension) {
        const size_t period_idx = pathCopy.rfind('.');
        if (std::string::npos != period_idx) {
            pathCopy.erase(period_idx);
        }
    }

    return pathCopy;
}

Registrar::~Registrar() {
    factoryVec.clear();
#ifdef RUNNING_ON_NOVA
    handles.clear();
#endif

}

void Registrar::addName(const std::string &name) {
    names.push_back(name);
}

int Registrar::getSize() {
    return getInstance().factoryVec.size();
}
