
/**
 * main_test_Malk.cpp
 * Comprehensive Test Suite for ArcadiaEngine
 * this includes more testcases and edge cases for all parts A-D
 * this is a hellish test suite 
 * msh lel 3eyal el toto
 **/

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <functional>
#include <cassert>
#include <random>
#include <algorithm>
#include "ArcadiaEngine.h"

    using namespace std;

// ==========================================
// FACTORY FUNCTIONS (LINKING)
// ==========================================
// These link to the functions at the bottom of your .cpp file
extern "C" {
    PlayerTable* createPlayerTable();
    Leaderboard* createLeaderboard();
    AuctionTree* createAuctionTree();
}

// ==========================================
// TEST UTILITIES
// ==========================================
class StudentTestRunner {
    int count = 0;
    int passed = 0;
    int failed = 0;

public:
    void runTest(string testName, bool condition) {
        count++;
        cout << "TEST: " << left << setw(70) << testName;
        if (condition) {
            cout << "[ PASS ]";
            passed++;
        }
        else {
            cout << "[ FAIL ]";
            failed++;
        }
        cout << endl;
    }

    void printSummary() {
        cout << "\n==========================================" << endl;
        cout << "SUMMARY: Passed: " << passed << " | Failed: " << failed << endl;
        cout << "==========================================" << endl;
        cout << "TOTAL TESTS: " << count << endl;
        if (failed == 0) {
            cout << "Great job! All tests passed." << endl;
            cout << "Your implementation handles both basic scenarios and edge cases correctly!" << endl;
        }
        else {
            cout << "Some tests failed. Check your logic carefully." << endl;
        }
    }
};

StudentTestRunner runner;

// ==========================================
// PART A: DATA STRUCTURES - EXTENDED TESTS
// ==========================================

void test_PartA_DataStructures() {
    cout << "\n--- Part A: Data Structures (Extended Tests) ---" << endl;

    // 1. PlayerTable (Double Hashing)
    PlayerTable* table = createPlayerTable();

    // Basic Insert and Search
    runner.runTest("PlayerTable: Basic Insert and Search", [&]() {
        table->insert(101, "Alice");
        return table->search(101) == "Alice";
        }());

    // Edge Case: Collision Handling
    runner.runTest("PlayerTable: Handle Collisions", [&]() {
        table->insert(101, "Alice");  // Same bucket as 202 (101 % 101 = 0, 202 % 101 = 0)
        table->insert(202, "Bob");
        return table->search(202) == "Bob";
        }());

    // Edge Case: Non-existent ID
    runner.runTest("PlayerTable: Search Non-existent ID", [&]() {
        string result = table->search(999);
        return result == "Not Found" || result == "";
        }());


    // Edge Case: Empty search
    PlayerTable* emptyTable = createPlayerTable();
    runner.runTest("PlayerTable: Search in Empty Table", [&]() {
        string result = emptyTable->search(101);
        return result == "Not Found" || result == "";
        }());
    delete emptyTable;

    delete table;

    // 2. Leaderboard (Skip List)
    Leaderboard* board = createLeaderboard();

    // Test A: Basic High Score
    runner.runTest("Leaderboard: Basic High Score", [&]() {
        board->addScore(1, 100);
        board->addScore(2, 200);
        vector<int> top = board->getTopN(1);
        return (!top.empty() && top[0] == 2);
        }());

    // Test B: Tie-Breaking (ID ordering)
    runner.runTest("Leaderboard: Tie-Break (ID 10 before ID 20)", [&]() {
        board->addScore(10, 500);
        board->addScore(20, 500);
        vector<int> top = board->getTopN(2);
        return (top.size() == 2 && top[0] == 10 && top[1] == 20);
        }());

    // Edge Case: Negative scores
    runner.runTest("Leaderboard: Negative Scores", [&]() {
        board->addScore(30, -100);
        board->addScore(40, -50);
        vector<int> top = board->getTopN(2);
        return (top.size() >= 1);
        }());

    // Edge Case: Update score for existing player
    runner.runTest("Leaderboard: Update Player Score", [&]() {
        board->addScore(50, 100);
        board->addScore(50, 300); // Update
        vector<int> top = board->getTopN(5);
        // Check if player 50 is in top (should be with score 300)
        auto it = find(top.begin(), top.end(), 50);
        return it != top.end();
        }());

    // Edge Case: Get more top players than exist
    runner.runTest("Leaderboard: Get Top N > Total Players", [&]() {
        Leaderboard* smallBoard = createLeaderboard();
        smallBoard->addScore(1, 100);
        smallBoard->addScore(2, 200);
        vector<int> top = smallBoard->getTopN(10);
        bool result = (top.size() == 2 && top[0] == 2 && top[1] == 1);
        delete smallBoard;
        return result;
        }());

    // Edge Case: Get top 0 players
    runner.runTest("Leaderboard: Get Top 0 Players", [&]() {
        vector<int> top = board->getTopN(0);
        return top.empty();
        }());

    // Edge Case: Remove non-existent player
    runner.runTest("Leaderboard: Remove Non-existent Player", [&]() {
        board->removePlayer(9999); // Should not crash
        return true;
        }());

    delete board;

    // 3. AuctionTree (Red-Black Tree) - EXTENSIVE TESTS
    cout << "\n--- Red-Black Tree Comprehensive Tests ---" << endl;

    // Test 1: Basic Insertions
    runner.runTest("RBTree: Basic Insertions (No Crash)", [&]() {
        AuctionTree* tree = createAuctionTree();
        tree->insertItem(1, 100);
        tree->insertItem(2, 50);
        tree->insertItem(3, 150);
        tree->insertItem(4, 75);
        delete tree;
        return true;
        }());

    // Test 2: Insert with same prices (should use ID as tie-breaker)
    runner.runTest("RBTree: Insert Items with Same Price", [&]() {
        AuctionTree* tree = createAuctionTree();
        tree->insertItem(10, 100);
        tree->insertItem(20, 100); // Same price, different ID
        tree->insertItem(5, 100);  // Same price, different ID
        delete tree;
        return true;
        }());

    // Test 3: Insert in descending order
    runner.runTest("RBTree: Insert Descending Prices", [&]() {
        AuctionTree* tree = createAuctionTree();
        for (int i = 10; i >= 1; i--) {
            tree->insertItem(i, i * 10);
        }
        delete tree;
        return true;
        }());

    // Test 4: Insert in ascending order
    runner.runTest("RBTree: Insert Ascending Prices", [&]() {
        AuctionTree* tree = createAuctionTree();
        for (int i = 1; i <= 10; i++) {
            tree->insertItem(i, i * 10);
        }
        delete tree;
        return true;
        }());

    // Test 5: Insert random order
    runner.runTest("RBTree: Insert Random Order", [&]() {
        AuctionTree* tree = createAuctionTree();
        vector<pair<int, int>> items = {
            {5, 500}, {2, 200}, {8, 800}, {1, 100},
            {9, 900}, {3, 300}, {6, 600}, {4, 400}, {7, 700}
        };
        for (auto& item : items) {
            tree->insertItem(item.first, item.second);
        }
        delete tree;
        return true;
        }());

    // Test 6: Large number of insertions
    runner.runTest("RBTree: Large Insertion (100 items)", [&]() {
        AuctionTree* tree = createAuctionTree();
        for (int i = 1; i <= 100; i++) {
            tree->insertItem(i, rand() % 1000);
        }
        delete tree;
        return true;
        }());

    // Test 7: Delete non-existent item
    runner.runTest("RBTree: Delete Non-existent Item", [&]() {
        AuctionTree* tree = createAuctionTree();
        tree->insertItem(1, 100);
        tree->deleteItem(999); // Should not crash
        delete tree;
        return true;
        }());

    // Test 8: Insert then delete
    runner.runTest("RBTree: Insert and Delete Single Item", [&]() {
        AuctionTree* tree = createAuctionTree();
        tree->insertItem(1, 100);
        tree->deleteItem(1);
        // Try to delete again (should not crash)
        tree->deleteItem(1);
        delete tree;
        return true;
        }());

    // Test 9: Complex delete scenarios
    runner.runTest("RBTree: Complex Delete Scenarios", [&]() {
        AuctionTree* tree = createAuctionTree();
        // Create a more complex tree
        tree->insertItem(50, 500);
        tree->insertItem(30, 300);
        tree->insertItem(70, 700);
        tree->insertItem(20, 200);
        tree->insertItem(40, 400);
        tree->insertItem(60, 600);
        tree->insertItem(80, 800);

        // Delete leaf node (20)
        tree->deleteItem(20);

        // Delete node with one child (30 now has only right child 40)
        tree->deleteItem(30);

        // Delete node with two children (50)
        tree->deleteItem(50);

        // Delete root
        tree->deleteItem(70);

        delete tree;
        return true;
        }());

    // Test 10: Delete all nodes
    runner.runTest("RBTree: Delete All Nodes", [&]() {
        AuctionTree* tree = createAuctionTree();
        for (int i = 1; i <= 20; i++) {
            tree->insertItem(i, i * 50);
        }

        // Delete in random order
        vector<int> ids = { 10, 5, 15, 3, 7, 12, 18, 1, 4, 6, 8, 11, 13, 16, 19, 2, 9, 14, 17, 20 };
        for (int id : ids) {
            tree->deleteItem(id);
        }

        // Tree should be empty now
        delete tree;
        return true;
        }());

    // Test 11: Red-Black Tree properties (color consistency)
    runner.runTest("RBTree: Stress Test with Random Operations", [&]() {
        AuctionTree* tree = createAuctionTree();
        const int OPERATIONS = 100;

        for (int i = 0; i < OPERATIONS; i++) {
            int op = rand() % 3;
            int id = rand() % 50 + 1;
            int price = rand() % 1000;

            if (op == 0) {
                tree->insertItem(id, price);
            }
            else if (op == 1) {
                tree->deleteItem(id);
            }
            // op == 2: do nothing (just testing stability)
        }

        delete tree;
        return true;
        }());

    // Test 12: Duplicate IDs with different prices
    runner.runTest("RBTree: Duplicate ID Insertion", [&]() {
        AuctionTree* tree = createAuctionTree();
        tree->insertItem(1, 100);
        tree->insertItem(1, 200); // Same ID, different price
        // Behavior depends on implementation - should not crash
        delete tree;
        return true;
        }());
}

// ==========================================
// PART B: INVENTORY SYSTEM - EXTENDED TESTS
// ==========================================

void test_PartB_Inventory() {
    cout << "\n--- Part B: Inventory System (Extended Tests) ---" << endl;

    // 1. Loot Splitting (Partition)
    runner.runTest("LootSplit: {1, 2, 4} -> Diff 1", [&]() {
        vector<int> coins = { 1, 2, 4 };
        return InventorySystem::optimizeLootSplit(3, coins) == 1;
        }());

    // Edge Case: Empty coins
    runner.runTest("LootSplit: Empty coins -> Diff 0", [&]() {
        vector<int> coins = {};
        return InventorySystem::optimizeLootSplit(0, coins) == 0;
        }());

    // Edge Case: Single coin
    runner.runTest("LootSplit: Single coin {5} -> Diff 5", [&]() {
        vector<int> coins = { 5 };
        return InventorySystem::optimizeLootSplit(1, coins) == 5;
        }());

    // Edge Case: All coins equal
    runner.runTest("LootSplit: All equal {3, 3, 3} -> Diff 3", [&]() {
        vector<int> coins = { 3, 3, 3 };
        return InventorySystem::optimizeLootSplit(3, coins) == 3;
        }());

    // Edge Case: Large values
    runner.runTest("LootSplit: Large values {1000, 500, 500} -> Diff 0", [&]() {
        vector<int> coins = { 1000, 500, 500 };
        return InventorySystem::optimizeLootSplit(3, coins) == 0;
        }());

    // 2. Inventory Packer (Knapsack)
    runner.runTest("Knapsack: Cap 10, All Fit -> Value 60", [&]() {
        vector<pair<int, int>> items = { {1, 10}, {2, 20}, {3, 30} };
        return InventorySystem::maximizeCarryValue(10, items) == 60;
        }());

    // Edge Case: Empty items
    runner.runTest("Knapsack: Empty items -> Value 0", [&]() {
        vector<pair<int, int>> items = {};
        return InventorySystem::maximizeCarryValue(10, items) == 0;
        }());

    // Edge Case: Zero capacity
    runner.runTest("Knapsack: Zero capacity -> Value 0", [&]() {
        vector<pair<int, int>> items = { {1, 10}, {2, 20} };
        return InventorySystem::maximizeCarryValue(0, items) == 0;
        }());

    // Edge Case: Single item fits
    runner.runTest("Knapsack: Single item fits", [&]() {
        vector<pair<int, int>> items = { {5, 50} };
        return InventorySystem::maximizeCarryValue(10, items) == 50;
        }());

    // Edge Case: Single item doesn't fit
    runner.runTest("Knapsack: Single item doesn't fit", [&]() {
        vector<pair<int, int>> items = { {15, 150} };
        return InventorySystem::maximizeCarryValue(10, items) == 0;
        }());

    // 3. Chat Autocorrect (String DP)
    runner.runTest("ChatDecoder: 'uu' -> 2 Possibilities", [&]() {
        return InventorySystem::countStringPossibilities("uu") == 2;
        }());

    // Edge Case: Empty string
    runner.runTest("ChatDecoder: Empty string -> 1", [&]() {
        return InventorySystem::countStringPossibilities("") == 1;
        }());

    // Edge Case: Single character
    runner.runTest("ChatDecoder: Single char 'n' -> 1", [&]() {
        return InventorySystem::countStringPossibilities("n") == 1;
        }());

    // Edge Case: No doubles
    runner.runTest("ChatDecoder: 'abc' -> 1", [&]() {
        return InventorySystem::countStringPossibilities("abc") == 1;
        }());

    // Edge Case: Multiple doubles
    runner.runTest("ChatDecoder: 'uuuu' -> 5", [&]() {
        return InventorySystem::countStringPossibilities("uuuu") == 5;
        }());

    // Edge Case: Mixed doubles
    runner.runTest("ChatDecoder: 'uunn' -> 4", [&]() {
        return InventorySystem::countStringPossibilities("uunn") == 4;
        }());
}

// ==========================================
// PART C: WORLD NAVIGATOR - EXTENDED TESTS
// ==========================================

void test_PartC_Navigator() {
    cout << "\n--- Part C: World Navigator (Extended Tests) ---" << endl;

    // 1. Safe Passage (Path Exists)
    runner.runTest("PathExists: 0->1->2 -> True", [&]() {
        vector<vector<int>> edges = { {0, 1}, {1, 2} };
        return WorldNavigator::pathExists(3, edges, 0, 2) == true;
        }());

    // Edge Case: No path exists
    runner.runTest("PathExists: Disconnected graph -> False", [&]() {
        vector<vector<int>> edges = { {0, 1}, {2, 3} };
        return WorldNavigator::pathExists(4, edges, 0, 3) == false;
        }());

    // Edge Case: Same source and destination
    runner.runTest("PathExists: Same node -> True", [&]() {
        vector<vector<int>> edges = { {0, 1}, {1, 2} };
        return WorldNavigator::pathExists(3, edges, 0, 0) == true;
        }());

    // Edge Case: Empty graph
    runner.runTest("PathExists: Empty graph -> False", [&]() {
        vector<vector<int>> edges = {};
        return WorldNavigator::pathExists(3, edges, 0, 2) == false;
        }());

    // Edge Case: Invalid node indices
    runner.runTest("PathExists: Invalid source -> False", [&]() {
        vector<vector<int>> edges = { {0, 1} };
        return WorldNavigator::pathExists(2, edges, 5, 1) == false;
        }());

    // 2. The Bribe (MST)
    runner.runTest("MinBribeCost: Triangle Graph -> Cost 15", [&]() {
        vector<vector<int>> roads = {
            {0, 1, 10, 0},
            {1, 2, 5, 0},
            {0, 2, 20, 0}
        };
        return WorldNavigator::minBribeCost(3, 3, 1, 1, roads) == 15;
        }());

    // Edge Case: Disconnected graph
    runner.runTest("MinBribeCost: Disconnected -> -1", [&]() {
        vector<vector<int>> roads = {
            {0, 1, 10, 0},
            {2, 3, 5, 0}
        };
        return WorldNavigator::minBribeCost(4, 2, 1, 1, roads) == -1;
        }());

    // Edge Case: Single node
    runner.runTest("MinBribeCost: Single node -> 0", [&]() {
        vector<vector<int>> roads = {};
        return WorldNavigator::minBribeCost(1, 0, 1, 1, roads) == 0;
        }());

    // Edge Case: Zero rates
    runner.runTest("MinBribeCost: Zero rates", [&]() {
        vector<vector<int>> roads = {
            {0, 1, 10, 5},
            {1, 2, 5, 2}
        };
        return WorldNavigator::minBribeCost(3, 2, 0, 0, roads) == 0;
        }());

    // 3. Teleporter (Binary Sum APSP)
    runner.runTest("BinarySum: Line Graph -> '110'", [&]() {
        vector<vector<int>> roads = {
            {0, 1, 1},
            {1, 2, 2}
        };
        return WorldNavigator::sumMinDistancesBinary(3, roads) == "110";
        }());

    // Edge Case: Single node
    runner.runTest("BinarySum: Single node -> '0'", [&]() {
        vector<vector<int>> roads = {};
        return WorldNavigator::sumMinDistancesBinary(1, roads) == "0";
        }());

    // Edge Case: Disconnected nodes (infinite distance)
    runner.runTest("BinarySum: Disconnected nodes", [&]() {
        vector<vector<int>> roads = {
            {0, 1, 5}
        };
        // Only pairs: (0,1)=5, (0,2)=INF, (1,2)=INF
        // Sum = 5 = "101" in binary
        return WorldNavigator::sumMinDistancesBinary(3, roads) == "101";
        }());

    // Edge Case: Complete graph with zero distances
    runner.runTest("BinarySum: All zero distances -> '0'", [&]() {
        vector<vector<int>> roads = {
            {0, 1, 0},
            {0, 2, 0},
            {1, 2, 0}
        };
        return WorldNavigator::sumMinDistancesBinary(3, roads) == "0";
        }());
}

// ==========================================
// PART D: SERVER KERNEL - EXTENDED TESTS
// ==========================================

void test_PartD_Kernel() {
    cout << "\n--- Part D: Server Kernel (Extended Tests) ---" << endl;

    // Basic test from PDF
    runner.runTest("Scheduler: {A, A, B}, n=2 -> 4 Intervals", [&]() {
        vector<char> tasks = { 'A', 'A', 'B' };
        return ServerKernel::minIntervals(tasks, 2) == 4;
        }());

    // Edge Case: Empty tasks
    runner.runTest("Scheduler: Empty tasks -> 0", [&]() {
        vector<char> tasks = {};
        return ServerKernel::minIntervals(tasks, 2) == 0;
        }());

    // Edge Case: Single task
    runner.runTest("Scheduler: Single task -> 1", [&]() {
        vector<char> tasks = { 'A' };
        return ServerKernel::minIntervals(tasks, 5) == 1;
        }());

    // Edge Case: All tasks different, n=0
    runner.runTest("Scheduler: All different, n=0 -> tasks.size()", [&]() {
        vector<char> tasks = { 'A', 'B', 'C', 'D' };
        return ServerKernel::minIntervals(tasks, 0) == 4;
        }());

    // Edge Case: Many same tasks, large n
    runner.runTest("Scheduler: Many A's, large n", [&]() {
        vector<char> tasks = { 'A', 'A', 'A', 'A', 'A', 'A', 'A' };
        // 7 A's, n=2
        // A _ _ A _ _ A _ _ A _ _ A _ _ A _ _ A
        // Expected: 7 + (6 * 2) = 7 + 12 = 19 intervals
        return ServerKernel::minIntervals(tasks, 2) == 19;
        }());

    // Edge Case: Tasks that don't need idle time
    runner.runTest("Scheduler: No idle needed", [&]() {
        vector<char> tasks = { 'A', 'B', 'C', 'D', 'E', 'F' };
        // All different, n=5, should take exactly 6 intervals
        return ServerKernel::minIntervals(tasks, 5) == 6;
        }());

    // Edge Case: Mixed frequencies
    runner.runTest("Scheduler: Mixed frequencies", [&]() {
        vector<char> tasks = { 'A', 'A', 'A', 'B', 'B', 'C' };
        // 3 A's, 2 B's, 1 C, n=2
        // A B C A B idle A
        // Expected: 7 intervals
        return ServerKernel::minIntervals(tasks, 2) == 7;
        }());
}

// ==========================================
// PERFORMANCE STRESS TESTS
// ==========================================

void test_Performance() {
    cout << "\n--- Performance Stress Tests ---" << endl;

    // Large RBTree test
    runner.runTest("RBTree: Stress Test (1000 operations)", [&]() {
        AuctionTree* tree = createAuctionTree();
        const int N = 1000;

        // Insert many items
        for (int i = 0; i < N; i++) {
            tree->insertItem(i, rand() % 10000);
        }

        // Delete some random items
        for (int i = 0; i < N / 2; i++) {
            tree->deleteItem(rand() % N);
        }

        delete tree;
        return true;
        }());

    // Large Knapsack test
    runner.runTest("Knapsack: Large Input (50 items)", [&]() {
        vector<pair<int, int>> items;
        for (int i = 0; i < 50; i++) {
            items.push_back({ rand() % 100 + 1, rand() % 1000 + 1 });
        }
        int result = InventorySystem::maximizeCarryValue(500, items);
        return result >= 0; // Just check it doesn't crash
        }());

    // Large graph test (path exists)
    runner.runTest("PathExists: Large Graph (100 nodes)", [&]() {
        vector<vector<int>> edges;
        // Create a chain 0-1-2-...-99
        for (int i = 0; i < 99; i++) {
            edges.push_back({ i, i + 1 });
        }
        return WorldNavigator::pathExists(100, edges, 0, 99) == true;
        }());
}

int main() {
    cout << "Arcadia Engine - Comprehensive Test Suite" << endl;
    cout << "=========================================" << endl;
    cout << "Includes basic tests, edge cases, and performance tests." << endl;

    // Seed random number generator for consistent tests
    srand(42);

    test_PartA_DataStructures();
    test_PartB_Inventory();
    test_PartC_Navigator();
    test_PartD_Kernel();
    test_Performance();

    runner.printSummary();

    return 0;
}