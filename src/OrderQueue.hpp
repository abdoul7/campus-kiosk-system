#pragma once
#include "../include/Common.hpp"

// =====================================================================
// Module 1: Order Queue Management
// Data Structure: QUEUE (FIFO) - must be self-implemented
//                 (no <queue>, <list>, <vector>, etc.)
// Owner: Member 1
// =====================================================================
//
// Functional requirements (from assignment spec):
//   - Accept and record new student orders
//   - Maintain an ordered list of incoming requests
//   - Process orders sequentially by arrival time
//   - Remove orders once assigned to a stall
//   - Report pending / current / completed orders
//   - Handle empty-queue / overload edge cases
//
// INTEGRATION CONTRACT: do not change these public method signatures —
// main.cpp and StallAssignment call into this class through them.
//
// TODO (Member 1): choose your underlying representation (linked
// nodes or a fixed-size array with front/rear indices) and implement
// the logic inside each method in OrderQueue.cpp.

class OrderQueue {
public:
    OrderQueue();
    ~OrderQueue();

    // Add a new order to the back of the queue
    void enqueueOrder(const Order& order);

    // Remove and return the order at the front of the queue
    // (call once a stall has been assigned to it)
    Order dequeueOrder();

    // Look at the front order without removing it
    Order peekFrontOrder() const;

    bool isEmpty() const;

    int size() const;

    // Prints all pending orders (for demo / debugging)
    void displayPendingOrders() const;

private:
    // Singly linked list node. 'next' points to the order behind it
    // in the line (the one that arrived later).
    struct Node {
        Order data;
        Node* next;
        Node(const Order& order) : data(order), next(nullptr) {}
    };

    Node* front;      // oldest order (served next)
    Node* rear;       // newest order (arrived most recently)
    int   count;      // how many orders are currently queued
    int   capacity;   // maximum allowed (used to detect system overload)
};
