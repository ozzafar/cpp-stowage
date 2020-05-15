//
// Created by Oz Zafar on 10/05/2020.
//

#ifndef CPP_STOWAGE_REGISTRAR_H
#define CPP_STOWAGE_REGISTRAR_H

#include <vector>
#include <iostream>
#include <functional>
#include <memory>
#include "../../interfaces/AbstractAlgorithm.h"

#ifdef RUNNING_ON_NOVA
#include <dlfcn.h>
#endif
using std::vector;


class Registrar {

public:  // need to be private

    static Registrar instance;
    vector<std::function<std::unique_ptr<AbstractAlgorithm>()>> factoryVec;
    vector<std::string> names;
    std::string extractFilenameFromPath(const std::string &path, bool removeExtension);

    virtual ~Registrar();

//    TODO causing segmentation fault
       struct DlCloser{
        void operator()(void* dlHandle) const noexcept {
            std::cout << "Closing handle" << std::endl;
            (void) dlHandle; //unused
            //dlclose(dlHandle);
        }
    };

//    std::vector<std::unique_ptr<void,DlCloser>> handles;


public:
    int loadSO(const std::string& path);

    void registerAlgorithm(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithm) {
        factoryVec.push_back(algorithm);
        std::cout << "Algorithm was registered!";
    }

    static Registrar& getInstance() {
        return instance;
    }

    void addName(const std::string &name);

};

#endif //CPP_STOWAGE_REGISTRAR_H
