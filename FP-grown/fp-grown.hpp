#ifndef FPGROWN
#define FPGROWN

#include <iostream>
#include "../transaction_divider.hpp"
#include <algorithm>
#include <unordered_map>
#include <deque>
#include <set>

bool freq_comparator(const std::string& obj1, const std::string& obj2, const std::unordered_map<std::string, size_t>& freq) {
    return freq.at(obj1) > freq.at(obj2);
}

auto find_by_value(std::vector<std::pair<std::string, size_t>>::iterator begin, std::vector<std::pair<std::string, size_t>>::iterator end, const std::string& value) {
    auto found = end;
    while (begin != end)
    {
        if(begin->first == value) {
            found = begin;
            break;
        }
        ++begin;
    }
    
    return found;
}

bool compare_size(const std::pair<std::string, size_t>& lhs, const std::pair<std::string, size_t>& rhs) {
    return lhs.second < rhs.second;
}

auto get_ordered_transactions(const std::vector<std::string>& records, size_t threshold) {   
    auto transactions = get_transactions(records);
    auto freq = get_transaction_frequency(records);

    
    std::vector<std::vector<std::string>> ordered;
    
    std::vector<std::string> tmp_items;
    for(auto& items: transactions) {

        for(auto& item: items) {
            auto it = freq.find(item);
            if(it == freq.end()) {
                throw std::logic_error("element not found!");
            }
            else {
                if(it->second >= threshold) {
                    tmp_items.push_back(std::move(item));
                }
            }
        }

        std::sort(tmp_items.begin(), tmp_items.end(), [&](const std::string& lhs, const std::string& rhs) {
            return freq_comparator(lhs, rhs, freq);
        });

        ordered.push_back(std::move(tmp_items));
    }

    return ordered;
}


struct FP_Node {
    std::string item;
    size_t count = 0;
    std::unordered_map<std::string, FP_Node*> children;
    FP_Node* parent = nullptr;
    FP_Node* prevOccurrence = nullptr;

public:
    FP_Node(const std::string& item, FP_Node* parent = nullptr): item(item), count(0), parent(parent), prevOccurrence(nullptr) {}

};

class FP_Tree {
    FP_Node* root = nullptr;
    std::unordered_map<std::string, FP_Node*> prev_array;

public:
    FP_Tree() = default;


    std::unordered_map<std::string, FP_Node*>& getPrevArr() {
        return prev_array;
    }

    void insert(const std::vector<std::string>& items) {
        FP_Node* current = root;

        if(root == nullptr) {
            root = new FP_Node("fakenode", nullptr);
            prev_array.emplace("fakenode", root);
            current = root;
        }     
        
        for(const auto& item: items) {            
            auto it = current->children.find(item);
            FP_Node* nextptr = nullptr;

            if(it != current->children.end()) {
                it->second->count += 1;
                nextptr = it->second;
            }
            else {
                nextptr = new FP_Node(item, current);
                current->children.emplace(item, nextptr);

                auto it2 = prev_array.find(item);
                if(it2 != prev_array.end()) {
                    nextptr->prevOccurrence = it2->second;
                    prev_array.erase(it2);
                }
                prev_array.emplace(item, nextptr);                
            }
            current = nextptr;
        } 
    }



};

auto getAssociation_map(const std::vector<std::string>& products, FP_Tree& tree) {
    std::unordered_map<std::string, std::vector<std::pair<std::string, size_t>>> association_map;
    
    std::vector<std::pair<std::string, size_t>> products_count;
    
    for(const auto& product: products) {
        auto it = tree.getPrevArr().find(product);
        
        if(it == tree.getPrevArr().end()) {
            continue;
        }
        
        FP_Node* node = it->second;
        while (node != nullptr)
        { 
            FP_Node* parent_node = node->parent;


            while (parent_node != nullptr && parent_node->item != "fakenode")
            {
                auto vec_it = find_by_value(products_count.begin(), products_count.end(), parent_node->item);
                if(vec_it != products_count.end()) {
                    vec_it->second += 1*node->count;
                }
                else {
                    products_count.emplace_back(parent_node->item, 1);
                }
                parent_node = parent_node->parent;
            }            
            node = node->prevOccurrence;    
        }
        std::sort(products_count.begin(), products_count.end(), compare_size);
        association_map.emplace(it->second->item, std::move(products_count));

    }

    return association_map;
}

#endif