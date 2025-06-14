#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <fstream>
#include <sstream>  
#include <map>
#include "Warehouse/warehouse.hpp"
#include "reader_csv.hpp"


std::random_device rd;
std::mt19937 gen(rd());


bool make_transition(long double P) {
    std::uniform_real_distribution<> dis(0.0, 1.0);
    if(P >= dis(gen)) {
        return true;
    }

    return false;
}



void sa_optimization(Warehouse& wh, std::vector<std::string>& products, const std::vector<std::string>& orders, int perShelf, double T0, double Tend, double cooling_rate, size_t steps) {
        wh.assign(products, perShelf);
    double bestE = wh.pathCost(orders);
    std::uniform_int_distribution<> swap(0, static_cast<int>(products.size() - 1));


    double currEnergy = bestE;

    size_t i = 0;
    while(i < steps) {

        auto tempProd = products;
        std::swap(tempProd[swap(gen)], tempProd[swap(gen)]);
        
        Warehouse trial = wh;
        trial.assign(tempProd, perShelf);
        double newEnergy = trial.pathCost(orders); 

        if(newEnergy < currEnergy || make_transition(exp(-((newEnergy-currEnergy)/T0)))) {
            
            bestE = std::min(newEnergy, bestE);

            wh = std::move(trial);
            products.swap(tempProd);
            currEnergy = newEnergy;
        }
       

        T0 *= cooling_rate; 

        if(T0 <= Tend) {
            std::cout << "Final cost = " << bestE << "\n";
            return ;
        }

        ++i;

    }
    
    std::cout << "Final cost = " << bestE << "\n";
}





int main() {
    
    try {
        auto transactions = get_records("Data/train.csv");
        auto products = read_lines("Data/products_names/products.txt");

        Warehouse wh(11, 7, {0, 0});
        wh.addShelfs({0,1}, {0,6});
        wh.addShelfs({2,3}, {8,4});
        wh.addShelfs({2,1}, {8,1});
        wh.addShelfs({2,6}, {8,6});
        wh.addShelfs({10,1}, {10,6});

        sa_optimization(wh, products, transactions, 3, 100000, 0.01, 0.9999, 1000000);
    }
    catch(const std::exception& e) {
        std::cout << "Error: " << e.what() << '\n';
        return 1;
    }
}