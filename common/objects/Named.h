//
// Created by Oz Zafar on 06/06/2020.
//

#ifndef CPP_STOWAGE_NAMED_H
#define CPP_STOWAGE_NAMED_H

template<typename T>
class Named {
    T value;
public:
    explicit Named(T value): value{value} {}
    operator T() const {return value;}
};



#endif //CPP_STOWAGE_NAMED_H
