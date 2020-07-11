//
// Created by Oz Zafar on 11/04/2020.
//

//
// Created by Oz Zafar on 10/04/2020.
//
#include "Ship.h"

#include "gtest/gtest.h"

// region Container tests
using namespace shipping;

namespace {

    TEST(Ship, BadShipOperationException_expected_duplicate_restrictions) {
        // exception due duplicate restrictions (whether or not it has same limit):
        // restriction with X{2}, Y{5} appears more than once

        // create restrictions
        std::vector<std::tuple<X, Y, Height>> restrictions = {
                std::tuple(X{2}, Y{6}, Height{0}),
                std::tuple(X{2}, Y{7}, Height{1}),
                std::tuple(X{2}, Y{5}, Height{6}),
        };

        // create bad ship 1:
        // exception due duplicate restrictions (whether or not it has same limit):
        // restriction with X{2}, Y{5} appears more than once
        restrictions.push_back( std::tuple(X{2}, Y{5}, Height{6}) );

        ASSERT_THROW( { Ship<std::string> myShip( X{4}, Y{12}, Height{16}, restrictions ); }, BadShipOperationException);
    }

    TEST(Ship, BadShipOperationException_expected_bad_load_no_room_after_good_load) {
        // test good load - no exception
        // test bad load after good load - no room

        // create restrictions
        std::vector<std::tuple<X, Y, Height>> restrictions = {
                std::tuple(X{2}, Y{6}, Height{0}),
                std::tuple(X{2}, Y{7}, Height{1}),
                std::tuple(X{2}, Y{5}, Height{6}),
        };

        // create good ship
        Ship<std::string> myShip{ X{4}, Y{8}, Height{8}, restrictions };

        // good load:
        ASSERT_NO_THROW( myShip.load(X{2}, Y{7}, "Hello") );

        // bad load - no room:
        ASSERT_THROW( { myShip.load(X{2}, Y{7}, "Goodbye"); }, BadShipOperationException );
    }

    TEST(Ship, simple_ship_content) {

        // create grouping pairs
        Grouping<std::string> groupingFunctions = {
                { "first_letter",
                        [](const std::string& s){ return std::string(1, s[0]); }
                },
                { "first_letter_toupper",
                        [](const std::string& s){ return std::string(1, char(std::toupper(s[0]))); }
                }
        };

        // create restrictions
        std::vector<std::tuple<X, Y, Height>> restrictions = {
                std::tuple(X{2}, Y{6}, Height{4}),
                std::tuple(X{2}, Y{7}, Height{6}),
        };

        // create ship
        Ship<std::string> myShip{ X{5}, Y{12}, Height{8},
                                  restrictions, groupingFunctions };

        // load “containers”
        myShip.load(X{0}, Y{0}, "Hello");
        myShip.load(X{1}, Y{1}, "hey");
        myShip.load(X{1}, Y{1}, "bye");
        myShip.load(X{0}, Y{0}, "hi");

        // loop on all “containers” - Hello, hi, hey, bye - in some undefined order
        // expected: Hello, hi, hey, bye - in some undefined order
        std::set<std::string> expectedSet_myShip{"Hello", "hi", "hey", "bye"};
        std::set<std::string> set_myShip;
        int numElements = 0;
        for(const auto& container : myShip) {
            set_myShip.insert(container);
            ++numElements;
        }

        EXPECT_EQ( set_myShip, expectedSet_myShip );
        EXPECT_EQ( numElements, expectedSet_myShip.size() );
    }

    using ShipSetOfData = std::set< std::tuple<X, Y, Height, std::string> >;

    TEST(Ship, View_getContainersViewByGroup) {

        // create grouping pairs
        Grouping<std::string> groupingFunctions = {
                { "first_letter",
                        [](const std::string& s){ return std::string(1, s[0]); }
                },
                { "first_letter_toupper",
                        [](const std::string& s){ return std::string(1, char(std::toupper(s[0]))); }
                }
        };

        // create restrictions
        std::vector<std::tuple<X, Y, Height>> restrictions = {
                std::tuple(X{2}, Y{6}, Height{4}),
                std::tuple(X{2}, Y{7}, Height{6}),
        };

        // create ship
        Ship<std::string> myShip{ X{5}, Y{12}, Height{8},
                                  restrictions, groupingFunctions };

        // load “containers”
        myShip.load(X{0}, Y{0}, "Hello");
        myShip.load(X{1}, Y{1}, "hey");
        myShip.load(X{1}, Y{1}, "bye");

        auto view_h = myShip.getContainersViewByGroup("first_letter", "h");

        myShip.load(X{0}, Y{0}, "hi");

        // loop on view_h:	{X{0}, Y{0}, Height{1}, hi},
        // 					{X{1}, Y{1}, Height{0}, hey}
        // - in some undefined order

        ShipSetOfData expectedSetProjectedBy_view_h{
                {X{0}, Y{0}, Height{1}, "hi"},
                {X{1}, Y{1}, Height{0}, "hey"}
        };
        ShipSetOfData setProjectedBy_view_h;

        int numElement = 0;
        for(const auto& container_data : view_h) {
            const std::tuple<X, Y, Height>& container_tuple = container_data.first;
            ++numElement;
            setProjectedBy_view_h.emplace(std::get<0>(container_tuple), std::get<1>(container_tuple), std::get<2>(container_tuple), container_data.second);
        }

        EXPECT_EQ( expectedSetProjectedBy_view_h, setProjectedBy_view_h );
        EXPECT_EQ( numElement, expectedSetProjectedBy_view_h.size() );
    }

    TEST(Ship, View_getContainersViewByGroup_initialize_empty) {

        // create grouping pairs
        Grouping<std::string> groupingFunctions = {
                { "first_letter",
                        [](const std::string& s){ return std::string(1, s[0]); }
                },
                { "first_letter_toupper",
                        [](const std::string& s){ return std::string(1, char(std::toupper(s[0]))); }
                }
        };

        // create restrictions
        std::vector<std::tuple<X, Y, Height>> restrictions = {
                std::tuple(X{2}, Y{6}, Height{4}),
                std::tuple(X{2}, Y{7}, Height{6}),
        };

        // create ship
        Ship<std::string> myShip{ X{5}, Y{12}, Height{8},
                                  restrictions, groupingFunctions };

        auto view_h = myShip.getContainersViewByGroup("first_letter", "h");

        // load “containers”
        myShip.load(X{1}, Y{1}, "hey");
        myShip.load(X{1}, Y{1}, "bye");
        myShip.load(X{0}, Y{0}, "Hello");
        myShip.load(X{0}, Y{0}, "hi");

        // loop on view_h:	{X{0}, Y{0}, Height{1}, hi},
        // 					{X{1}, Y{1}, Height{0}, hey}
        // - in some undefined order

        ShipSetOfData expectedSetProjectedBy_view_h{
                {X{0}, Y{0}, Height{1}, "hi"},
                {X{1}, Y{1}, Height{0}, "hey"}
        };
        ShipSetOfData setProjectedBy_view_h;

        int numElements = 0;
        for(const auto& container_data : view_h) {
            const std::tuple<X, Y, Height>& container_tuple = container_data.first;
            ++numElements;
            setProjectedBy_view_h.emplace(std::get<0>(container_tuple), std::get<1>(container_tuple), std::get<2>(container_tuple), container_data.second);
        }

        EXPECT_EQ( expectedSetProjectedBy_view_h, setProjectedBy_view_h );
        EXPECT_EQ( numElements, expectedSetProjectedBy_view_h.size() );
    }

    template<typename ShipType>
    void test_copy_constructor() {

        // create grouping pairs
        Grouping<std::string> groupingFunctions = {
                { "first_letter",
                        [](const std::string& s){ return std::string(1, s[0]); }
                },
        };

        // create restrictions
        std::vector<std::tuple<X, Y, Height>> restrictions = {
                std::tuple(X{2}, Y{6}, Height{4}),
                std::tuple(X{2}, Y{7}, Height{6}),
        };

        // create ship
        ShipType myShip{ X{5}, Y{12}, Height{8},
                         restrictions, groupingFunctions };

        // load “containers”
        myShip.load(X{1}, Y{1}, "hey");

        // copy ship
        ShipType myShip2(myShip);

        // change original ship
        myShip.unload(X{1}, Y{1});
        myShip.load(X{1}, Y{1}, "hye");

        // get view of new ship
        auto view_h = myShip2.getContainersViewByGroup("first_letter", "h");

        auto& from_ship = *myShip2.begin();
        auto& from_view = view_h.begin()->second;

        // test that view is indeed of new ship
        EXPECT_EQ( from_ship, from_view );

        // test container is of reference
        EXPECT_EQ( &from_ship, &from_view );
    }

    TEST(Ship, CopyConstructor) {

        // test only if copy constructible
        if constexpr(!std::is_copy_constructible_v<Ship<std::string>>) {
        }
        else {
            test_copy_constructor<Ship<std::string>>();
        }
    }



}

