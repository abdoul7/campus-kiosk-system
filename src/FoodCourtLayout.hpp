#pragma once
#include "../include/Common.hpp"
#include <string>

// =====================================================================
// Module 5 (OPTIONAL): Food Court Layout & Navigation
// Data Structure: TREE (general, not BST) - self-implemented
// Owner: Member 5
// =====================================================================

class FoodCourtLayout {
public:
    FoodCourtLayout();
    ~FoodCourtLayout();

    // Add a child location under a given parent (e.g. a stall under a zone).
    // Pass parentName == "" (or the root's own name) to add a top-level zone.
    void addLocation(const std::string& parentName, const std::string& locationName);

    // Print the route from one location to another.
    void printRoute(const std::string& from, const std::string& to) const;

    // Print the full layout tree.
    void displayLayout() const;

private:
    // First-child / next-sibling representation of a general tree.
    struct TreeNode {
        std::string name;
        TreeNode* firstChild;
        TreeNode* nextSibling;

        explicit TreeNode(const std::string& locationName)
            : name(locationName), firstChild(nullptr), nextSibling(nullptr) {}
    };

    TreeNode* root;

    TreeNode* findNode(TreeNode* node, const std::string& name) const;
    void deleteTree(TreeNode* node);
    void displayTree(const TreeNode* node, int depth) const;
    int countNodes(const TreeNode* node) const;

    // Stores root-to-target nodes in path and writes the number of entries
    // to pathLength when the target is found.
    bool buildPath(TreeNode* node, const std::string& target,
                   TreeNode** path, int depth, int& pathLength) const;

    bool isBlank(const std::string& text) const;
};
