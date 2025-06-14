# Warehouse Picking Optimisation

A minimal C++17 project that models a rectangular **warehouse** and applies **simulated annealing (SA)** to assign products to shelf locations so that the total travel distance for fulfilling a set of orders is minimised.

---

## ✨ Features

* Lightweight header‑only `Warehouse` model.
* Configurable warehouse geometry, start position, and shelf blocks.
* SA optimiser with tunable temperature schedule and iteration budget.
* Simple plain‑text & CSV inputs – no external libraries required.

---

## 📁 Directory Layout

```text
.
├── src/
│   ├── main.cpp                ← entry point & parameter setup
│   ├── Warehouse/
│   │   └── warehouse.hpp       ← warehouse data structure & distance metric
│   └── reader_csv.hpp          ← tiny utility to load CSV orders
├── Data/
│   ├── train.csv               ← sample order lines (comma‑separated)
│   └── products_names/
│       └── products.txt        ← list of all SKUs, one per line
└── README.md                   ← this file
```

Feel free to reorganise – the paths in `main.cpp` simply need updating.

---

## 🔧 Building

### Prerequisites

* **C++17**‑capable compiler – GCC ≥ 7, Clang ≥ 7

### One‑liner with g++

```bash
g++ -std=c++17 -O3 src/main.cpp -o optimise
```


## 📑 Input Files

| File                               | Purpose                        | Expected Format                                                                                       |
| ---------------------------------- | ------------------------------ | ----------------------------------------------------------------------------------------------------- |
| `Data/products_names/products.txt` | Universe of products/SKUs      | `SKU_001`<br>`SKU_002`<br>`…` – *one per line*                                                        |
| `Data/train.csv`                   | Historical or synthetic orders | `SKU_001,SKU_203,SKU_087`<br>`SKU_150`<br>`SKU_087,SKU_991,SKU_150` – *comma‑separated names per row* |

> **Tip:** Use your own datasets – just keep names consistent across both files.

---

## 🚀 Running

```bash
./a.out
```

All knobs live in `main.cpp` – by default:

```cpp
sa_optimization(
    wh,             // Warehouse layout
    products,       // Product universe (vector<string>)
    transactions,   // Orders (vector<string>)
    3,              // perShelf – items per physical shelf
    100000,         // T0 – initial temperature
    0.01,           // Tend – stopping temperature
    0.9999,         // cooling_rate – geometric schedule factor
    1'000'000       // steps – iteration cap
);
```
---

## 🏗️  Warehouse Coordinate System

* Origin **(0, 0)** sits at the **picker start** (bottom‑left corner).
* A cell is walkable unless flagged as a shelf (`cells[idx] == true`).
* Rectangular shelf blocks are added via:

```cpp
wh.addShelfs({x1, y1}, {x2, y2}); // inclusive top‑left & bottom‑right
```

Example layout from 

```text
Y 6 |  █████████         █  
  5 |                    █  
  4 |  █████████  
  3 |                    █  ← horizontal aisle
  2 |                      
  1 |  █████████  
  0 | S                (0,0) → X
```

*(S = start, █ = shelf cell)*

---

## 🔬 Algorithm Overview

1. **Initial placement** – products are assigned to shelf coordinates sequentially.
2. **Neighbour generation** – randomly pick and swap two products.
3. **Energy function** – sum of Euclidean distances travelled when processing every order in `train.csv`, including a return to start.
4. **Acceptance rule** – always accept improvements; otherwise accept with probability `exp(-(ΔE) / T)`.
5. **Cooling** – multiply `T` by `cooling_rate` until `T ≤ Tend` or iteration budget reached.

The best layout encountered is kept and printed with its cost.

---


## 📄 Licence

Licensed under the **MIT License** – see `LICENSE` for details.

---


## 🙏 Acknowledgements

Inspired by classic order‑picking optimisation literature and many warehouse robotics hack‑days.
