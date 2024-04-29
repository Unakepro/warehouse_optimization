
#ifndef SQUARE
#define SQUARE

#include <iostream>

template <size_t side>
class Square {
    std::pair<double, double> center;
    std::pair<size_t, size_t> gf_cost{0, 0};
    bool blocked = 0;
    
    Square* parent = nullptr;


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
    

    void setParent(Square* ptr) {
        parent = ptr;
    }

    void setF(size_t h) {
        gf_cost.second = gf_cost.first + h;
    }
    
    void setG(size_t g) {
        gf_cost.first = g;
    }

    Square<side>* getParent() {
        return parent;
    }

    size_t getG() {
        return gf_cost.first;
    }

    size_t getF() {
        return gf_cost.second;
    }

    bool operator==(const Square<side>& obj) const {
        return center.first == obj.center.first && center.second == obj.center.second;
    }

    void reset() {
        gf_cost.first = 0;
        gf_cost.second = 0;
        parent = nullptr;
    }
};

#endif