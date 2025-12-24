# The Arcadia Engine

A high-performance backend engine designed for the Arcadia MMORPG world, implemented for the CS321 — Algorithms Analysis and Design course at Cairo University. This project integrates advanced data structures, dynamic programming, graph algorithms, and greedy scheduling into one cohesive engine.

---

## 📌 Overview

The **Arcadia Engine** is a modular library consisting of four major subsystems:

1. **Registry** – Player lookup, leaderboards, and auction systems using:

   - Custom Hash Table (with double hashing)
   - Skip List leaderboard
   - Red-Black Tree auction house

2. **Inventory System** – Player loot and backpack management using:

   - Partition DP (Loot Splitting)
   - 0/1 Knapsack DP (Inventory Packer)
   - String DP (Chat autocorrect)

3. **Navigator** – World traversal and safety algorithms:

   - Path existence (BFS/DFS)
   - Minimum bribe MST (Kruskal/Prim)
   - All-pairs shortest path (Floyd–Warshall)

4. **Kernel** – Server task scheduling:

   - CPU task scheduler with cooldown interval (Greedy)

---

## 🧩 Project Structure

```
The-Arcadia-Engine/
├── ArcadiaEngine.h          # Provided — must NOT be changed
├── ArcadiaEngine.cpp        # Full implementation
├── TeamInfo.txt             # Team member details
└── README.md                # This file
```

---



## 🚀 Features by Subsystem

### 🔹 **Part A — Registry**

- **PlayerTable**: Custom hash function + double hashing.
- **Leaderboard**: Skip List with `O(log n)` insertion, deletion, search, and `getTopN(n)`.
- **AuctionTree**: Fully implemented Red-Black Tree with all invariants preserved.

---

### 🔹 **Part B — Inventory System**

- **optimizeLootSplit**: Partition DP minimizing difference.
- **maximizeCarryValue**: 0/1 Knapsack DP.
- **countPossibleOriginals**: String DP for autocorrect.

---

### 🔹 **Part C — Navigator**

- **pathExists**: BFS/DFS connectivity.
- **minBribeCost**: MST variant using weighted cost conversion.
- **sumOfShortestPaths**: All-Pairs Shortest Path (Floyd–Warshall) returning binary string.

---

### 🔹 **Part D — Kernel**

- **leastInterval**: Greedy CPU scheduler respecting cooldown constraints.

---

## 📚 Algorithms Used

| Subsystem      | Algorithm                                    | Complexity         |
| -------------- | -------------------------------------------- | ------------------ |
| Hash Table     | Division + Double Hashing                    | O(1) avg           |
| Skip List      | Randomized multilevel list                   | O(log n)           |
| Red-Black Tree | Self-balancing BST                           | O(log n)           |
| Loot Split     | Subset-sum DP                                | O(n × sum)         |
| Knapsack       | 0/1 DP                                       | O(n × W)           |
| String DP      | Linear transitions                           | O(n)               |
| Path Existence | DFS                                          | O(V + E)           |
| MST            | Kruskal                                      | O(E log V)         |
| APSP           | Floyd–Warshall                               | O(V³)              |
| Scheduler      | Greedy max-frequency                         | O(n) or O(n log n) |

---



## 🏛️ Academic Notes

This project was submitted as part of:

**CS321 — Algorithms Analysis and Design**\
Faculty of Computing and Artificial Intelligence, Cairo University

All work complies with the course guidelines and plagiarism policy.

---

## 📜 License

This project is licensed under the **MIT License**. You are free to use, modify, distribute, and build upon this software, provided that the original copyright notice and permission notice are included in all copies or substantial portions of the software.

---

### ⭐ Thank you for exploring *The Arcadia Engine*!"

