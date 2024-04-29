#ifndef AREA
#define AREA

#include <iostream>
#include <vector>
#include <cmath>
#include "square.hpp"

template <size_t square_side>
class Area {
    std::vector<Square<square_side>*> values;
    size_t width = 0;
    size_t height = 0;

public:
    Area(size_t width, size_t height): width(width), height(height) {
        for(size_t i = 0; i < height; ++i) {
            for(size_t j = 0; j < width; ++j) {
                values.push_back(new Square<square_side>(std::pair<double, double>((j*square_side) + square_side/2.0, (i*square_side) + square_side/2.0)));
            }
        }              
    }

    size_t getWidth() const {
        return width;
    }

    size_t getHeight() const {
        return height;
    }
    
    Square<square_side>* getSquare(size_t i, size_t j) {
        return values[j * width + i];
    }

    Square<square_side>* getSquareByCenter(double i, double j) {
        return getSquare((i-square_side/2.0)/square_side, (j-square_side/2.0)/square_side);
    }

    std::vector<Square<square_side>*> getSquares(const std::pair<size_t, size_t>& obj1, const std::pair<size_t, size_t>& obj2) {
        std::vector<Square<square_side>*> squares;
        std::pair<size_t, size_t> counter;

        counter.first = obj1.first;
        counter.second = obj1.second;

        if(obj2.first < obj1.first || obj2.second < obj1.second) {
            throw std::logic_error("Wrong coordinates");
        }


        while (counter.second <= obj2.second) {
            squares.push_back(getSquare(counter.first, counter.second));
            
            if(counter.first == obj2.first) {
                ++counter.second;
                counter.first = obj1.first;
                continue;
            }
            ++counter.first;
            
        }
        return squares;
    }
    

    bool SquareExist(const std::pair<double, double>& center) const {
        if(center.first < square_side/2.0 || center.first > width*square_side-square_side/2.0 || center.second < square_side/2.0 || center.second > height*square_side-square_side/2.0) {
            return 0;
        }
        return 1;
    }

    bool SquareExist(Square<square_side>* obj) const {
        std::pair<double, double> center;

        center = obj->getCenter();
        if(center.first < square_side/2.0 || center.first > width*square_side-square_side/2.0 || center.second < square_side/2.0 || center.second > height*square_side-square_side/2.0) {
            return 0;
        }
        
        return 1;
    }

    
    void add_neighbor(std::vector<Square<square_side>*>& square_values, const std::pair<double, double>& position) {
        if(SquareExist(position) && !(getSquareByCenter(position.first, position.second)->isBlocked())) {
            square_values.push_back(getSquareByCenter(position.first, position.second));
        }
    }


    std::vector<Square<square_side>*> getNeighbors(const Square<square_side>& current) {
        std::vector<Square<square_side>*> square_values;
        std::pair<double, double> neighbor;
        
        neighbor = getUpperCenter(current);
        add_neighbor(square_values, neighbor);

        neighbor = getUpperRightCenter(current);
        add_neighbor(square_values, neighbor);

        neighbor = getRightCenter(current);
        add_neighbor(square_values, neighbor);

        neighbor = getBottomRightCenter(current);
        add_neighbor(square_values, neighbor);

        neighbor = getBottomCenter(current);
        add_neighbor(square_values, neighbor);
   
        neighbor = getBottomLeftCenter(current);
        add_neighbor(square_values, neighbor);

        neighbor = getLeftCenter(current);
        add_neighbor(square_values, neighbor);

        neighbor = getUpperLeftCenter(current);
        add_neighbor(square_values, neighbor);


        return square_values;
    }



    static std::pair<double, double> getUpperCenter(const Square<square_side>& obj) {
        return std::pair<double, double>(obj.getCenter().first, obj.getCenter().second+square_side);
    }

    static std::pair<double, double> getUpperRightCenter(const Square<square_side>& obj) {
        return std::pair<double, double>(obj.getCenter().first+square_side, obj.getCenter().second+square_side);
    }

    static std::pair<double, double> getRightCenter(const Square<square_side>& obj) { 
        return std::pair<double, double>(obj.getCenter().first+square_side, obj.getCenter().second);
    }

    static std::pair<double, double> getBottomRightCenter(const Square<square_side>& obj) { 
        return std::pair<double, double>(obj.getCenter().first+square_side, obj.getCenter().second-square_side);
    }

    static std::pair<double, double> getBottomCenter(const Square<square_side>& obj) {
        return std::pair<double, double>(obj.getCenter().first, obj.getCenter().second-square_side);
    }

    static std::pair<double, double> getBottomLeftCenter(const Square<square_side>& obj) {
        return std::pair<double, double>(obj.getCenter().first-square_side, obj.getCenter().second-square_side);
    }

    static std::pair<double, double> getLeftCenter(const Square<square_side>& obj) { 
        return std::pair<double, double>(obj.getCenter().first-square_side, obj.getCenter().second);
    }

    static std::pair<double, double> getUpperLeftCenter(const Square<square_side>& obj) { 
        return std::pair<double, double>(obj.getCenter().first-square_side, obj.getCenter().second+square_side);
    }
    
    void reset_squares() {
        for(size_t i = 0; i < values.size(); ++i) {
            values[i]->reset();
        }
    }

    void print() {
        for(auto* obj: values) {

            std::cout << obj << '\n';
        }
    }

};

#endif