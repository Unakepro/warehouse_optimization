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


template <size_t square_side>
void add_items(std::vector<Square<square_side>*> loc, std::vector<std::string>::iterator& it, std::unordered_map<std::string, Square<square_side>*>& data, size_t n) {
    
    for(auto* location: loc) {
        for(size_t count = 0; count < n; ++count, ++it) {
            data.insert(std::pair<std::string, Square<2>*>(*it, location));
        }
    }
}

template<size_t side>
double total_cost(std::vector<std::string>& transactions, Square<side>* start, std::unordered_map<std::string, Square<2>*>& locations) {
    double curr_cost = 0;

    for(const auto& item: transactions) {
        auto item_it = locations.find(item);
        if(item_it != locations.end()) {
                curr_cost += euclidian_distance(start, item_it->second);
                start = item_it->second;
        }
        else {
            throw std::logic_error("Not found");
        }
    }

    return curr_cost;
}

template<size_t side>
void start_state(Area<side>& warehouse, std::vector<std::string>& items, std::unordered_map<std::string, Square<side>*>& locations) {
    auto it = items.begin();
    
    add_items<side>(warehouse.getSquares(std::pair<size_t, size_t>(0, 1), std::pair<size_t, size_t>(0, 6)), it, locations, 1);
    add_items<side>(warehouse.getSquares(std::pair<size_t, size_t>(2, 3), std::pair<size_t, size_t>(8, 4)), it, locations, 1);
    add_items<side>(warehouse.getSquares(std::pair<size_t, size_t>(2, 1), std::pair<size_t, size_t>(8, 1)), it, locations, 1);
    add_items<side>(warehouse.getSquares(std::pair<size_t, size_t>(2, 6), std::pair<size_t, size_t>(8, 6)), it, locations, 1);
    add_items<side>(warehouse.getSquares(std::pair<size_t, size_t>(10, 1), std::pair<size_t, size_t>(10, 6)), it, locations, 1);
}

template<size_t side>
void generate_new_state(Area<side>& warehouse, std::vector<std::string>& items, std::unordered_map<std::string, Square<side>*>& locations) {
    std::uniform_int_distribution<size_t> dist(0, items.size()-1);

    size_t i = dist(gen);
    size_t j = dist(gen);


    if(i > j) {
        std::reverse(items.begin() + j, items.end() - i);
    }
    else {
        std::reverse(items.begin() + i, items.end() - j);
    }
    
    auto it = items.begin();
    add_items<side>(warehouse.getSquares(std::pair<size_t, size_t>(0, 1), std::pair<size_t, size_t>(0, 6)), it, locations, 1);
    add_items<side>(warehouse.getSquares(std::pair<size_t, size_t>(2, 3), std::pair<size_t, size_t>(8, 4)), it, locations, 1);
    add_items<side>(warehouse.getSquares(std::pair<size_t, size_t>(2, 1), std::pair<size_t, size_t>(8, 1)), it, locations, 1);
    add_items<side>(warehouse.getSquares(std::pair<size_t, size_t>(2, 6), std::pair<size_t, size_t>(8, 6)), it, locations, 1);
    add_items<side>(warehouse.getSquares(std::pair<size_t, size_t>(10, 1), std::pair<size_t, size_t>(10, 6)), it, locations, 1);

}

bool make_transition(long double P) {
    std::uniform_real_distribution<> dis(0.0, 1.0);
    if(P >= dis(gen)) {
        return true;
    }

    return false;
}



template<size_t side>
void sa_optimization(double start_temp, double end_temp, double cooling_rate, size_t steps, Area<side>& warehouse, Square<side>* start) {
    std::unordered_map<std::string, Square<side>*> locations;
    std::vector<std::string> records;
    
    std::ifstream file_csv("Data/reduced_csv/train_r.csv");
    get_records(file_csv, records);
    file_csv.close();

    auto transactions = get_transactions(records);
    std::vector<std::string> items;    
    
    std::ifstream file2("Data/products_names/reduced_pop.txt");
    get_product_names(file2, items);
    file2.close();

    start_state<side>(warehouse, items, locations);

    double currEnergy = total_cost<side>(transactions, start, locations);
    double T = start_temp;
    
    size_t i = 0;
    while(i < steps && currEnergy != 0) {

        double newEnergy = 0;
        
        Area<side> new_warehouse(warehouse.getWidth(), warehouse.getHeight());
        std::unordered_map<std::string, Square<side>*> new_locations;
        std::vector<std::string> new_items = items; 

        generate_new_state<side>(new_warehouse, new_items, new_locations);
     
        
        newEnergy = total_cost<side>(transactions, start, new_locations);
        

        if(newEnergy < currEnergy) {
            currEnergy = newEnergy;

            warehouse = new_warehouse;
            locations = std::move(new_locations);
            items = std::move(new_items);
        }
        else {
            if(make_transition(exp(-((newEnergy-currEnergy)/T)))) {
                currEnergy = newEnergy;

                warehouse = new_warehouse;
                locations = std::move(new_locations);
                items = std::move(new_items);
            }
   
        }

        T = start_temp / (1 + cooling_rate * i); 

        if(T <= end_temp) {
            return ;
        }

        ++i;
        
        std::cout << i << "   " << T << std::endl; 
    }

    std::cout << std::fixed << std::setprecision(5) << currEnergy << std::endl;
}


int main() {
    
    Area<2> warehouse(11, 7);
    Square<2>* start = warehouse.getSquare(0, 0);

    sa_optimization<2>(100000, 0.01, 0.99, 100000, warehouse, start);
}