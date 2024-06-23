#ifndef A_STAR
#define A_STAR

#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <unordered_set>
#include <cmath>
#include "area.hpp"


template <size_t square_size>
size_t manhattan_distance(const Square<square_size>& obj1, const Square<square_size>& obj2) {
    return std::abs(obj1.getCenter().first - obj2.getCenter().first) + std::abs(obj1.getCenter().second - obj2.getCenter().second);
}

template <size_t square_size>
size_t distance(const Square<square_size>& obj1, const Square<square_size>& obj2) {
    return std::sqrt(std::pow((obj2.getCenter().first - obj1.getCenter().first), 2) + std::pow((obj2.getCenter().second - obj1.getCenter().second), 2));
}

template <size_t square_size>
size_t calculate_gcost(const Square<square_size>& current, const Square<square_size>& neighbor) {
    size_t dx = std::abs(current.getCenter().first - neighbor.getCenter().first);
    size_t dy = std::abs(current.getCenter().second - neighbor.getCenter().second);

    if(dx != 0 && dy != 0) {
        return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
    }
    else {
        return dx+dy;
    }    
}

template <size_t square_side>
size_t calculate_total_cost(Square<square_side>* obj) {    
    return obj->getG();
}


template <size_t square_size>
class Compare {
    public:
        bool operator()(Square<square_size>* obj1, Square<square_size>* obj2) {
            return obj1->getF() > obj2->getF();
        }
};

template <size_t square_size>
Square<square_size>* a_search(Area<square_size>& obj, Square<square_size>* start_square, Square<square_size>* end_square) {
    
    std::priority_queue<Square<square_size>*, std::vector<Square<square_size>*>, Compare<square_size>> open_list;
    std::unordered_set<Square<square_size>*> closed_list;


    if(!obj.SquareExist(start_square)) {
        throw std::logic_error("Wrong start!");
    }

    if(!obj.SquareExist(end_square)) {
        throw std::logic_error("Wrong end!");
    }

    open_list.push(start_square);

    while(!open_list.empty()) {
        auto* current = open_list.top();
        open_list.pop();
        closed_list.insert(current);

        if(current == end_square) {
            return current;
        }

        for(auto* square: obj.getNeighbors(*current)) {            
 
            bool inOpen = square->getF();
                
            size_t h = manhattan_distance(*square, *end_square);       
            size_t g = current->getG() + calculate_gcost(*current, *square);
            
            if(closed_list.find(square) != closed_list.end()) {
                continue;
            }


            if(h+g < square->getF() || !inOpen) {
                
                square->setG(g);
                square->setF(h);

                square->setParent(current);
                if(!inOpen) {
                    open_list.push(square);
                }
            }
        

        }
    }   
    return nullptr;
}

#endif