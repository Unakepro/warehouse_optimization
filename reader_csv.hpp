#ifndef READER_CSV
#define READER_CSV

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>  
#include <map>
#include <set>
#include <unordered_map>


void get_records(std::istream& stream, std::vector<std::string>& obj) {
    std::string line;
    std::string segment;

    std::getline(stream, line);
    while (std::getline(stream, line))
    {
        std::stringstream s(line);

        size_t i = 0;
        while(std::getline(s, segment, ',')) {
            ++i;
            if(i % 2 == 1) {
                continue;
            }
            obj.push_back(std::move(segment));
        }
    }
}

void get_product_names(std::istream& stream, std::vector<std::string>& obj) {
    std::string product;

    while(std::getline(stream, product)) {
        obj.push_back(std::move(product));
    }
}

#endif