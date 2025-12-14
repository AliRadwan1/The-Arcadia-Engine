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
    struct Player
    {
        int playerID;
        string name;
        bool busy; // To handle busy entries in open addressing

        Player(int id = -1, string n = "", bool del = false){
            playerID=id; name=n; busy=del; 
        }
    };

    Player table[101];

public:
    void insert(int playerID, string name) override {
        int hash1 = playerID % 101;
        int hash2 = 97 - (playerID % 97);
        if(hash2 == 0) hash2 = 1;
        for(int i = 0; i < 101; i++){
            int index = (hash1 + (long long)i * hash2) % 101;
            if(!table[index].busy){
                table[index] = Player(playerID, name, true);
                return;
            }
        }
        cout << "Hash Table Full, cannot insert player " << playerID << endl;
    }

    string search(int playerID) override {
        int hash1 = playerID % 101;
        int hash2 = 97 - (playerID % 97);
        if(hash2 == 0) hash2 = 1;
        for(int i = 0; i < 101; i++){
            int index = (hash1 + (long long)i * hash2) % 101;
            if(table[index].busy && table[index].playerID == playerID){
                return table[index].name;
            }
            if(!table[index].busy){
                return "Not Found";
            }
        }
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
    struct RBNode {
        int id;
        int price;
        char color; // 'R' or 'B'
        RBNode *left, *right, *parent;
        RBNode(int id, int p) : id(id), price(p), color('R'), left(nullptr), right(nullptr), parent(nullptr) {}
    };

    RBNode* root;

    // helper function: Left Rotation
    void rotateLeft(RBNode*& node)
    {
        RBNode* child = node->right;
        node->right = child->left;
        if (node->right != nullptr)
            node->right->parent = node;
        child->parent = node->parent;
        if (node->parent == nullptr)
            root = child;
        else if (node == node->parent->left)
            node->parent->left = child;
        else
            node->parent->right = child;
        child->left = node;
        node->parent = child;
    }

    // helper function: Right Rotation
    void rotateRight(RBNode*& node)
    {
        RBNode* child = node->left;
        node->left = child->right;
        if (node->left != nullptr)
            node->left->parent = node;
        child->parent = node->parent;
        if (node->parent == nullptr)
            root = child;
        else if (node == node->parent->left)
            node->parent->left = child;
        else
            node->parent->right = child;
        child->right = node;
        node->parent = child;
    }

public:
    ConcreteAuctionTree() {
        // TODO: Initialize your Red-Black Tree
        root = nullptr;
        root->color = 'B'; // root must be always black
    }

    void insertItem(int itemID, int price) override {
        // TODO: Implement Red-Black Tree insertion
        // Remember to maintain RB-Tree properties with rotations and recoloring
        RBNode* newNode = new RBNode(itemID,price);
        RBNode* current = root;
        RBNode* parent = nullptr;

        while (current != nullptr){
            parent = current;
            if (newNode->price < current->price){
                current = current->left;
            }
            else{
                current = current->right;
            }
        }

        newNode->parent = parent;

        if (parent == nullptr){
            root = newNode;
        }
        else if (newNode->price < parent->price){
            parent->left = newNode;
        }
        else{
            parent->right = newNode;
        }

        if (newNode->parent == nullptr){ //case 0 : new node is root
            newNode->color = 'B';
            return;
        }

        else if (newNode->parent->color == 'B'){ //case 1 : parent is black doesn't violate any rules
            return;
        }

        //fixing loop
        RBNode* p = nullptr;
        RBNode* g = nullptr;
        while (newNode != root && newNode->color == 'R' && newNode->parent->color == 'R') {
            p = newNode->parent ;
            g = p->parent;
            if(p == g->left){ // if newNode's parent is the left child of its parent
                RBNode* unc = g->right;
                if(unc != nullptr && unc->color == 'R'){ //case 2: parent is red & uncle is red
                    p->color = 'B';
                    unc->color = 'B';
                    g->color = 'R';
                    newNode = g;
                }
                else{ //case 3: parent is red but uncle is black
                    if (newNode == p->right) {
                        rotateLeft(p);
                        newNode = p;
                        p = newNode->parent;
                    }
                    rotateRight(g);
                    p->color = 'B';
                    g->color = 'R';
                    newNode = p;
                }
            }
            else{
                RBNode* unc = g->left;
                if(unc != nullptr && unc->color == 'R'){ //case 2: parent is red & uncle is red
                    p->color = 'B';
                    unc->color = 'B';
                    g->color = 'R';
                    newNode = g;
                }
                else{ //case 3: parent is red but uncle is black
                    if (newNode == p->right) {
                        rotateRight(p);
                        newNode = p;
                        p = newNode->parent;
                    }
                    rotateLeft(g);
                    p->color = 'B';
                    g->color = 'R';
                    newNode = p;
                }
            }
            root->color = 'B';
 }
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
    int sum = accumulate(coins.begin(), coins.end(), 0); // Total sum of coins
    int mid = sum / 2; 
    vector<bool> arr(mid + 1, false); // intialize array of bool values wit size mid+1 with false
    // if we can make sum j with subset of coins dp[j] = true
    arr[0] = true;  

    for (int coin : coins) {
        // from a mid decreasing to coin to avoid recomputation
        for (int j = mid; j >= coin; j--) {
            if (arr[j - coin]) {
                arr[j] = true;
            }
        }
    }

    // find the closest sum to mid
    int closest = 0;
    for (int j = mid; j >= 0; j--) {
        if (arr[j]) {
            closest = j;
            break;
        }
    }

    return sum - 2 * closest;
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

void sort_edges(vector<vector<long long>>& edges)
{
    // sort => O(NlogN) == O(ElogE)
    sort(edges.begin(), edges.end(),
        [](const vector<long long>& a, vector<long long>& b) {
            return a[2] < b[2]; // Comparisson by cost
        }
    );
}

// Helper function to check which group a vertex belongs to
int find_group(int i, vector<int>& parent)
{
//    while (true)
//    {
//        if (parent[i] == i)
//        {
//            parent[p] = i;
//            break;
//        }
//        i = parent[i];
//    }
    
    /*
        implementing using the idea of Path Comparison

        the idea is to save steps when checking for the group
        for example if 0 -> 1 -> 2 -> 4
        instead of each time taking 3 setps when checkin for node 0
        we save the last value so we only take one step
        0 -> 4

        average Time = O(1)
    */
    
    if (parent[i] == i)
    {
        return i;
    }
    // Path comparison: Points directly to the root
    parent[i] = find_group(parent[i], parent);

    return parent[i];
}

// Implemented using Kruskal's algorithm
long long WorldNavigator::minBribeCost(int n, int m, long long goldRate, long long silverRate,
                                       vector<vector<int>>& roadData) {
    // n (Nodes/Vertices): number of distinct locations "Cities"
    // m (Edges): number of availabel roads connecting those 
    // Total cost per road (Edge) = goldCost * goldRate + silverCost * silverRate
    // Return -1 if graph cannot be fully connected

    // 1. Convert inputs to edges with calculate weights
    vector<vector<long long>> edges;
    for (vector<int>& r : roadData)
    {
        long long cost = (r[2] * goldRate) + (r[3] * silverRate);
        edges.push_back({ r[0],r[1],cost });
    }

    // 2. Sort edges by cost (Cheapest first)
    sort_edges(edges);

    // 3. Initialize Union-Find (Disjoint Set)
    // same idea of makeing number of v sets then union them
    vector<int> parent(n);
    iota(parent.begin(), parent.end(), 0); // fill parent with sequentially increasing values (equal to loop)
    
    // 4. Calculate the minimum cost
    long long total_cost = 0;
    int MST_edges = 0;

    for (vector<long long>& e : edges)
    {
        int u = (int)e[0], v = (int)e[1];
        long long c = e[2];

        int rootU = find_group(u, parent);
        int rootV = find_group(v, parent);

        // Checking if u and v are not in the same group to prevent cycling
        if (rootU != rootV)
        {
            total_cost += c;
            parent[rootU] = rootV; // Union operation
            MST_edges++;
        }
    }
    // The minimum number of edges required to be fully connected is (V - 1)
    return (MST_edges < n - 1) ? -1 : total_cost;
}
// =============================================================================================
// Initializes the Distance matrix
void initialize_distance_matrix(vector<vector<long long>>& D, const vector<vector<int>>& roads)
{
    // initialize distance matrix with all values set to INFINITY
    for (int i = 0; i < D.size(); i++)
    {
        for (int j = 0; j < D.size(); j++)
        {
            D[i][j] = LLONG_MAX;
        }
    }

    for (int i = 0; i < D.size(); i++) { D[i][i] = 0; } // each city to itself is 0 distance

    // assign the values for each road(edge) between the cities(vertices)
    for (vector<int> r : roads)
    {
        int u = r[0], v = r[1], d = r[2];
        D[u][v] = d;
    }
}
// Floyd-Warshall algorithm
void floydWarshall(vector<vector<long long>>& D)
{
    int v = D.size();

    // Loop over all the intermediate vertices 
    for (int k = 0; k < v; k++)
    {
        // Pick each city(vertices) as the source
        for (int i = 0; i < v; i++)
        {
            // Pick each city(vertices) as the distination for all the picked sources
            for (int j = 0; j < v; j++)
            {
                if (D[i][k] != LLONG_MAX && D[k][j] != LLONG_MAX) // K is intermediate between i & j
                {
                    D[i][j] = min(D[i][j], D[i][k] + D[k][j]);
                }
            }
        }
    }
}
// Helper function to convert the result to binary representation
string decToBinary(long long sum)
{
    string bin = "";

    while (sum > 0)
    {
        int bit = sum % 2;
        bin.push_back('0' + bit);
        sum /= 2;
    }
    reverse(bin.begin(), bin.end());

    return bin;
}
// Implemented using Floyd-Warshall algorithm
string WorldNavigator::sumMinDistancesBinary(int n, vector<vector<int>>& roads) {
    // Sum all shortest distances between unique pairs (i < j)

    vector<vector<long long>> D(n, vector<long long>(n)); // Distance matrix

    initialize_distance_matrix(D, roads);

    floydWarshall(D);

    // Calculate the sum
    long long sum = 0;
    for (int i = 0; i < D.size(); i++)
    {
        for (int j = i + 1; j < D.size(); j++)
        {
            if (D[i][j] != LLONG_MAX) // if the two cities are connected through a path
            {
                sum += D[i][j];
            }
        }
    }

    // Return the sum as a binary string
    string result = (sum != 0) ? decToBinary(sum) : "0";

    return result;
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
