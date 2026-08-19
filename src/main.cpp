// =====================================================================
// main.cpp - Campus Food Court Self-Order Kiosk System
// =====================================================================
// The console is presented the way a real kiosk works: a student scans
// their ID, browses/searches the menu, places an order, and can undo
// steps - while staff manage the menu, stalls and order queue behind a
// separate panel. Each kiosk action is driven by one of the modules:
//
//   Browse / search / manage menu ... Module 4 (BST)   <-- implemented
//   Place order -> order queue ....... Module 1 (Queue)
//   Order -> stall assignment ........ Module 2 (Circular Queue)
//   Undo / activity history .......... Module 3 (Stack)
//   Food court map ................... Module 5 (Tree, optional)
//
// Modules 1, 2, 3 and 5 are wired to their owners' classes; their data-
// structure logic is still a placeholder (stub) until each owner
// implements it - no change to this file will be needed when they do.
//
// AI declaration: AI was used to clarify C++ syntax for validating
// numeric console input. All program logic is my own work.
// =====================================================================

#include <iostream>
#include <string>
#include <cctype>
#include <cstdlib>
#include "../include/Common.hpp"
#include "OrderQueue.hpp"
#include "StallAssignment.hpp"
#include "SessionHistory.hpp"
#include "MenuSearch.hpp"
#include "FoodCourtLayout.hpp"

// Capacity of the circular-queue stall buffer (Member 2 may adjust).
static const int STALL_CAPACITY = 10;

// =====================================================================
// Input helpers - every prompt reads a full line and re-asks on bad
// input, keeping the kiosk robust against typos.
// =====================================================================

// Read one line of text; exit cleanly if the input stream is closed.
static std::string promptLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) {
        std::cout << "\nInput closed. Shutting down.\n";
        std::exit(0);
    }
    return line;
}

// Read a whole number, rejecting anything that is not a valid integer.
static int promptInt(const std::string& prompt) {
    while (true) {
        std::string line = promptLine(prompt);
        try {
            size_t pos = 0;
            int value = std::stoi(line, &pos);
            while (pos < line.size() && std::isspace((unsigned char)line[pos]))
                pos++;                       // allow trailing spaces only
            if (pos == line.size())
                return value;
        } catch (...) { /* fall through to error message */ }
        std::cout << "  Please enter a valid whole number.\n";
    }
}

// Read a price, rejecting non-numeric or negative values.
static double promptPrice(const std::string& prompt) {
    while (true) {
        std::string line = promptLine(prompt);
        try {
            size_t pos = 0;
            double value = std::stod(line, &pos);
            while (pos < line.size() && std::isspace((unsigned char)line[pos]))
                pos++;
            if (pos == line.size() && value >= 0.0)
                return value;
        } catch (...) { /* fall through */ }
        std::cout << "  Please enter a valid non-negative price.\n";
    }
}

static StallStatus promptStallStatus() {
    int s = promptInt("  Status (1=Available, 2=Busy, 3=Closed): ");
    switch (s) {
        case 2:  return StallStatus::BUSY;
        case 3:  return StallStatus::CLOSED;
        default: return StallStatus::AVAILABLE;
    }
}

// =====================================================================
// Formatting helpers
// =====================================================================

static void printItem(const MenuItem& item) {
    std::cout << "  [" << item.itemId << "] " << item.name
              << " (" << item.stallName << ") - RM" << item.price << "\n";
}

static std::string orderStatusName(OrderStatus s) {
    switch (s) {
        case OrderStatus::PENDING:     return "Pending";
        case OrderStatus::ASSIGNED:    return "Assigned";
        case OrderStatus::IN_PROGRESS: return "In Progress";
        case OrderStatus::COMPLETED:   return "Completed";
    }
    return "Unknown";
}

static void printOrder(const Order& o) {
    std::cout << "  Order #" << o.orderId << " | student " << o.studentId
              << " | item " << o.itemId << " | stall '" << o.assignedStall
              << "' | " << orderStatusName(o.status) << "\n";
}

// Record one action into the student's session history (Module 3).
static void logStep(SessionHistory& session, int& nextStepId, const std::string& desc) {
    session.recordStep({nextStepId++, desc});
}

// =====================================================================
// STUDENT-FACING: search the menu (Module 4)
// =====================================================================

static void searchMenu(MenuSearch& menu) {
    std::cout << "  Search by: 1=ID  2=Name  3=Category(stall)\n";
    int how = promptInt("  Choice: ");
    if (how == 1) {
        int id = promptInt("  Item ID: ");
        bool found = false;
        MenuItem item = menu.searchById(id, found);
        if (found) { std::cout << "  Found:\n"; printItem(item); }
        else       std::cout << "  No item with ID " << id << ".\n";
    } else if (how == 2) {
        std::string name = promptLine("  Item name: ");
        bool found = false;
        MenuItem item = menu.searchByName(name, found);
        if (found) { std::cout << "  Found:\n"; printItem(item); }
        else       std::cout << "  No item named '" << name << "'.\n";
    } else if (how == 3) {
        std::string stall = promptLine("  Category (stall): ");
        menu.searchByCategory(stall);
    } else {
        std::cout << "  Unknown search option.\n";
    }
}

// =====================================================================
// STUDENT-FACING: one full self-order session
// Ties together Modules 4 (menu), 1 (queue), 2 (stalls),
// 3 (history) and 5 (map) as the student navigates the kiosk.
// =====================================================================

static void studentSession(OrderQueue& orders, StallAssignment& stalls,
                           SessionHistory& session, MenuSearch& menu,
                           FoodCourtLayout& layout,
                           int& nextOrderId, int& nextStepId) {
    int studentId = promptInt("\nScan your student ID: ");
    logStep(session, nextStepId, "Scanned student ID");
    std::cout << "Welcome, student #" << studentId << "!\n";

    int choice;
    do {
        std::cout << "\n----- Kiosk: Student #" << studentId << " -----\n"
                  << "1. Browse the full menu\n"
                  << "2. Search for an item\n"
                  << "3. Place an order\n"
                  << "4. View the food court map\n"
                  << "5. Go back (undo my last step)\n"
                  << "6. View my activity history\n"
                  << "7. Finish and exit\n";
        choice = promptInt("Select an option: ");
        switch (choice) {
            case 1:
                std::cout << "  -- Today's Menu --\n";
                menu.displaySortedItems();
                logStep(session, nextStepId, "Browsed the menu");
                break;
            case 2:
                searchMenu(menu);
                logStep(session, nextStepId, "Searched the menu");
                break;
            case 3: {
                int itemId = promptInt("  Enter the item ID to order: ");
                bool found = false;
                MenuItem item = menu.searchById(itemId, found);
                if (!found) { std::cout << "  Sorry, no item with ID " << itemId << ".\n"; break; }

                // Build the order, assign a stall (Module 2), then queue it (Module 1).
                Order order{nextOrderId++, studentId, itemId, "", OrderStatus::PENDING};
                std::string stall = stalls.assignNextStall(order);
                orders.enqueueOrder(order);

                std::cout << "  Order #" << order.orderId << " placed for "
                          << item.name << " (RM" << item.price << ").\n";
                if (!stall.empty()) std::cout << "  Please collect from the " << stall << " stall.\n";
                else                std::cout << "  A stall will be assigned when one is free.\n";
                logStep(session, nextStepId, "Ordered " + item.name);
                break;
            }
            case 4:
                std::cout << "  -- Food Court Map --\n";
                layout.displayLayout();
                logStep(session, nextStepId, "Viewed the food court map");
                break;
            case 5:
                if (session.isEmpty()) std::cout << "  Nothing to undo.\n";
                else std::cout << "  Undid: " << session.goBack().description << "\n";
                break;
            case 6:
                std::cout << "  -- Your Activity So Far --\n";
                session.displayHistory();
                break;
            case 7:
                std::cout << "  Thank you! Please collect your order when ready.\n";
                break;
            default:
                std::cout << "  Invalid option, try again.\n";
        }
    } while (choice != 7);
}

// =====================================================================
// STAFF PANEL: manage the menu database (Module 4 - insert/update/remove)
// =====================================================================

static void manageMenu(MenuSearch& menu) {
    int choice;
    do {
        std::cout << "\n--- Staff: Manage Menu Items ---\n"
                  << "1. Add a new item\n"
                  << "2. Update an item\n"
                  << "3. Remove an item\n"
                  << "4. Display all items (sorted)\n"
                  << "0. Back\n";
        choice = promptInt("Select an option: ");
        switch (choice) {
            case 1: {
                int id = promptInt("  Item ID: ");
                std::string name  = promptLine("  Name: ");
                std::string stall = promptLine("  Stall / Category: ");
                double price = promptPrice("  Price: RM");
                menu.insertItem({id, name, stall, price});
                std::cout << "  Item added.\n";
                break;
            }
            case 2: {
                int id = promptInt("  Item ID to update: ");
                bool found = false;
                menu.searchById(id, found);
                if (!found) { std::cout << "  No item with ID " << id << ".\n"; break; }
                std::string name  = promptLine("  New name: ");
                std::string stall = promptLine("  New stall / category: ");
                double price = promptPrice("  New price: RM");
                menu.updateItem(id, name, stall, price);
                std::cout << "  Item updated.\n";
                break;
            }
            case 3: {
                int id = promptInt("  Item ID to remove: ");
                if (menu.removeItem(id)) std::cout << "  Item removed.\n";
                else                     std::cout << "  No item with ID " << id << ".\n";
                break;
            }
            case 4:
                std::cout << "  -- Menu (sorted by ID) --\n";
                menu.displaySortedItems();
                break;
            case 0: break;
            default: std::cout << "  Invalid option, try again.\n";
        }
    } while (choice != 0);
}

// =====================================================================
// STAFF PANEL: stalls (Module 2), order queue (Module 1), map (Module 5)
// =====================================================================

static void manageStalls(StallAssignment& stalls) {
    int choice;
    do {
        std::cout << "\n--- Staff: Stalls ---\n"
                  << "1. Add a stall\n"
                  << "2. Set a stall's status\n"
                  << "3. Display stall status\n"
                  << "0. Back\n";
        choice = promptInt("Select an option: ");
        switch (choice) {
            case 1: {
                int stallId = promptInt("  Stall ID: ");
                std::string name = promptLine("  Stall name: ");
                stalls.addStall({stallId, name, promptStallStatus()});
                std::cout << "  Stall added.\n";
                break;
            }
            case 2: {
                int stallId = promptInt("  Stall ID: ");
                stalls.setStallStatus(stallId, promptStallStatus());
                std::cout << "  Status updated.\n";
                break;
            }
            case 3:
                stalls.displayStallStatus();
                break;
            case 0: break;
            default: std::cout << "  Invalid option, try again.\n";
        }
    } while (choice != 0);
}

static void manageOrders(OrderQueue& orders) {
    int choice;
    do {
        std::cout << "\n--- Staff: Order Queue ---\n"
                  << "1. Display pending orders\n"
                  << "2. Process (fulfil) next order\n"
                  << "3. Show number of pending orders\n"
                  << "0. Back\n";
        choice = promptInt("Select an option: ");
        switch (choice) {
            case 1:
                orders.displayPendingOrders();
                break;
            case 2:
                if (orders.isEmpty()) std::cout << "  No orders waiting.\n";
                else { std::cout << "  Fulfilling:\n"; printOrder(orders.dequeueOrder()); }
                break;
            case 3:
                std::cout << "  Pending orders: " << orders.size() << "\n";
                break;
            case 0: break;
            default: std::cout << "  Invalid option, try again.\n";
        }
    } while (choice != 0);
}

static void manageLayout(FoodCourtLayout& layout) {
    int choice;
    do {
        std::cout << "\n--- Staff: Food Court Layout ---\n"
                  << "1. Add a location (zone / stall / counter)\n"
                  << "2. Show route between two locations\n"
                  << "3. Display full layout\n"
                  << "0. Back\n";
        choice = promptInt("Select an option: ");
        switch (choice) {
            case 1: {
                std::string parent   = promptLine("  Parent location (blank for top level): ");
                std::string location = promptLine("  New location name: ");
                layout.addLocation(parent, location);
                std::cout << "  Location request processed.\n";
                break;
            }
            case 2: {
                std::string from = promptLine("  From: ");
                std::string to   = promptLine("  To: ");
                layout.printRoute(from, to);
                break;
            }
            case 3:
                layout.displayLayout();
                break;
            case 0: break;
            default: std::cout << "  Invalid option, try again.\n";
        }
    } while (choice != 0);
}

static void staffPanel(OrderQueue& orders, StallAssignment& stalls,
                       MenuSearch& menu, FoodCourtLayout& layout) {
    int choice;
    do {
        std::cout << "\n===== Staff / Admin Panel =====\n"
                  << "1. Manage menu items\n"
                  << "2. Manage stalls\n"
                  << "3. Manage order queue\n"
                  << "4. Manage food court layout\n"
                  << "0. Back to kiosk\n";
        choice = promptInt("Select an option: ");
        switch (choice) {
            case 1: manageMenu(menu);      break;
            case 2: manageStalls(stalls);  break;
            case 3: manageOrders(orders);  break;
            case 4: manageLayout(layout);  break;
            case 0: break;
            default: std::cout << "  Invalid option, try again.\n";
        }
    } while (choice != 0);
}

// =====================================================================
// Program entry - the kiosk welcome screen
// =====================================================================

int main() {
    // Shared instances of every module for the whole kiosk session.
    OrderQueue      orders;
    StallAssignment stalls(STALL_CAPACITY);
    SessionHistory  session;
    MenuSearch      menu;
    FoodCourtLayout layout;

    // Seed a few sample menu items so the kiosk starts with a menu.
    menu.insertItem({101, "Nasi Lemak", "Malay", 6.50});
    menu.insertItem({102, "Char Kway Teow", "Chinese", 7.00});
    menu.insertItem({103, "Beef Burger", "Western", 9.50});

    int nextOrderId = 1001;   // running order number across the day
    int nextStepId  = 1;      // running session-step number

    std::cout << "=====================================================\n"
              << "   APU Campus Food Court - Self-Order Kiosk System\n"
              << "=====================================================\n";

    int choice;
    do {
        std::cout << "\n========== WELCOME ==========\n"
                  << "1. Start ordering (I'm a student)\n"
                  << "2. Staff / Admin panel\n"
                  << "0. Shut down kiosk\n";
        choice = promptInt("Select an option: ");
        switch (choice) {
            case 1: studentSession(orders, stalls, session, menu, layout,
                                   nextOrderId, nextStepId); break;
            case 2: staffPanel(orders, stalls, menu, layout); break;
            case 0: std::cout << "Kiosk shutting down. Goodbye!\n"; break;
            default: std::cout << "  Invalid option, try again.\n";
        }
    } while (choice != 0);

    return 0;
}
