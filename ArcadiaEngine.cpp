// ArcadiaEngine.cpp - STUDENT TEMPLATE
// TODO: Implement all the functions below according to the assignment requirements

#include "ArcadiaEngine.h"
#include <algorithm>
#include <queue>
#include <numeric>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <set>

using namespace std;

// =========================================================
// PART A: DATA STRUCTURES (Concrete Implementations)
// =========================================================

// --- 1. PlayerTable (Double Hashing) ---

class ConcretePlayerTable : public PlayerTable {
private:
    // TODO: Define your data structures here
    // Hint: You'll need a hash table with double hashing collision resolution

public:
    ConcretePlayerTable() {
        // TODO: Initialize your hash table
    }

    void insert(int playerID, string name) override {
        // TODO: Implement double hashing insert
        // Remember to handle collisions using h1(key) + i * h2(key)
    }

    string search(int playerID) override {
        // TODO: Implement double hashing search
        // Return "" if player not found
        return "";
    }
};

// --- 2. Leaderboard (Skip List) ---

struct Node {
    int playerID;
    int score;
    vector<Node*> forward; // forward pointers for each level

    Node(int id, int s, int level) : playerID(id), score(s), forward(level + 1, nullptr) {}
};

class ConcreteLeaderboard : public Leaderboard {
private:
    int Max_Level = 16; // Maximum level for skip list
    float P = 0.5; // Probability for random level generation

    Node* head; // Head node for each level in the skip list
    int currentLevel; // Current highest level in the skip list

    int randomLevel() {
        int lvl = 0;
        while (((float)rand() / RAND_MAX) < P && lvl < Max_Level)
            lvl++;
        return lvl;
    }

public:
    ConcreteLeaderboard() {
        currentLevel = 0;
        head = new Node(-1, INT_MAX, Max_Level); // Dummy head node has -1 ID beause is not a real player with max score to be in front always
    }

    
    void addScore(int playerID, int score) override {
        vector<Node*> update(Max_Level + 1, nullptr);
        Node* curr = head;

        // search position (descending order by score)
        for (int lvl = currentLevel; lvl >= 0; lvl--) { // loop through levels from top to bottom
            // continue moving forward while new node's score is less than current node's score and it's ID is greater (to maintain order)
            while (curr->forward[lvl] &&(curr->forward[lvl]->score >= score && curr->forward[lvl]->playerID < playerID)) curr = curr->forward[lvl];
            update[lvl] = curr; // Keep track of last node at this level before the position that the new node will be inserted
        }

        curr = curr->forward[0]; // Move forward in level 0 to check if player already exists, curr is now the node after the position where new node would be inserted

        // check first curr is not null and If player exists → update score (remove then insert again)
        if (curr && curr->playerID == playerID) {
            removePlayer(playerID);
        }

        int newLevel = randomLevel(); // Generate random level for new node

        if (newLevel > currentLevel) {
            for (int i = currentLevel + 1; i <= newLevel; i++)
                update[i] = head;
            currentLevel = newLevel;
        }

        Node* newNode = new Node(playerID, score, newLevel);

        for (int i = 0; i <= newLevel; i++) {
            newNode->forward[i] = update[i]->forward[i]; // Link new node to next node at level i
            update[i]->forward[i] = newNode; // Link previous node to new node at level i
        }
    }


    void removePlayer(int playerID) override {
        vector<Node*> update(Max_Level + 1, nullptr);
        Node* curr = head;

        // Find node
        for (int lvl = currentLevel; lvl >= 0; lvl--) {
            while (curr->forward[lvl] && curr->forward[lvl]->playerID < playerID) curr = curr->forward[lvl];
            update[lvl] = curr;
        }

        curr = curr->forward[0];

        if (curr && curr->playerID == playerID) {
            for (int i = 0; i <= currentLevel; i++) {
                if (update[i]->forward[i] != curr) break;
                update[i]->forward[i] = curr->forward[i];
            }

            delete curr;

            while (currentLevel > 0 && head->forward[currentLevel] == nullptr)
                currentLevel--;
        }
    }


    vector<int> getTopN(int n) override {
        vector<int> result;
        Node* curr = head->forward[0]; // First node in level 0

        while (curr && n--) {
            result.push_back(curr->playerID);
            curr = curr->forward[0];
        }

        return result;
    }
};

// --- 3. AuctionTree (Red-Black Tree) ---

class ConcreteAuctionTree : public AuctionTree {
private:
    // TODO: Define your Red-Black Tree node structure
    // Hint: Each node needs: id, price, color, left, right, parent pointers

public:
    ConcreteAuctionTree() {
        // TODO: Initialize your Red-Black Tree
    }

    void insertItem(int itemID, int price) override {
        // TODO: Implement Red-Black Tree insertion
        // Remember to maintain RB-Tree properties with rotations and recoloring
    }

    void deleteItem(int itemID) override {
        // TODO: Implement Red-Black Tree deletion
        // This is complex - handle all cases carefully
    }
};

// =========================================================
// PART B: INVENTORY SYSTEM (Dynamic Programming)
// =========================================================

int InventorySystem::optimizeLootSplit(int n, vector<int>& coins) {
    // TODO: Implement partition problem using DP
    // Goal: Minimize |sum(subset1) - sum(subset2)|
    // Hint: Use subset sum DP to find closest sum to total/2
    return 0;
}

int InventorySystem::maximizeCarryValue(int capacity, vector<pair<int, int>>& items) {
    int size_items = items.size();
    vector<vector<int>> V(size_items + 1, vector<int>(capacity + 1, 0)); // DP table
    vector<vector<int>> P(size_items + 1, vector<int>(capacity + 1, 0)); // To track selected items

    // Fill DP table
    for (int i = 1; i <= size_items; i++) {  // i is item index
        int weight = items[i-1].first;
        int value = items[i-1].second;
        for (int j = 1; j <= capacity; j++) {  // j is current capacity
            if (weight <= j && value + V[i-1][j - weight] > V[i-1][j]) {
                V[i][j] = value + V[i-1][j - weight];
                P[i][j] = j - weight;
            } else {
                V[i][j] = V[i-1][j];
                P[i][j] = j;
            }
        }
    }

    // Backtracking to print selected items
    int j = capacity;
    vector<int> selectedItems;
    for (int i = size_items; i >= 1; i--) {
        if (P[i][j] != j) { // item i-1 was included
            selectedItems.push_back(i-1);
            j = P[i][j];
        }
    }

    cout << "Selected item num: ";
    for (int num : selectedItems) cout << num + 1 << " ";
    cout << endl;

    return V[size_items][capacity];
}

long long InventorySystem::countStringPossibilities(string s) {
    if (s.size() < 2)
        return 1;
    long long dp1=1,dp2=1,current=1;
    for (int i = 1; i < s.size(); i++)
    {
        current = dp1;
        if (s[i] == s[i - 1] && (s[i] == 'u' || s[i] == 'n'))
        {
            current = (current + dp2);
        }
        dp2 = dp1;
        dp1 = current;
    }
    return current;
}

// =========================================================
// PART C: WORLD NAVIGATOR (Graphs)
// =========================================================
bool dfs(int start, vector<bool> &visited, vector<vector<int>> &adj, int dest)
{
    visited[start] = true;
    if (start == dest)
        return true;
    for (int i = 0; i < adj[start].size(); i++)
    {
        if (adj[start][i] == 1 && (!visited[i]))
        {
            if (dfs(i, visited, adj, dest))
                return true;
        }
    }
    return false;
}
bool WorldNavigator::pathExists(int n, vector<vector<int>>& edges, int source, int dest) {
    vector<bool> visited(n, false);
    edges = vector<vector<int>>(n, vector<int>(n, 0));
    cout << "Enter number of edges: ";
    cin >> n;
    int s, d;
    for (int i = 0; i < n; ++i)
    {
        cout << "Edge source: ";
        cin >> s;
        cout << "Edge destination: ";
        cin >> d;
        edges[s][d] = 1;
    }
    cout << "----------------------------------------" << endl;
    cout << "DFS Traversal: ";
    return dfs(source, visited, edges, dest);
}

long long WorldNavigator::minBribeCost(int n, int m, long long goldRate, long long silverRate,
                                       vector<vector<int>>& roadData) {
    // TODO: Implement Minimum Spanning Tree (Kruskal's or Prim's)
    // roadData[i] = {u, v, goldCost, silverCost}
    // Total cost = goldCost * goldRate + silverCost * silverRate
    // Return -1 if graph cannot be fully connected
    return -1;
}

string WorldNavigator::sumMinDistancesBinary(int n, vector<vector<int>>& roads) {
    // TODO: Implement All-Pairs Shortest Path (Floyd-Warshall)
    // Sum all shortest distances between unique pairs (i < j)
    // Return the sum as a binary string
    // Hint: Handle large numbers carefully
    return "0";
}

// =========================================================
// PART D: SERVER KERNEL (Greedy)
// =========================================================

int ServerKernel::minIntervals(vector<char>& tasks, int n) {
    // TODO: Implement task scheduler with cooling time
    // Same task must wait 'n' intervals before running again
    // Return minimum total intervals needed (including idle time)
    // Hint: Use greedy approach with frequency counting
    return 0;
}

// =========================================================
// FACTORY FUNCTIONS (Required for Testing)
// =========================================================

extern "C" {
    PlayerTable* createPlayerTable() { 
        return new ConcretePlayerTable(); 
    }

    Leaderboard* createLeaderboard() { 
        return new ConcreteLeaderboard(); 
    }

    AuctionTree* createAuctionTree() { 
        return new ConcreteAuctionTree(); 
    }
}
