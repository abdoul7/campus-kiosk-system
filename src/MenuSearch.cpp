#include "MenuSearch.hpp"
#include <iostream>

// =====================================================================
// Module 4: Menu Item Search & Management - BST implementation
// =====================================================================

MenuSearch::MenuSearch() {
    root = nullptr;
}

MenuSearch::~MenuSearch() {
    destroy(root);   // free every node before the object dies
}

// --- Insertion -------------------------------------------------------

// Recursively find the correct empty spot and hang a new node there.
// Smaller ids go left, larger ids go right; duplicate ids are rejected.
MenuSearch::Node* MenuSearch::insert(Node* node, const MenuItem& item) {
    if (node == nullptr)
        return new Node(item);

    if (item.itemId < node->data.itemId)
        node->left = insert(node->left, item);
    else if (item.itemId > node->data.itemId)
        node->right = insert(node->right, item);
    else
        std::cout << "[MenuSearch] Item ID " << item.itemId
                  << " already exists; not inserted.\n";

    return node;
}

void MenuSearch::insertItem(const MenuItem& item) {
    root = insert(root, item);
}

// --- Search ----------------------------------------------------------

// Standard iterative BST search: at each node go left or right based on
// how the target id compares to the current node's id.
MenuItem MenuSearch::searchById(int itemId, bool& found) const {
    Node* current = root;
    while (current != nullptr) {
        if (itemId == current->data.itemId) {
            found = true;
            return current->data;
        }
        current = (itemId < current->data.itemId) ? current->left : current->right;
    }
    found = false;
    return MenuItem{};
}

// Name is not the BST key, so we cannot use the ordering to prune -
// walk the whole tree until the first name match is found.
const MenuSearch::Node* MenuSearch::findByName(const Node* node,
                                               const std::string& name) const {
    if (node == nullptr)
        return nullptr;
    if (node->data.name == name)
        return node;

    const Node* leftResult = findByName(node->left, name);
    if (leftResult != nullptr)
        return leftResult;
    return findByName(node->right, name);
}

MenuItem MenuSearch::searchByName(const std::string& name, bool& found) const {
    const Node* result = findByName(root, name);
    if (result != nullptr) {
        found = true;
        return result->data;
    }
    found = false;
    return MenuItem{};
}

// In-order walk so matches print sorted by id; 'count' tracks matches.
void MenuSearch::printByCategory(const Node* node, const std::string& stallName,
                                 int& count) const {
    if (node == nullptr)
        return;

    printByCategory(node->left, stallName, count);
    if (node->data.stallName == stallName) {
        std::cout << "  [" << node->data.itemId << "] " << node->data.name
                  << " - RM" << node->data.price << "\n";
        count++;
    }
    printByCategory(node->right, stallName, count);
}

void MenuSearch::searchByCategory(const std::string& stallName) const {
    std::cout << "Items in category '" << stallName << "':\n";
    int count = 0;
    printByCategory(root, stallName, count);
    if (count == 0)
        std::cout << "  (no items found)\n";
}

// --- Management ------------------------------------------------------

// Locate the node by id (id is the key, so a normal descent) and
// overwrite its editable fields. The key itself is never changed.
bool MenuSearch::updateItem(int itemId, const std::string& newName,
                            const std::string& newStall, double newPrice) {
    Node* current = root;
    while (current != nullptr) {
        if (itemId == current->data.itemId) {
            current->data.name = newName;
            current->data.stallName = newStall;
            current->data.price = newPrice;
            return true;
        }
        current = (itemId < current->data.itemId) ? current->left : current->right;
    }
    return false;   // id not present
}

// Smallest id in a subtree lives at its left-most node.
MenuSearch::Node* MenuSearch::findMin(Node* node) const {
    while (node->left != nullptr)
        node = node->left;
    return node;
}

// Standard BST delete. Recurse toward the target, then handle the three
// classic cases at the node being removed.
MenuSearch::Node* MenuSearch::remove(Node* node, int itemId, bool& removed) {
    if (node == nullptr)
        return nullptr;

    if (itemId < node->data.itemId) {
        node->left = remove(node->left, itemId, removed);
    } else if (itemId > node->data.itemId) {
        node->right = remove(node->right, itemId, removed);
    } else {
        removed = true;

        // Case 1 & 2: no child or a single child - splice the node out.
        if (node->left == nullptr) {
            Node* rightChild = node->right;
            delete node;
            return rightChild;
        }
        if (node->right == nullptr) {
            Node* leftChild = node->left;
            delete node;
            return leftChild;
        }

        // Case 3: two children - copy the in-order successor's data up,
        // then delete that successor from the right subtree.
        Node* successor = findMin(node->right);
        node->data = successor->data;
        node->right = remove(node->right, successor->data.itemId, removed);
    }
    return node;
}

bool MenuSearch::removeItem(int itemId) {
    bool removed = false;
    root = remove(root, itemId, removed);
    return removed;
}

// --- Display ---------------------------------------------------------

// In-order traversal visits ids in ascending order -> sorted output.
void MenuSearch::inorderPrint(const Node* node) const {
    if (node == nullptr)
        return;

    inorderPrint(node->left);
    std::cout << "  [" << node->data.itemId << "] " << node->data.name
              << " (" << node->data.stallName << ") - RM"
              << node->data.price << "\n";
    inorderPrint(node->right);
}

void MenuSearch::displaySortedItems() const {
    if (root == nullptr) {
        std::cout << "  (menu is empty)\n";
        return;
    }
    inorderPrint(root);
}

// Post-order delete: free both children before the parent so we never
// touch a node after it has been freed.
void MenuSearch::destroy(Node* node) {
    if (node == nullptr)
        return;

    destroy(node->left);
    destroy(node->right);
    delete node;
}
