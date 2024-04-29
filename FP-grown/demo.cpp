#include <iostream>
#include "fp-grown.hpp"
#include <fstream>
#include "../reader_csv.hpp"


// decltype(auto) find_common(std::set<std::string>& obj1, const std::set<std::string>& obj2) {
//     std::set<std::string> result;

//     for(const auto& item: obj1) {
//         if(obj2.find(item) != obj2.end()) {
//             result.emplace(item);
//         }
//     }

//     return result;
// }

int main() {
    std::vector<std::string> records;

  //  FP_Tree tree;

    std::ifstream file_csv("../Data/original_data.csv");
    get_records(file_csv, records);
    file_csv.close();
    

    auto obj = get_ordered_frequency(records, 900);
    for(auto trans: obj) {
        for(auto item: trans) {
            std::cout << item << ' ';
        }
        std::cout << '\n';
    }

    // for(size_t n = obj.size(); n > 0; --n) {
    //     for(auto& item: obj[n-1]) {
    //         std::cout << n << ' ' << item << ' ';
    //     }
        
    //     std::cout << "\n\n";
    // }


    // auto obj2 = get_ordered_frequency(records, 60);
    
    
    // std::vector<std::string> unpopular;
    
    // std::ifstream file("../Data/most_popular.txt");
    // get_product_names(file, unpopular);
    // file.close();

    // std::reverse(unpopular.begin(), unpopular.end());
  

    // for(auto& [_, list]: obj2) {
    //     tree.insert(list);
    // }

    // std::map<std::string, std::set<std::set<std::string>>> association_table;
    
    // for(const auto& item: unpopular) {
    //     auto it = tree.prev_list.find(item);
    //     FP_Node* node = nullptr;
        
    //     if(it != tree.prev_list.end()) {
    //         node = it->second;
    //     }
    //     else {
    //         association_table[item];
    //         continue;
    //     }

    //     while (node != nullptr) {
    //         //std::cout << node->item << '\n';

    //         auto parent = node->parent;
            
            
    //         std::set<std::string> tmp;
    //         while (parent != nullptr && parent->item != "fakenode")
    //         {
    //             tmp.insert(parent->item);
    //             parent = parent->parent;

    //         }
    //         auto it = association_table.find(item);
    //         node = node->prevOccurrence;
    //     }
        
    //     for(auto& st: obj) {
    //         for(auto& item: st) 
    //             std::cout << item << ' ';
    //         std::cout << '\n';
    //     }


    //     std::set<std::string> common = *(obj.begin());
    //     for (auto& vct: obj) {
    //         common = find_common(common, vct);
    //     }
      
    //     // for(auto item: common) {
    //     //     std::cout << item << ' ';
    //     // }
       
    //     std::cout << '\n';
    // }


}