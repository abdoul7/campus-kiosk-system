#include "OrderQueue.hpp"
#include <iostream>

/*==================================================================================
    MODULE 1: Order Queue Management
    DATA STRUCTURE: Queue (FIFO), self-implemented as a singly linked list.

    Why a Queue?
      Orders must be processed fairly, in the exact order they arrive (first come,
      first served). Enqueue adds to the REAR, dequeue removes from the FRONT.

    Why a linked list (and not an array)?
      Orders flow in continuously and we don't know how many there will be. A
      linked list grows one node at a time, and keeping a 'rear' pointer makes
      both enqueue and dequeue O(1) with no shifting.
==================================================================================*/

// Cap on pending orders, used only to detect the "system overload" edge case.
static const int MAX_CAPACITY = 100;

OrderQueue::OrderQueue() {
    front = nullptr;
    rear = nullptr;
    count = 0;
    capacity = MAX_CAPACITY;
}

OrderQueue::~OrderQueue() {
    while (front != nullptr) {
        Node* doomed = front;
        front = front->next;
        delete doomed;
    }
}

bool OrderQueue::isEmpty() const {
    return front == nullptr;
}

int OrderQueue::size() const {
    return count;
}

// ENQUEUE - add a new order at the rear. Rejects when at capacity
// (system overload edge case).
void OrderQueue::enqueueOrder(const Order& order) {
    if (count >= capacity) {
        std::cout << "  [Rejected] Order queue is full (system overload). Order "
                  << order.orderId << " could not be accepted.\n";
        return;
    }

    Node* newNode = new Node(order);
    if (isEmpty()) {
        front = newNode;
        rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
    count++;
}

// DEQUEUE - remove and return the order that has waited longest (the front).
Order OrderQueue::dequeueOrder() {
    if (isEmpty()) {
        std::cout << "  [Empty] No pending orders to process.\n";
        return Order{};
    }

    Node* oldFront = front;
    Order data = oldFront->data;
    front = front->next;
    if (front == nullptr)
        rear = nullptr;   // queue just became empty - reset rear too
    delete oldFront;
    count--;
    return data;
}

// PEEK - look at the front order without removing it.
Order OrderQueue::peekFrontOrder() const {
    if (isEmpty()) {
        std::cout << "  [Empty] No order at the front.\n";
        return Order{};
    }
    return front->data;
}

static std::string statusToString(OrderStatus status) {
    switch (status) {
        case OrderStatus::PENDING:     return "Pending";
        case OrderStatus::ASSIGNED:    return "Assigned";
        case OrderStatus::IN_PROGRESS: return "In Progress";
        case OrderStatus::COMPLETED:   return "Completed";
    }
    return "Unknown";
}

// DISPLAY - walk from front to rear, printing every waiting order.
void OrderQueue::displayPendingOrders() const {
    if (isEmpty()) {
        std::cout << "  (no pending orders)\n";
        return;
    }

    std::cout << "  ---------------------------------------------------------------\n";
    std::cout << "   Pos | OrderID | Student  | Item | Stall            | Status\n";
    std::cout << "  ---------------------------------------------------------------\n";
    int pos = 1;
    for (Node* current = front; current != nullptr; current = current->next) {
        std::cout << "   " << pos << "   |  " << current->data.orderId
                  << "\t|  " << current->data.studentId
                  << "  | " << current->data.itemId
                  << "    | " << current->data.assignedStall
                  << "  | " << statusToString(current->data.status) << "\n";
        pos++;
    }
    std::cout << "  ---------------------------------------------------------------\n";
    std::cout << "  Total pending orders: " << count << "\n";
}
