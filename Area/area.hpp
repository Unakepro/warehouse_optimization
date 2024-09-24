#ifndef AREA
#define AREA

#include <iostream>
#include <vector>
#include <cmath>
#include "square.hpp"

template <size_t square_side>
class Area {
    size_t width = 0;
    size_t height = 0;
    std::pair<size_t, size_t> start{0, 0};

    std::vector<Square<square_side>*> values;
    std::unordered_map<std::string, Square<square_side>*> positions;

public:
    Area(size_t width, size_t height, std::pair<size_t, size_t> start = {0, 0}): width(width), height(height), start(start), values(width*height, nullptr) {
        values[start.first + start.second * width] = new Square<square_side>(getCoords(start.first, start.second));
    }

    size_t getWidth() const {
        return width;
    }

    size_t getHeight() const {
        return height;
    }

    std::pair<double, double> getCoords(size_t i, size_t j) {
        return std::pair<double, double>((j*square_side) + square_side/2.0, (i*square_side) + square_side/2.0);
    }
    
    Square<square_side>* getSquare(size_t i, size_t j) {
        return values[j * width + i];
    }

    Square<square_side>* getSquareByCenter(double i, double j) {
        return getSquare((i-square_side/2.0)/square_side, (j-square_side/2.0)/square_side);
    }

    void setSquares(const std::pair<size_t, size_t>& obj1, const std::pair<size_t, size_t>& obj2) {
        std::pair<size_t, size_t> indexes;

        indexes.first = obj1.first;
        indexes.second = obj1.second;

        if(obj2.first < obj1.first || obj2.second < obj1.second) {
            throw std::logic_error("Wrong coordinates");
        }


        while (indexes.second <= obj2.second) {
            values[indexes.first + indexes.second * width] = new Square<square_side>(getCoords(indexes.first, indexes.second));
            
            if(indexes.first == obj2.first) {
                ++indexes.second;
                indexes.first = obj1.first;
                continue;
            }
            ++indexes.first;
            
        }
    }

    void setClosest(std::vector<std::string>& items, size_t items_per_shelf) {
        std::vector<Square<square_side>*> sorted_values;
        for(size_t i = 0; i < values.size(); ++i) {
            if(values[i] && i != (start.first + start.second * width)) {
                sorted_values.push_back(values[i]);
            }
        }

        std::sort(sorted_values.begin(), sorted_values.end(), [this](Square<square_side>* lhs, Square<square_side>* rhs) {
            return euclidian_distance(lhs, values[start.first + start.second * width]) < euclidian_distance(rhs, values[start.first + start.second * width]);
        });
        
        size_t j = 0;
        for(size_t i = 0; i < items.size(); ++i) {
            if(i % items_per_shelf == 0 && i != 0) {
                ++j;
            }

            positions.emplace(items[i], sorted_values[j]);
        }

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

    double total_cost(std::vector<std::string>& transactions) {
        double curr_cost = 0;

        Square<square_side>* current = values[start.first + start.second * width];
        for(const auto& item: transactions) {
            auto item_it = positions.find(item);

            if(item_it != positions.end()) {
                curr_cost += euclidian_distance(current, item_it->second);
                current = item_it->second;
            }
            else {
                throw std::logic_error("Not found");
            }
        }

        curr_cost += euclidian_distance(current, values[start.first + start.second * width]);

        return curr_cost;
    }

    Area<square_side>& operator=(Area<square_side>& obj) {
        for(size_t i = 0; i < values.size(); ++i) {
            delete values[i];
        }

        width = obj.width;
        height = obj.height;
        start = obj.start;

        values = std::move(obj.values);
        positions = obj.positions;

        return *this;
    }

    
    // void add_neighbor(std::vector<Square<square_side>*>& square_values, const std::pair<double, double>& position) {
    //     if(SquareExist(position) && !(getSquareByCenter(position.first, position.second)->isBlocked())) {
    //         square_values.push_back(getSquareByCenter(position.first, position.second));
    //     }
    // }


    // std::vector<Square<square_side>*> getNeighbors(const Square<square_side>& current) {
    //     std::vector<Square<square_side>*> square_values;
    //     std::pair<double, double> neighbor;
        
    //     neighbor = getUpperCenter(current);
    //     add_neighbor(square_values, neighbor);

    //     neighbor = getUpperRightCenter(current);
    //     add_neighbor(square_values, neighbor);

    //     neighbor = getRightCenter(current);
    //     add_neighbor(square_values, neighbor);

    //     neighbor = getBottomRightCenter(current);
    //     add_neighbor(square_values, neighbor);

    //     neighbor = getBottomCenter(current);
    //     add_neighbor(square_values, neighbor);
   
    //     neighbor = getBottomLeftCenter(current);
    //     add_neighbor(square_values, neighbor);

    //     neighbor = getLeftCenter(current);
    //     add_neighbor(square_values, neighbor);

    //     neighbor = getUpperLeftCenter(current);
    //     add_neighbor(square_values, neighbor);


    //     return square_values;
    // }


    // static std::pair<double, double> getUpperCenter(const Square<square_side>& obj) {
    //     return std::pair<double, double>(obj.getCenter().first, obj.getCenter().second+square_side);
    // }

    // static std::pair<double, double> getUpperRightCenter(const Square<square_side>& obj) {
    //     return std::pair<double, double>(obj.getCenter().first+square_side, obj.getCenter().second+square_side);
    // }

    // static std::pair<double, double> getRightCenter(const Square<square_side>& obj) { 
    //     return std::pair<double, double>(obj.getCenter().first+square_side, obj.getCenter().second);
    // }

    // static std::pair<double, double> getBottomRightCenter(const Square<square_side>& obj) { 
    //     return std::pair<double, double>(obj.getCenter().first+square_side, obj.getCenter().second-square_side);
    // }

    // static std::pair<double, double> getBottomCenter(const Square<square_side>& obj) {
    //     return std::pair<double, double>(obj.getCenter().first, obj.getCenter().second-square_side);
    // }

    // static std::pair<double, double> getBottomLeftCenter(const Square<square_side>& obj) {
    //     return std::pair<double, double>(obj.getCenter().first-square_side, obj.getCenter().second-square_side);
    // }

    // static std::pair<double, double> getLeftCenter(const Square<square_side>& obj) { 
    //     return std::pair<double, double>(obj.getCenter().first-square_side, obj.getCenter().second);
    // }

    // static std::pair<double, double> getUpperLeftCenter(const Square<square_side>& obj) { 
    //     return std::pair<double, double>(obj.getCenter().first-square_side, obj.getCenter().second+square_side);
    // }
    

    void print() {
        for(int16_t j = height-1; j >= 0; --j) {
            for(int16_t i = 0; i < width; ++i) {
                if(i == start.first && j == start.second) {
                    std::cout << 'S';
                }
                else {
                    if(values.at(i+j*width)) {
                        std::cout << 1;  
                    }
                    else {
                        std::cout << 0;
                    }
                }
                std::cout << "   ";
            }
            std::cout << '\n';
        }
        std::cout << "\n\n";
    }


    ~Area() {
        for(size_t i = 0; i < values.size(); ++i) {
            delete values[i];
        }
    }

};


#endif