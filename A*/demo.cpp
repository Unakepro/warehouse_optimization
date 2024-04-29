#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <unordered_set>
#include <cmath>
#include "A_star.hpp"

int main() {
    Area<2> xs(5, 8);

    auto* start = xs.getSquare(0, 0);
    auto* end = xs.getSquare(0, 7);

    xs.getSquare(0, 5)->Block();
    xs.getSquare(1, 5)->Block();

    xs.getSquare(1, 1)->Block();
    xs.getSquare(1, 2)->Block();
    xs.getSquare(1, 3)->Block();

    Square<2>* result = a_search<2>(xs, start, end);

    while(result) {
        std::cout << result->getCenter().first << ' ' << result->getCenter().second;
        result = result->getParent();
        std::cout << '\n';
    }
} 