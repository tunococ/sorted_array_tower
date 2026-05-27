#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#ifdef SAT_USE_MODULES
import sorted_array_tower;
#else
#include "sorted_array_tower/sorted_array_tower.hpp"
#endif

using namespace sorted_array_tower;

TEST_SUITE("SortedArrayTower Basic Operations") {
    TEST_CASE("Default construction creates empty array") {
        SortedArrayTower<int> arr;
        CHECK(arr.empty());
        CHECK(arr.size() == 0);
    }

    TEST_CASE("Initializer list construction") {
        SortedArrayTower<int> arr{5, 2, 8, 1, 9};
        CHECK(arr.size() == 5);
        CHECK(arr.at(0) == 1);
        CHECK(arr.at(1) == 2);
        CHECK(arr.at(2) == 5);
        CHECK(arr.at(3) == 8);
        CHECK(arr.at(4) == 9);
    }

    TEST_CASE("Insert maintains sorted order") {
        SortedArrayTower<int> arr;
        arr.insert(5);
        arr.insert(2);
        arr.insert(8);
        
        CHECK(arr.size() == 3);
        CHECK(arr.at(0) == 2);
        CHECK(arr.at(1) == 5);
        CHECK(arr.at(2) == 8);
    }

    TEST_CASE("Insert with duplicate values") {
        SortedArrayTower<int> arr;
        arr.insert(5);
        arr.insert(5);
        arr.insert(3);
        
        CHECK(arr.size() == 3);
        CHECK(arr.at(0) == 3);
        CHECK(arr.at(1) == 5);
        CHECK(arr.at(2) == 5);
    }

    TEST_CASE("Insert with move semantics") {
        SortedArrayTower<std::string> arr;
        arr.insert(std::string("world"));
        arr.insert(std::string("hello"));
        
        CHECK(arr.size() == 2);
        CHECK(arr.at(0) == "hello");
        CHECK(arr.at(1) == "world");
    }
}

TEST_SUITE("SortedArrayTower Search Operations") {
    TEST_CASE("Find existing element") {
        SortedArrayTower<int> arr{1, 3, 5, 7, 9};
        
        auto it = arr.find(5);
        CHECK(it != arr.end());
        CHECK(*it == 5);
    }

    TEST_CASE("Find non-existing element") {
        SortedArrayTower<int> arr{1, 3, 5, 7, 9};
        
        auto it = arr.find(4);
        CHECK(it == arr.end());
    }

    TEST_CASE("Contains existing element") {
        SortedArrayTower<int> arr{1, 3, 5, 7, 9};
        
        CHECK(arr.contains(1));
        CHECK(arr.contains(5));
        CHECK(arr.contains(9));
    }

    TEST_CASE("Contains non-existing element") {
        SortedArrayTower<int> arr{1, 3, 5, 7, 9};
        
        CHECK_FALSE(arr.contains(2));
        CHECK_FALSE(arr.contains(10));
    }
}

TEST_SUITE("SortedArrayTower Deletion Operations") {
    TEST_CASE("Remove existing element") {
        SortedArrayTower<int> arr{1, 3, 5, 7, 9};
        
        bool removed = arr.remove(5);
        CHECK(removed);
        CHECK(arr.size() == 4);
        CHECK_FALSE(arr.contains(5));
    }

    TEST_CASE("Remove non-existing element") {
        SortedArrayTower<int> arr{1, 3, 5, 7, 9};
        
        bool removed = arr.remove(4);
        CHECK_FALSE(removed);
        CHECK(arr.size() == 5);
    }

    TEST_CASE("Remove first occurrence of duplicate") {
        SortedArrayTower<int> arr{1, 3, 5, 5, 9};
        
        bool removed = arr.remove(5);
        CHECK(removed);
        CHECK(arr.size() == 4);
        CHECK(arr.contains(5)); // Second 5 should still exist
    }

    TEST_CASE("Clear array") {
        SortedArrayTower<int> arr{1, 3, 5, 7, 9};
        arr.clear();
        
        CHECK(arr.empty());
        CHECK(arr.size() == 0);
    }
}

TEST_SUITE("SortedArrayTower Min/Max Operations") {
    TEST_CASE("Get minimum element") {
        SortedArrayTower<int> arr{5, 2, 8, 1, 9};
        CHECK(arr.min() == 1);
    }

    TEST_CASE("Get maximum element") {
        SortedArrayTower<int> arr{5, 2, 8, 1, 9};
        CHECK(arr.max() == 9);
    }

    TEST_CASE("Min/Max on single element") {
        SortedArrayTower<int> arr{42};
        CHECK(arr.min() == 42);
        CHECK(arr.max() == 42);
    }

    TEST_CASE("Min/Max throws on empty array") {
        SortedArrayTower<int> arr;
        
        CHECK_THROWS_AS(arr.min(), std::out_of_range);
        CHECK_THROWS_AS(arr.max(), std::out_of_range);
    }
}

TEST_SUITE("SortedArrayTower Iteration") {
    TEST_CASE("Iterate through elements") {
        SortedArrayTower<int> arr{5, 2, 8, 1, 9};
        std::vector<int> collected;
        
        for (const auto& elem : arr) {
            collected.push_back(elem);
        }
        
        CHECK(collected == std::vector<int>{1, 2, 5, 8, 9});
    }

    TEST_CASE("cbegin and cend") {
        SortedArrayTower<int> arr{3, 1, 2};
        
        auto it = arr.cbegin();
        CHECK(*it == 1);
        CHECK(std::next(it) != arr.cend());
    }
}

TEST_SUITE("SortedArrayTower Floating Point") {
    TEST_CASE("Works with floating point numbers") {
        SortedArrayTower<double> arr{3.14, 2.71, 1.41, 1.73};
        
        CHECK(arr.size() == 4);
        CHECK(arr.at(0) == doctest::Approx(1.41));
        CHECK(arr.at(1) == doctest::Approx(1.73));
        CHECK(arr.at(2) == doctest::Approx(2.71));
        CHECK(arr.at(3) == doctest::Approx(3.14));
    }
}

TEST_SUITE("SortedArrayTower Edge Cases") {
    TEST_CASE("At with out of range index") {
        SortedArrayTower<int> arr{1, 2, 3};
        
        CHECK_THROWS_AS(arr.at(3), std::out_of_range);
        CHECK_THROWS_AS(arr.at(10), std::out_of_range);
    }

    TEST_CASE("Many insertions maintain order") {
        SortedArrayTower<int> arr;
        
        for (int i = 100; i > 0; --i) {
            arr.insert(i);
        }
        
        CHECK(arr.size() == 100);
        CHECK(arr.at(0) == 1);
        CHECK(arr.at(50) == 51);
        CHECK(arr.at(99) == 100);
    }
}
