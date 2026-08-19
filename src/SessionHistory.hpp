#pragma once
#include "../include/Common.hpp"

// =====================================================================
// Module 3: Kiosk Session History & Navigation
// Data Structure: STACK (LIFO) - must be self-implemented
// Owner: Member 3
// =====================================================================
//
// Functional requirements:
//   - Record each step taken during a kiosk session
//   - Store full session history from login to current state
//   - Allow backward navigation one step at a time (undo)
//   - Support re-visiting / modifying a previously reached state
//   - Handle empty-history / limit-exceeded edge cases
//
// INTEGRATION CONTRACT: other modules don't call into this directly —
// main.cpp drives it per-student-session. If Module 5 (optional) is
// built, it may also call recordStep() for location-aware navigation.
//
// TODO (Member 3): implement the underlying node/array structure and
// the logic inside each method in SessionHistory.cpp.

class SessionHistory {
public:
    SessionHistory();
    ~SessionHistory();

    // Record a new step taken during the session (push)
    void recordStep(const SessionStep& step);

    // Undo / go back one step; returns the step navigated back to (pop)
    SessionStep goBack();

    // View the current (most recent) step without removing it
    SessionStep peekCurrentStep() const;

    bool isEmpty() const;

    // Prints the full trace of steps recorded so far
    void displayHistory() const;

private:
    // Singly linked list node. Each node owns one recorded step and a
    // pointer to the node pushed before it (i.e. the next-older step).
    struct Node {
        SessionStep data;
        Node* next;
        Node(const SessionStep& step, Node* nextNode) : data(step), next(nextNode) {}
    };

    // Points at the most recently pushed node (the "top" of the stack).
    // nullptr means the history is empty.
    Node* top;
};
