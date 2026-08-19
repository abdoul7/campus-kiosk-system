#include "FoodCourtLayout.hpp"
#include <cctype>
#include <iostream>

// =====================================================================
// Module 5: Food Court Layout & Navigation using a general tree.
// =====================================================================

FoodCourtLayout::FoodCourtLayout()
    : root(new TreeNode("Food Court")) {
}

FoodCourtLayout::~FoodCourtLayout() {
    deleteTree(root);
    root = nullptr;
}

bool FoodCourtLayout::isBlank(const std::string& text) const {
    for (char character : text) {
        if (!std::isspace(static_cast<unsigned char>(character))) {
            return false;
        }
    }
    return true;
}

FoodCourtLayout::TreeNode* FoodCourtLayout::findNode(
    TreeNode* node, const std::string& name) const {

    if (node == nullptr) {
        return nullptr;
    }

    if (node->name == name) {
        return node;
    }

    TreeNode* result = findNode(node->firstChild, name);
    if (result != nullptr) {
        return result;
    }

    return findNode(node->nextSibling, name);
}

void FoodCourtLayout::deleteTree(TreeNode* node) {
    if (node == nullptr) {
        return;
    }

    deleteTree(node->firstChild);
    deleteTree(node->nextSibling);
    delete node;
}

void FoodCourtLayout::addLocation(const std::string& parentName,
                                  const std::string& locationName) {
    if (isBlank(locationName)) {
        std::cout << "  Error: location name cannot be empty.\n";
        return;
    }

    // Location names are kept unique so route searches are unambiguous.
    if (findNode(root, locationName) != nullptr) {
        std::cout << "  Error: location '" << locationName
                  << "' already exists.\n";
        return;
    }

    const std::string actualParent = isBlank(parentName)
                                         ? root->name
                                         : parentName;

    TreeNode* parent = findNode(root, actualParent);
    if (parent == nullptr) {
        std::cout << "  Error: parent location '" << actualParent
                  << "' was not found.\n";
        return;
    }

    TreeNode* newLocation = new TreeNode(locationName);

    if (parent->firstChild == nullptr) {
        parent->firstChild = newLocation;
    } else {
        TreeNode* lastChild = parent->firstChild;
        while (lastChild->nextSibling != nullptr) {
            lastChild = lastChild->nextSibling;
        }
        lastChild->nextSibling = newLocation;
    }
}

int FoodCourtLayout::countNodes(const TreeNode* node) const {
    if (node == nullptr) {
        return 0;
    }

    return 1 + countNodes(node->firstChild)
             + countNodes(node->nextSibling);
}

bool FoodCourtLayout::buildPath(TreeNode* node, const std::string& target,
                                TreeNode** path, int depth,
                                int& pathLength) const {
    if (node == nullptr) {
        return false;
    }

    path[depth] = node;

    if (node->name == target) {
        pathLength = depth + 1;
        return true;
    }

    TreeNode* child = node->firstChild;
    while (child != nullptr) {
        if (buildPath(child, target, path, depth + 1, pathLength)) {
            return true;
        }
        child = child->nextSibling;
    }

    return false;
}

void FoodCourtLayout::printRoute(const std::string& from,
                                 const std::string& to) const {
    if (isBlank(from) || isBlank(to)) {
        std::cout << "  Error: both route locations must be provided.\n";
        return;
    }

    const int totalNodes = countNodes(root);
    TreeNode** fromPath = new TreeNode*[totalNodes];
    TreeNode** toPath = new TreeNode*[totalNodes];

    int fromLength = 0;
    int toLength = 0;

    const bool fromFound = buildPath(root, from, fromPath, 0, fromLength);
    const bool toFound = buildPath(root, to, toPath, 0, toLength);

    if (!fromFound || !toFound) {
        std::cout << "  Error: ";
        if (!fromFound && !toFound) {
            std::cout << "locations '" << from << "' and '" << to
                      << "' were not found.\n";
        } else if (!fromFound) {
            std::cout << "location '" << from << "' was not found.\n";
        } else {
            std::cout << "location '" << to << "' was not found.\n";
        }

        delete[] fromPath;
        delete[] toPath;
        return;
    }

    // Find the final shared node in both root-to-location paths.
    int commonIndex = 0;
    const int shorterLength = (fromLength < toLength) ? fromLength : toLength;
    while (commonIndex < shorterLength
           && fromPath[commonIndex] == toPath[commonIndex]) {
        commonIndex++;
    }
    const int lowestCommonAncestor = commonIndex - 1;

    std::cout << "  Route: ";
    bool firstPrinted = true;

    // Move upwards from the source to the lowest common ancestor.
    for (int index = fromLength - 1;
         index >= lowestCommonAncestor;
         --index) {
        if (!firstPrinted) {
            std::cout << " -> ";
        }
        std::cout << fromPath[index]->name;
        firstPrinted = false;
    }

    // Move downwards from the ancestor's child to the destination.
    for (int index = lowestCommonAncestor + 1;
         index < toLength;
         ++index) {
        if (!firstPrinted) {
            std::cout << " -> ";
        }
        std::cout << toPath[index]->name;
        firstPrinted = false;
    }

    std::cout << "\n";

    delete[] fromPath;
    delete[] toPath;
}

void FoodCourtLayout::displayTree(const TreeNode* node, int depth) const {
    if (node == nullptr) {
        return;
    }

    for (int level = 0; level < depth; ++level) {
        std::cout << "  ";
    }

    if (depth > 0) {
        std::cout << "|- ";
    }
    std::cout << node->name << "\n";

    displayTree(node->firstChild, depth + 1);
    displayTree(node->nextSibling, depth);
}

void FoodCourtLayout::displayLayout() const {
    if (root == nullptr) {
        std::cout << "  The food court layout is empty.\n";
        return;
    }

    std::cout << "\n  Food Court Layout\n"
              << "  -----------------\n";
    displayTree(root, 0);
}
