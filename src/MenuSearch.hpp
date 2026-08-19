#pragma once
#include "../include/Common.hpp"
#include <string>

// =====================================================================
// Module 4: Menu Item Search & Management
// Data Structure: BINARY SEARCH TREE (BST) - self-implemented
// Owner: Member 4
// =====================================================================
//
// Functional requirements:
//   - Store item details (ID, name, stall, price)
//   - Insert new menu items
//   - Search for items by ID / name / category (stall)
//   - Update or remove item records
//   - Display items in a structured, sorted format
//
// Why a BST: menu items are keyed on a unique itemId. A BST keeps them
// ordered by id, giving fast average-case O(log n) search/insert/remove
// and a free sorted listing via in-order traversal.
//
// INTEGRATION CONTRACT: Module 1 (OrderQueue) stores only itemId on an
// Order; main.cpp / other modules call searchById() to resolve the full
// item details when needed. The extra search/update methods below are
// additive - main.cpp still compiles unchanged.
//
// AI declaration: AI was used to clarify C++ syntax for recursive BST
// node deletion (the two-child successor case). All logic is my own.

class MenuSearch {
public:
    MenuSearch();
    ~MenuSearch();

    // --- Insertion ---
    void insertItem(const MenuItem& item);

    // --- Search ---
    // Search by item ID (the BST key). 'found' is set to false when no
    // match exists - check it before using the returned MenuItem.
    MenuItem searchById(int itemId, bool& found) const;

    // Search by exact item name. Name is not the key, so this scans the
    // whole tree; returns the first match and sets 'found'.
    MenuItem searchByName(const std::string& name, bool& found) const;

    // Print every item belonging to a given category (stall name).
    void searchByCategory(const std::string& stallName) const;

    // --- Management ---
    // Update an existing item's name/stall/price (found by id).
    // Returns false if the id is not in the tree.
    bool updateItem(int itemId, const std::string& newName,
                    const std::string& newStall, double newPrice);

    // Remove an item by id. Returns false if the id is not found.
    bool removeItem(int itemId);

    // --- Display ---
    // In-order traversal -> items printed sorted by itemId.
    void displaySortedItems() const;

private:
    // One BST node holds a MenuItem plus links to its two subtrees.
    struct Node {
        MenuItem data;
        Node* left = nullptr;
        Node* right = nullptr;
        Node(const MenuItem& item) : data(item) {}
    };

    Node* root;

    // Recursive helpers (operate on subtrees rooted at 'node').
    Node* insert(Node* node, const MenuItem& item);
    Node* remove(Node* node, int itemId, bool& removed);
    Node* findMin(Node* node) const;                 // smallest node in a subtree
    const Node* findByName(const Node* node, const std::string& name) const;
    void printByCategory(const Node* node, const std::string& stallName, int& count) const;
    void inorderPrint(const Node* node) const;
    void destroy(Node* node);                        // post-order delete
};
