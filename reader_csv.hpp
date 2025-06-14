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


std::vector<std::string> get_records(const std::string& path) {
    std::ifstream in(path);

    if (!in) {
        throw std::runtime_error("Cannot open " + path);
    }
    
    std::vector<std::string> res;
    std::string line;


    std::getline(in, line);
    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string segment;

        size_t i = 0;
        while(std::getline(ss, segment, ',')) {
            if(i % 2 == 1) {
                res.push_back(std::move(segment));
            }
            ++i;
        }
    }

    return res;
}

std::vector<std::string> read_lines(const std::string& path) {
    std::ifstream in(path);
    if(!in) {
        throw std::runtime_error("Cannot open" + path);
    } 

    std::vector<std::string> res;
    std::string s;

    while(std::getline(in, s)) {
        res.push_back(std::move(s));
    }

    return res;
}

#endif