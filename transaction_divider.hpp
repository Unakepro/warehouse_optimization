#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>

decltype(auto) get_transactions(const std::vector<std::string>& records) {
    std::vector<std::vector<std::string>> transactions;
    
    std::vector<std::string> tmp_items;
    std::string id = records[0];

    for(size_t i = 0; i < records.size(); ++i) {
       if(records[i] != id) {
            transactions.push_back(std::move(tmp_items));
            id = records[i];
            tmp_items.resize(0);
            
        }
        
        ++i;
        tmp_items.push_back(records[i]);
    }

    if(tmp_items.size() != 0) {
        transactions.push_back(std::move(tmp_items));
    }

    return transactions;
}

std::unordered_map<std::string, size_t> get_transaction_frequency(const std::vector<std::string>& records) {
    std::unordered_map<std::string, size_t> freq;
    for(size_t i = 1; i < records.size(); ++i) {
        
        auto it = freq.find(records[i]);
        if(it == freq.end()) {
            freq.emplace(records[i], 1);
        }
        else {
            (*it).second += 1;
        }
        ++i;
    }
    return freq;
}