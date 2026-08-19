#pragma once
#include <string>

// =====================================================================
// Common.hpp
// Shared data types used across ALL modules.
// This is the "integration contract" for data — every member reads
// and writes these same structs, so field names/types must not be
// changed without agreement from the whole team.
// =====================================================================

enum class OrderStatus { PENDING, ASSIGNED, IN_PROGRESS, COMPLETED };
enum class StallStatus { AVAILABLE, BUSY, CLOSED };

// A single menu item (owned by Module 4 - MenuSearch / BST)
struct MenuItem {
    int itemId = 0;
    std::string name;
    std::string stallName;
    double price = 0.0;
};

// A student order (owned/moved between Module 1 -> Module 2)
struct Order {
    int orderId = 0;
    int studentId = 0;
    int itemId = 0;              // references MenuItem::itemId
    std::string assignedStall;   // filled in by StallAssignment
    OrderStatus status = OrderStatus::PENDING;
};

// A food stall (owned by Module 2 - StallAssignment / Circular Queue)
struct Stall {
    int stallId = 0;
    std::string name;            // e.g. "Malay", "Chinese", "Western"
    StallStatus status = StallStatus::AVAILABLE;
};

// A single recorded kiosk session step (owned by Module 3 - SessionHistory / Stack)
struct SessionStep {
    int stepId = 0;
    std::string description;     // e.g. "Browsed Menu", "Selected Item 101"
};
