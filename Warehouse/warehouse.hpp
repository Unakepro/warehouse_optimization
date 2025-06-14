#ifndef WAREHOUSE
#define WAREHOUSE

#include <iostream>
#include <vector>
#include <cmath>


struct Position { int x, y; };

inline double dist(const Position& a, const Position& b) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return std::sqrt(static_cast<double>(dx*dx + dy*dy));
}


class Warehouse {
    size_t width = 0;
    size_t height = 0;
    Position start{0, 0};

    std::vector<char> cells;
    std::vector<Position> shelves; 
    std::unordered_map<std::string, Position> mapping;

public:
    Warehouse(size_t w, size_t h, Position start): width(w), height(h), start(start), cells(width * height, false) {}

    size_t getWidth() const {
        return width;
    }

    size_t getHeight() const {
        return height;
    }

   
    void addShelfs(Position tl, Position br) {
        for (int y = tl.y; y <= br.y; ++y) {
            for (int x = tl.x; x <= br.x; ++x) {
                cells[idx(x, y)] = true;
            }
        }
    }
    

    void assign(const std::vector<std::string>& products, int perShelf) {
        shelves.clear();

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                    if (cells[idx(x, y)] && !(x == start.x && y == start.y))  {
                        shelves.push_back({x, y});
                   }
            }
        }

        // std::sort(shelves.begin(), shelves.end(), [](auto a, auto b) {
        //     return dist(a, start) < dist(b, start);
        // });

        if (products.size() > shelves.size() * static_cast<size_t>(perShelf)) {
            throw std::runtime_error("Not enough shelves for all products");
        }

        for (size_t i = 0; i < products.size(); ++i) {
            mapping[products[i]] = shelves[i / perShelf];
        }
    }        


    double pathCost(const std::vector<std::string>& pickList) const {
        Position cur = start;  
        double cost = 0;

        for (const auto& p : pickList) {
            auto it = mapping.find(p);
            if(it == mapping.end()) {
                throw std::logic_error("Product not found");
            }
            cost += dist(cur, it->second);
            cur = it->second;
        }

        
        return cost + dist(cur, start);
    }


    // void print() {
    //     for(int16_t j = height-1; j >= 0; --j) {
    //         for(int16_t i = 0; i < width; ++i) {
    //             if(i == start.first && j == start.second) {
    //                 std::cout << 'S';
    //             }
    //             else {
    //                 if(values.at(i+j*width)) {
    //                     std::cout << 1;  
    //                 }
    //                 else {
    //                     std::cout << 0;
    //                 }
    //             }
    //             std::cout << "   ";
    //         }
    //         std::cout << '\n';
    //     }
    //     std::cout << "\n\n";
    // }



private:
    int idx(int x, int y) const { return y * width + x; }

};


#endif