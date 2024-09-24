#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <fstream>
#include <sstream>  
#include <map>
#include "Area/area.hpp"
#include "reader_csv.hpp"


std::random_device rd;
std::mt19937 gen(rd());


// template <size_t square_side>
// void add_items(std::vector<Square<square_side>*> loc, std::vector<std::string>::iterator& it, std::unordered_map<std::string, Square<square_side>*>& data, size_t n) {
    
//     for(auto* location: loc) {
//         for(size_t count = 0; count < n; ++count, ++it) {
//             data.insert(std::pair<std::string, Square<2>*>(*it, location));
//         }
//     }
// }


template<size_t side>
void start_state(Area<side>& warehouse, std::vector<std::string>& items, size_t items_per_shelf) {    
    warehouse.setSquares(std::pair<size_t, size_t>(0, 1), std::pair<size_t, size_t>(0, 6));
    warehouse.setSquares(std::pair<size_t, size_t>(2, 3), std::pair<size_t, size_t>(8, 4));
    warehouse.setSquares(std::pair<size_t, size_t>(2, 1), std::pair<size_t, size_t>(8, 1));
    warehouse.setSquares(std::pair<size_t, size_t>(2, 6), std::pair<size_t, size_t>(8, 6));
    warehouse.setSquares(std::pair<size_t, size_t>(10, 1), std::pair<size_t, size_t>(10, 6));

    warehouse.setClosest(items, items_per_shelf);
}

template<size_t side>
void generate_new_state(Area<side>& warehouse, std::vector<std::string>& items, size_t items_per_shelf) {
    std::uniform_int_distribution<size_t> dist(0, items.size()-1);

    size_t i = dist(gen);
    size_t j = dist(gen);

    size_t k = dist(gen);
    size_t m = dist(gen);

    std::swap(items[i], items[j]);
    std::swap(items[k], items[m]);

    // if(i > j) {
    //     std::reverse(items.begin() + j, items.end() - i);
    // }
    // else {
    //     std::reverse(items.begin() + i, items.end() - j);
    // }

    warehouse.setSquares(std::pair<size_t, size_t>(0, 1), std::pair<size_t, size_t>(0, 6));
    warehouse.setSquares(std::pair<size_t, size_t>(2, 3), std::pair<size_t, size_t>(8, 4));
    warehouse.setSquares(std::pair<size_t, size_t>(2, 1), std::pair<size_t, size_t>(8, 1));
    warehouse.setSquares(std::pair<size_t, size_t>(2, 6), std::pair<size_t, size_t>(8, 6));
    warehouse.setSquares(std::pair<size_t, size_t>(10, 1), std::pair<size_t, size_t>(10, 6));
    
    warehouse.setClosest(items, items_per_shelf);
}

bool make_transition(long double P) {
    std::uniform_real_distribution<> dis(0.0, 1.0);
    if(P >= dis(gen)) {
        return true;
    }

    return false;
}



template<size_t side>
void sa_optimization(double start_temp, double end_temp, double cooling_rate, size_t steps, Area<side>& warehouse, std::vector<std::string>& items, size_t items_per_shelf, std::vector<std::string>& transactions) {
    
    start_state<side>(warehouse, items, items_per_shelf);

    double currEnergy = warehouse.total_cost(transactions);
    double T = start_temp;
    
    std::cout << warehouse.total_cost(transactions) << std::endl;

    size_t i = 0;
    while(i < steps && currEnergy != 0) {

        double newEnergy = 0;       
        Area<side> new_warehouse(warehouse.getWidth(), warehouse.getHeight());
        std::vector<std::string> new_items = items;
        
        generate_new_state<side>(new_warehouse, new_items, items_per_shelf);    
        newEnergy = new_warehouse.total_cost(transactions);
        

        if(newEnergy < currEnergy) {
            currEnergy = newEnergy;

            warehouse = new_warehouse;
            items = std::move(new_items);
        }
        else {
            if(make_transition(exp(-((newEnergy-currEnergy)/T)))) {
                currEnergy = newEnergy;

                warehouse = new_warehouse;
                items = std::move(new_items);
            }
   
        }

        T = start_temp / (1 + cooling_rate * i); 

        if(T <= end_temp) {
            return ;
        }

        ++i;
        
        std::cout << i << "   " << T << ' ' << currEnergy << std::endl; 
    }

}


int main() {
    
    Area<2> warehouse(11, 7);

    warehouse.setSquares(std::pair<size_t, size_t>(0, 1), std::pair<size_t, size_t>(0, 6));
    warehouse.setSquares(std::pair<size_t, size_t>(2, 3), std::pair<size_t, size_t>(8, 4));
    warehouse.setSquares(std::pair<size_t, size_t>(2, 1), std::pair<size_t, size_t>(8, 1));
    warehouse.setSquares(std::pair<size_t, size_t>(2, 6), std::pair<size_t, size_t>(8, 6));
    warehouse.setSquares(std::pair<size_t, size_t>(10, 1), std::pair<size_t, size_t>(10, 6));

    warehouse.print();
    std::vector<std::string> transactions;
    std::vector<std::string> products;

    std::ifstream file_csv("Data/train.csv");
    get_records(file_csv, transactions);  
    file_csv.close();

    
    std::ifstream file2("Data/products_names/most_popular.txt");
    get_product_names(file2, products);
    file2.close();

    sa_optimization<2>(50000, 0.01, 0.99, 100000, warehouse, products, 3, transactions);
    std::cout << warehouse.total_cost(transactions) << std::endl;

}