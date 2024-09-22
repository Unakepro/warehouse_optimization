
#ifndef SQUARE
#define SQUARE

#include <iostream>

template <size_t side>
class Square {
    std::pair<double, double> center;
    bool blocked = 0;
    

public:
    Square(std::pair<double, double> center, bool blocked = 0): center(center), blocked(blocked) {}

    std::pair<double, double> getCenter() const {
        return center;
    }

    bool isBlocked() const {
        return blocked;
    }

    void Block() {
        blocked = 1;
    }

    void Unlock() {
        blocked = 0;
    }

    bool operator==(const Square<side>& obj) const {
        return center.first == obj.center.first && center.second == obj.center.second;
    }

};

template <size_t side>
double euclidian_distance(Square<side>* obj1, Square<side>* obj2) {
    return std::sqrt(std::pow(obj1->getCenter().first - obj2->getCenter().first, 2) + std::pow(obj1->getCenter().second - obj2->getCenter().second, 2));
}

#endif