#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>  
#include <map>
#include "A*/A_star.hpp"
#include "reader_csv.hpp"
#include "FP-grown/fp-grown.hpp"

template <size_t square_side>
void add_items(std::vector<Square<square_side>*> loc, std::vector<std::string>::iterator& it, std::map<std::string, Square<square_side>*>& data, size_t n) {
    
    for(auto* location: loc) {
        for(size_t count = 0; count < n; ++count, ++it) {
            data.insert(std::pair<std::string, Square<2>*>(std::move(*it), location));
        }
    }
}



auto fp_location(std::vector<std::string>& records, std::vector<std::string>& items_by_popularity) {
    FP_Tree tree;
    std::vector<std::string> result;

    auto transactions = get_ordered_transactions(records, 60);
        
    for(auto transaction: transactions) {
        tree.insert(transaction);
    }
    
    auto association_map = getAssociation_map(items_by_popularity, tree);

    std::unordered_set<std::string> closed;

    for(auto& product: items_by_popularity) {
        if(closed.find(product) != closed.end()) {
            continue;
        }
        
        closed.emplace(product);
        result.push_back(product);

        if(association_map.find(product) != association_map.end()) { 

            std::string sel_item = "none";
            for(auto& [item, counter1]: association_map.find(product)->second) {

                if(closed.find(item) == closed.end()) {
                    sel_item = item;
                    closed.emplace(item);
                    result.push_back(item);

                    break;
                }
            }

            if(sel_item == "none") {
                continue;
            }

            for(auto& [item2, counter2]: association_map.find(sel_item)->second) {
                        
                if(closed.find(item2) == closed.end()) {
                    closed.emplace(item2);
                    result.push_back(item2);
                    break;
                }
             
            }
                    
        }        
    }
    return result;
}







int main() {
    
    Area<2> warehouse(11, 7);
    
    std::map<std::string, Square<2>*> products_loc;
    std::vector<std::string> records;

    std::ifstream file_csv("Data/run_data.csv");
    get_records(file_csv, records);
    file_csv.close();
 

    std::vector<std::string> items_by_popularity;
    
    std::ifstream file2("Data/most_popular.txt");
    get_product_names(file2, items_by_popularity);
    file2.close();

    // use association_map
    auto items_fp_loc = fp_location(records, items_by_popularity);
    auto it = items_fp_loc.begin();

    // simply put in shelfs most popular item first
    //auto it = items_by_popularity.begin();
    
    
    // poor location
    // add_items<2>(warehouse.getSquares(std::pair<size_t, size_t>(0, 1), std::pair<size_t, size_t>(0, 6)), it, products_loc, 3);
    // add_items<2>(warehouse.getSquares(std::pair<size_t, size_t>(2, 1), std::pair<size_t, size_t>(8, 1)), it, products_loc, 3);
    // add_items<2>(warehouse.getSquares(std::pair<size_t, size_t>(10, 1), std::pair<size_t, size_t>(10, 6)), it, products_loc, 3);
    // add_items<2>(warehouse.getSquares(std::pair<size_t, size_t>(2, 6), std::pair<size_t, size_t>(8, 6)), it, products_loc, 3);
    // add_items<2>(warehouse.getSquares(std::pair<size_t, size_t>(2, 3), std::pair<size_t, size_t>(8, 4)), it, products_loc, 3);

    //naive location
    add_items<2>(warehouse.getSquares(std::pair<size_t, size_t>(0, 1), std::pair<size_t, size_t>(0, 6)), it, products_loc, 3);
    add_items<2>(warehouse.getSquares(std::pair<size_t, size_t>(2, 3), std::pair<size_t, size_t>(8, 4)), it, products_loc, 3);
    add_items<2>(warehouse.getSquares(std::pair<size_t, size_t>(2, 1), std::pair<size_t, size_t>(8, 1)), it, products_loc, 3);
    add_items<2>(warehouse.getSquares(std::pair<size_t, size_t>(2, 6), std::pair<size_t, size_t>(8, 6)), it, products_loc, 3);
    add_items<2>(warehouse.getSquares(std::pair<size_t, size_t>(10, 1), std::pair<size_t, size_t>(10, 6)), it, products_loc, 3);


    Square<2>* start = warehouse.getSquare(0, 0);
    size_t user_path = 0;
    auto tr_item_map = get_transactions(records);

    for(const auto& items: tr_item_map) {
        for(const auto& item: items) {
            auto item_it = products_loc.find(item);
            if(item_it != products_loc.end()) {
    
                user_path += calculate_total_cost(a_search<2>(warehouse, start, item_it->second));
                start = item_it->second;

                warehouse.reset_squares();
            }
        }
    }

    std::cout << user_path;
}

