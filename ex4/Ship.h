// ozzafar 206039984

#ifndef CPP_STOWAGE_SHIP_H
#define CPP_STOWAGE_SHIP_H

#include <vector>
#include <set>
#include <unordered_map>
#include <tuple>
#include <functional>
#include <string>
#include <iostream>

using std::string;
using std::vector;
using std::set;

/// --------------------- Ship class foe ex4 !! ---------------------

namespace shipping {

    // region NamedType

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

    // endregion

    using Position = std::tuple<shipping::X, shipping::Y, shipping::Height>;

    /// Exception
    struct BadShipOperationException {
        string msg;
        BadShipOperationException(X x, Y y, const std::string& msg) {
            this->msg = msg + " : X {" + std::to_string(x) + "}, Y {" + std::to_string(y) + "}\n";
        }

        const string getMsg() {
            return msg;
        }
    };

    template<typename Container>
    using Grouping = std::unordered_map<std::string, std::function<std::string(const Container&)>>;


    template<typename Container>
    class Ship {
        X x; Y y; Height h;
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

            PositionView() {}

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
                while (positionsItr != positionsEnd && (*positionsItr).empty()) {
                    ++positionsItr;
                }
                containersItr = (*positionsItr).begin();
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

        /// get upper container in position
        Container& getTopContainerInPos(X xPos, Y yPos) {
            return getStack(xPos,yPos).back();
        }

        void addContainerToGroups(X xPos, Y yPos, Height height) {
            Container& container = getTopContainerInPos(xPos, yPos);
            for(auto& group_pair: groupingFunctions) {
                groups[group_pair.first][group_pair.second(container)].insert({std::tuple{xPos, yPos, height}, container});
            }
        }
        void removeContainerFromGroups(X xPos, Y yPos, Height height) {
            Container& container = getTopContainerInPos(xPos, yPos);
            for(auto& group_pair: groupingFunctions) {
                groups[group_pair.first][group_pair.second(container)].erase(std::tuple{xPos, yPos, height});
            }
        }

    private:

        /// check if there is available place to load in position
        bool hasPlaceInPos(X xPos, Y yPos){
            int pos = posIndex(xPos, yPos);
            auto& stack = stacks[pos];
            return (int)stack.size() < availableHeights[pos];
        }

        /// calculate 1d position from 2d position
        int posIndex(X xPos, Y yPos) const {
            if(xPos >= 0 && xPos < this->x && yPos >= 0 && yPos < this->y) {
                return yPos * this->x + xPos;
            }
            throw BadShipOperationException(xPos, yPos, "index out of range");
        }

        /// validates and update restrictions
        void addRestrictions(std::vector<std::tuple<X, Y, Height>> restrictionsParam, Height maxHeight){
            vector<int> availables(x * y, maxHeight);
            set<std::tuple<shipping::X, shipping::Y>> positions;
            for (auto &rest : restrictionsParam) {
                X xPos = std::get<0>(rest);
                Y yPos = std::get<1>(rest);
                if (positions.find({xPos, yPos}) != positions.end()) {
                    throw BadShipOperationException(xPos, yPos, "duplicate restriction of this position");
                }
                positions.insert({xPos, yPos});

                Height height = std::get<2>(rest);
                if (height < h) {
                    availables[posIndex(xPos, yPos)] = height;
                } else{
                    throw BadShipOperationException(xPos, yPos, "restriction with height=" + std::to_string(height) + " when original height is equal or smaller");
                }
            }
            this->availableHeights = std::move(availables);
        }

        /// get stack of containers in specific position
        vector<Container>& getStack(X xPos, Y yPos){
            return stacks[posIndex(xPos, yPos)];
        }

        /// const version of above
        const vector<Container>& getStack(X xPos, Y yPos) const {
            return stacks[posIndex(xPos, yPos)];
        }

    public:

        Ship(X x, Y y, Height max_height, std::vector<std::tuple<X, Y, Height>> restrictions, Grouping<Container> groupingFunctions) noexcept(false) : Ship(x, y, max_height, restrictions) {
            this->groupingFunctions = groupingFunctions;
        }

        Ship(X x, Y y, Height max_height, std::vector<std::tuple<X, Y, Height>> restrictions) noexcept(false) : Ship(x, y, max_height) {
            addRestrictions(restrictions, max_height);
        }

        Ship(X x, Y y, Height max_height) noexcept : x(x), y(y), h(max_height), stacks(x*y) {
            availableHeights = vector<int>(x * y, max_height);
            for (auto& stack: stacks) {
                // reserve in order to fix reference invalidations
                stack.reserve(max_height + 1);
            }
        };

        // region Managing Containers

        void load(X xPos, Y yPos, Container container) noexcept(false){
            auto& stack = getStack(xPos, yPos);
            if ((int)stack.size() < availableHeights[posIndex(xPos, yPos)]){
                // there is available place for loading
                stack.push_back(container);
                addContainerToGroups(xPos, yPos, Height{int(stack.size()-1)});
            } else {
                throw BadShipOperationException(xPos,yPos,"out of space");
            }
        }

        Container unload(X xPos, Y yPos) noexcept(false){
            auto& stack = getStack(xPos, yPos);
            if(stack.empty()) { // if !stack
                throw BadShipOperationException(xPos, yPos, "no container to unload");
            }
            removeContainerFromGroups(xPos, yPos, Height{int(stack.size())-1});
            // now can pop container
            Container unloaded = stack.back();
            stack.pop_back();
            return unloaded;
        }

        void move(X from_x, Y from_y, X to_x, Y to_y) noexcept(false){

            // validation
            posIndex(from_x, from_y);
            posIndex(to_x, to_y);

            if (getStack(from_x, from_y).empty()) {
                throw BadShipOperationException(from_x, from_y, "can't move from empty position");
            }
            if (from_x != to_x || from_y != to_y) { // positions are not equal
                if (hasPlaceInPos(to_x, to_y)) {
                    load(to_x, to_y, unload(from_x, from_y));
                } else {
                    throw BadShipOperationException(to_x, to_y, "out of space");
                }
            }
        }

        // endregion

        // region Get View

        PositionView getContainersViewByPosition(X xPos, Y yPos) const {
            try {
                auto &stack = getStack(xPos, yPos);
                return PositionView(stack);
            } catch(BadShipOperationException& e) {
                return PositionView();
            }
        }

        GroupView getContainersViewByGroup(const std::string& groupingName, const std::string& groupName) const {
            auto groupItr = groups.find(groupingName);
            if(groupItr == groups.end() && groupingFunctions.find(groupingName) != groupingFunctions.end()) {
                // C++17 auto tuple unpack
                auto [insert_itr, _] = groups.insert({groupingName, Group{}});
                groupItr = insert_itr;
            }
            if(groupItr != groups.end()) {
                const auto& grouping = groupItr->second;
                auto groupNameItr = grouping.find(groupName);
                if(groupNameItr == grouping.end()) {
                    // creating the group
                    auto [insert_itr, _] = groupItr->second.insert({groupName, Pos2Container{}});
                    groupNameItr = insert_itr;
                }
                return GroupView {groupNameItr->second };
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
    /// hash function in order to insert Position to hash table (unordered map)
    template<> struct hash<shipping::Position>
    {
        std::size_t operator()(const shipping::Position& pos) const noexcept
        {
            return std::get<0>(pos) ^ (std::get<1>(pos) << 1) + std::get<2>(pos);
        }
    };
}

#endif //CPP_STOWAGE_SHIP_H
