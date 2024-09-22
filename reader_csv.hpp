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

auto get_transactions(const std::vector<std::string>& records) {
    std::vector<std::string> transactions;
    
    for(size_t i = 0; i < records.size(); ++i) {        
        ++i;
        transactions.push_back(records[i]);
    }

    return transactions;
}


void get_records(std::istream& stream, std::vector<std::string>& obj) {
    std::string line;
    std::string segment;

    std::getline(stream, line);
    while (std::getline(stream, line))
    {
        std::stringstream s(line);
        while(std::getline(s, segment, ',')) {
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