//
// Created by Oz Zafar on 06/07/2020.
//

#ifndef CPP_STOWAGE_SHIP_H
#define CPP_STOWAGE_SHIP_H

#include <vector>
#include <unordered_map>
#include <tuple>
#include <functional>
#include <string>
#include <iostream>

using std::string;
using std::vector;

namespace shipping {
    template<typename T> class NamedType {
        T t;
    public:
        explicit NamedType(T t): t(t) {}
        operator T() const {
            return t;
        }
    };

    struct X : NamedType<int> {
        using NamedType<int>::NamedType;
    };

    struct Y : NamedType<int> {
        using NamedType<int>::NamedType;
    };

    struct Height : NamedType<int> {
        using NamedType<int>::NamedType;
    };

    using Position = std::tuple<shipping::X, shipping::Y, shipping::Height>;

    struct BadShipOperationException {
        BadShipOperationException(X x, Y y, const std::string& msg) {
            std::cout << msg << " : X {" << x << "}, Y {" << y << "}\n";
        }
    };

    template<typename Container>
    using Grouping = std::unordered_map<std::string, std::function<std::string(const Container&)>>;


    template<typename Container>
    class Ship {
        X x;
        Y y;
        Height h;
        std::vector<std::tuple<X, Y, Height>> restrictions;
        Grouping<Container> groupingFunctions;
        vector<vector<Container>> stacks;
        vector<int> availableHeights;

        class GroupView {
            const std::unordered_map<Position, const Container &>* positionGroup = nullptr;
            using iterator_type = typename std::unordered_map<Position, const Container &>::const_iterator;
        public:
            GroupView(const std::unordered_map<Position, const Container &> &group) : positionGroup(&group) {}

            GroupView(int) {}

            auto begin() const {
                return positionGroup ? positionGroup->begin() : iterator_type{};
            }

            auto end() const {
                return positionGroup ? positionGroup->end() : iterator_type{};
            }
        };


        class PositionView {
            const std::vector<Container>* containers = nullptr;
            using iterator_type = typename std::vector<Container>::const_reverse_iterator;

        public:
            PositionView(const std::vector<Container>& containers) : containers(&containers) {}

            PositionView(int) {}

            auto begin() const {
                return containers ? containers->rbegin() : iterator_type{};
            }

            auto end() const {
                return containers ? containers->rend() : iterator_type{};
            }
        };


        class iterator {
            using PositionsItr =  typename std::vector<std::vector<Container>>::const_iterator;
            using ContainersItr =  typename std::vector<Container>::const_iterator;
            PositionsItr positionsItr;
            PositionsItr positionsEnd;
            ContainersItr containersItr;

            void setItrToOccupiedPosition() {
                while (positionsItr != positionsEnd && !(*positionsItr)) {
                    ++positionsItr;
                }
                containersItr = (*positionsItr)->begin();
            }

        public:
            iterator(PositionsItr containers_itr, PositionsItr containers_end)
                    : positionsItr(containers_itr), positionsEnd(containers_end) { setItrToOccupiedPosition(); }

            iterator operator++() {
                if (++containersItr == (*positionsItr).end()) {
                    // if reached the end then jump to next non-empty position
                    ++positionsItr;
                    setItrToOccupiedPosition();
                }
                return *this;
            }

            const Container &operator*() const {
                return *containersItr;
            }
            bool operator!=(iterator other) const {
                return positionsItr != other.positionsItr;
            }
        };


        using Pos2Container = std::unordered_map<Position, const Container&>;
        using Group = std::unordered_map<std::string, Pos2Container>;
        mutable std::unordered_map<std::string, Group> groups;

        Container& getTopContainerInPos(X xPos, Y yPos) {
            return stacks[posIndex(xPos, yPos)].back();
        }

        void addContainerToGroups(X xPos, Y yPos, Height height) {
            Container& container = getTopContainerInPos(xPos, yPos);
            for(auto& group_pair: groupingFunctions) {
                groups[group_pair.first][group_pair.second(container)].insert({std::tuple{xPos, yPos, height}, container } );
            }
        }
        void removeContainerFromGroups(X xPos, Y yPos, Height height) {
            Container& e = getTopContainerInPos(xPos, yPos);
            for(auto& group_pair: groupingFunctions) {
                groups[group_pair.first][group_pair.second(e)].erase(std::tuple{xPos, yPos, height});
            }
        }

    private:
        int posIndex(X xPos, Y yPos) const {
            if(xPos >= 0 && xPos < this->x && yPos >= 0 && yPos < this->y) {
                return yPos * this->x + xPos;
            }
            throw BadShipOperationException(xPos, yPos, "index out of range");
        }

        void addRestrictions(std::vector<std::tuple<X, Y, Height>> restrictionsParam){
            vector<int> availables (x*y,0);
            for (auto &rest : restrictionsParam) {
                X xPos = std::get<0>(rest);
                Y yPos = std::get<1>(rest);
                Height height = std::get<2>(rest);
                if (height < h) {
                    availables[posIndex(xPos, yPos)] = height;
                } else{
                    throw BadShipOperationException(xPos, yPos, "restriction with height=" + std::to_string(height) + " when original height is equal or smaller");
                }
            }
            this->availableHeights = std::move(availables);
        }

    public:

        Ship(X x, Y y, Height max_height,
             std::vector<std::tuple<X, Y, Height>> restrictions,
             Grouping<Container> groupingFunctions) noexcept(false) : Ship(x, y, max_height, restrictions) {
            this->groupingFunctions = groupingFunctions;
        }

        Ship(X x, Y y, Height max_height, std::vector<std::tuple<X, Y, Height>> restrictions) noexcept(false) : Ship(x, y, max_height) {
            addRestrictions(restrictions);
        }

        Ship(X x, Y y, Height max_height) noexcept : x(x), y(y), h(max_height), stacks(x*y) {
            availableHeights = vector<int>(x*y,0);
        };

        // region Managing Containers

        void load(X xPos, Y yPos, Container container) noexcept(false){
            auto& stack = stacks[posIndex(xPos, yPos)];
            if ((int)stack.size() < availableHeights[posIndex(xPos, yPos)]){
                stack.push_back(std::move(container));
                addContainerToGroups(xPos, yPos, Height{int(stack.size())});
            } else {
                throw BadShipOperationException(xPos,yPos,"out of space");
            }
        }

        Container unload(X xPos, Y yPos) noexcept(false){
            auto& stack = stacks[posIndex(xPos, yPos)];
            if(stack.empty()) { // if !stack
                throw BadShipOperationException(xPos, yPos, "no container to unload");
            }
            removeContainerFromGroups(xPos, yPos, Height{int(stack.size())-1});
            Container unloaded = stack.back();
            stack.pop_back();
            return unloaded;
        }

        void move(X from_x, Y from_y, X to_x, Y to_y) noexcept(false) {
            load(to_x, to_y, unload(from_x, from_y));
        }

        // endregion

        // region Get View

        PositionView getContainersViewByPosition(X xPos, Y yPos) const {
            auto& stack = stacks[posIndex(xPos, yPos)];
            return stack.begin();
        }

        GroupView getContainersViewByGroup(const std::string& groupingName, const std::string& groupName) const {
            auto itr = groups.find(groupingName);
            if(itr == groups.end() && groupingFunctions.find(groupingName) != groupingFunctions.end()) {
                // C++17 auto tuple unpack
                auto [insert_itr, _] = groups.insert({groupingName, Group{}});
                itr = insert_itr;
            }
            if(itr != groups.end()) {
                const auto& grouping = itr->second;
                auto itr2 = grouping.find(groupName);
                if(itr2 == grouping.end()) {
                    // C++17 auto tuple unpack
                    auto [insert_itr, _] = itr->second.insert({groupName, Pos2Container{}});
                    itr2 = insert_itr;
                }
                return GroupView { itr2->second };
            }
            return GroupView { 0 };
        }

        // endregion

        iterator begin() const {
            return {stacks.begin(), stacks.end() };
        }
        iterator end() const {
            return {stacks.end(), stacks.end() };
        }

        Ship(const Ship&) = delete;
        Ship& operator=(const Ship&) = delete;
        Ship(Ship&&) = default;
        Ship& operator=(Ship&&) = default;
        //-------------------------------------------------------
    };
}

namespace std
{
    template<> struct hash<shipping::Position>
    {
        std::size_t operator()(const shipping::Position& pos) const noexcept
        {
            return std::get<0>(pos) ^ (std::get<1>(pos) << 1) + std::get<2>(pos);
        }
    };
}

#endif //CPP_STOWAGE_SHIP_H
