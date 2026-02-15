# BLG 223E - Data Structures Portfolio (2025-2026)

This repository contains the programming assignments for the **BLG 223E - Data Structures** course at Istanbul Technical University (İTÜ). Each project focuses on implementing fundamental data structures and algorithms using the **C programming language**.

---

## 📂 Project Summaries

### 1. Four-Directional Cell Grid (Homework 1)
Implementation of a specialized 2D grid where each node is linked to its four immediate neighbors: **up, down, left, and right**.
* **Unique Architecture**: Maintains a `head` pointer that always points to the **center node**.
* **Functionality**: Includes dynamic row deletion and grid-wide summation.

### 2. BST-Based Inventory Management (Homework 2)
A Binary Search Tree (BST) system designed to manage and filter items based on their price and expiration data.
* **Core Features**: Implements recursive insertion and search algorithms to retrieve items above a specific price threshold in ascending order.

### 3. AVL Tree: The Grand Vizier's Ledger (Homework 3)
A self-balancing AVL Tree implementation to organize historical records, ordered by their appointment year.
* **Efficient Navigation**: Utilizes **Parent Pointers** in the node structure to allow fast access to immediate predecessors and successors.
* **Balancing Logic**: Implements left/right rotations to ensure $|BF| \le 1$.

### 4. PageRank & Graph Algorithms (Homework 4)
Implementation of the PageRank algorithm using a random-walk-based approximation and shortest path searching.
* **Random Surfer Model**: Simulates a user surfing through **1,000,000 steps** to calculate stationary distributions.
* **Mathematical Foundation**: $G=dM+(1-d)\frac{1}{N}11^{T}$
* **Pathfinding**: Includes Breadth-First Search (BFS) to find the shortest path between nodes in a web graph.

---

## 🛠️ Technical Stack
* **Language**: C
* **Tools**: Valgrind for memory leak analysis.
* **Algorithms**: Random Walk, AVL Rotations, BFS.

---
*Created as part of my Computer Engineering studies at İTÜ.*
