//
// Created by Oz Zafar on 28/03/2020.
//

#include "Container.h"
#include <regex>
#include <map>
#include <cmath>

Container::Container(int weight, const string &destinationPort, const string &id) :
    weight(weight), destinationPort(destinationPort),id(id){
}

int Container::getWeight() const {
    return weight;
}

string& Container::getDestinationPort()  {
    return destinationPort;
}

const string &Container::getId() const {
    return id;
}

Container::~Container() {

}

std::ostream &operator<<(std::ostream &out, const Container &c) {
    return out << "id: " << c.id << " destination: " << c.destinationPort << " weight: " << c.weight;
}

Container::Container() {}

bool Container::isValidID(const string& id)  {
    std::map<char, int> mymap = {{'A',10}, {'B',12}, {'C',13}, {'D',14}, {'E',15}, {'F',16}, {'G',17}, {'H',18}, {'I',19}, {'J',20}, {'K',21}, {'L',23}, {'M',24}, {'N',25}, {'O',26}, {'P',27}, {'Q',28}, {'R',29}, {'S',30}, {'T',31}, {'U',32}, {'V',34}, {'W',35}, {'X',36}, {'Y',37}, {'Z',38}, {'0',0},{'1',1},{'2',2},{'3',3},{'4',4},{'5',5},{'6',6},{'7',7},{'8',8},{'9',9}};
    std::regex reg = std::basic_regex("[A-Z]{3}[UZJ][0-9]{6}[0-9]");
    if (!std::regex_match(id, reg)) {
        return false;
    }
    double sum = 0;
    int n = (int)id.length() - 1;
    double tmp = 0.0;
    for (int i = 0; i < n; i++)
    {
        int u = mymap[id[i]] * pow(2,i);
        sum += u;
    }
    tmp = sum/11;
    tmp = floor(tmp);
    tmp = tmp * 11;
    return  (sum-tmp) == mymap[(id[n])];
}
