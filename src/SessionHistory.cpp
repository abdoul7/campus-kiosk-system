#include "SessionHistory.hpp"
#include <iostream>

// =====================================================================
// Member 3: implement each method below. Signatures must stay as-is.
// =====================================================================
//
// Stack mechanics (singly linked list, LIFO):
//   - "top" always points to the node holding the most recently
//     recorded step (or nullptr when the stack is empty).
//   - PUSH (recordStep): allocate a new node whose "next" points at
//     the current top, then move "top" to the new node. O(1), and
//     nothing before it needs to shift.
//   - POP (goBack): read the data out of "top", save a pointer to
//     "top->next", delete the old top node, then move "top" to the
//     saved pointer. This removes the most recent step and exposes
//     the one recorded before it.
//   - Because each node only points to the node beneath it, walking
//     from "top" via "next" naturally visits steps most-recent-first.

SessionHistory::SessionHistory() : top(nullptr) {
    // Empty stack: no nodes yet, so top starts as null.
}

SessionHistory::~SessionHistory() {
    // Walk the list from top to bottom, freeing each node so we don't
    // leak memory when the SessionHistory object is destroyed.
    while (top != nullptr) {
        Node* doomed = top;
        top = top->next;
        delete doomed;
    }
}

void SessionHistory::recordStep(const SessionStep& step) {
    // Push: new node's "next" links to the current top, then it
    // becomes the new top. This is the only place nodes are created.
    top = new Node(step, top);
}

SessionStep SessionHistory::goBack() {
    if (isEmpty()) {
        // Nothing recorded yet - return a default/blank step instead
        // of dereferencing a null top pointer.
        std::cout << "[SessionHistory] No steps to go back to.\n";
        return SessionStep{};
    }

    // Pop: detach the top node, keep its data, then reattach top to
    // whatever was underneath it.
    Node* oldTop = top;
    SessionStep stepData = oldTop->data;
    top = oldTop->next;
    delete oldTop;
    return stepData;
}

SessionStep SessionHistory::peekCurrentStep() const {
    if (isEmpty()) {
        // Safe default when there's no current step to look at.
        return SessionStep{};
    }
    return top->data;
}

bool SessionHistory::isEmpty() const {
    return top == nullptr;
}

void SessionHistory::displayHistory() const {
    if (isEmpty()) {
        std::cout << "[SessionHistory] No steps recorded yet.\n";
        return;
    }

    std::cout << "[SessionHistory] Steps recorded (most recent first):\n";
    // Traverse from top downward via "next" - this walks the stack
    // most-recent-step-first since that's how it was built.
    for (Node* cur = top; cur != nullptr; cur = cur->next) {
        std::cout << "  [" << cur->data.stepId << "] "
                  << cur->data.description << "\n";
    }
}
