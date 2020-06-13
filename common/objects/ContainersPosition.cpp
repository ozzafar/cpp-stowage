//
// Created by Oz Zafar on 03/04/2020.
//

#include <thread>
#include <mutex>
#include "ContainersPosition.h"
#include "../utils/Errors.h"


// region methods


int ContainersPosition::load(const string& containerId, bool print) {
    std::mutex g_display_mutex;
    if (howManyAvailiable() > 0) {
        containers.push_back(containerId);
        if(print)
        {
            g_display_mutex.lock();
            std::cout << "\t\tThread id " << std::this_thread::get_id() <<" Container with id: " + containerId + " was loaded" << std::endl;
            g_display_mutex.unlock();
        }
        return (int)Error::SUCCESS;
    }
    g_display_mutex.lock();
    std::cout << "\t\tThread id " << std::this_thread::get_id() << "No available place for container with id: " + containerId << std::endl;
    g_display_mutex.unlock();
    return 1;
}

int ContainersPosition::unload(const string& containerId, bool print){
    std::mutex g_display_mutex;
    string& topId = containers.back();
    if (topId == containerId){
        containers.pop_back();
        if(print)
        {
            g_display_mutex.lock();
            std::cout << "\t\tThread id " << std::this_thread::get_id() << "Container with id: " + containerId + " was unloaded" << std::endl;
            g_display_mutex.unlock();
        }
        return (int)Error::SUCCESS;
    } else {
        g_display_mutex.lock();
        std::cout << "\t\tThread id " << std::this_thread::get_id() << "Container with id: " + containerId + " isn't on top of the position" << std::endl;
        g_display_mutex.unlock();
        return 1;
    }
}

int ContainersPosition::howManyAvailiable() {
    return numOfFloors - startFloor - containers.size();
}

void ContainersPosition::setStartFloor(int startFloorP) {
    ContainersPosition::startFloor = startFloorP;
}

int ContainersPosition::getStartFloor() const {
    return startFloor;
}

int ContainersPosition::getNumOfActiveFloors() const {
    return containers.size();
}

int ContainersPosition::getTopFloorNumber() {
    return startFloor+getNumOfActiveFloors()-1;
}

list<string>::iterator ContainersPosition::begin()
{
    return containers.begin();
}

list<string>::iterator ContainersPosition::end()
{
    return containers.end();
}


string &ContainersPosition::getTop() {
    return containers.back();
}

ContainersPosition::ContainersPosition(int numOfFloors) : numOfFloors(numOfFloors) {}

int ContainersPosition::load(const string &containerId) {
    return load(containerId, false);
}

int ContainersPosition::unload(const string &containerId) {
    return unload(containerId, false);
}

int ContainersPosition::move(ContainersPosition &source, ContainersPosition &target, const string& containerId) {
    if (source.unload(containerId)==(int)Error::SUCCESS){
        return target.load(containerId);
    }
    return 1;
}



// endregion
