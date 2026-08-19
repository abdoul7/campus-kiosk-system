#pragma once
#include "../include/Common.hpp"
#include <string>

// =====================================================================
// Module 2: Stall Assignment
// Data Structure: CIRCULAR QUEUE - self-implemented
// Owner: Member 2
// =====================================================================
//
//
// coming out of Module 1's OrderQueue. Fill in order.assignedStall and
// order.status before returning.
//
//     StallRecord) with front/rear indices that wrap using %, exactly
//     like a textbook circular queue - this is what lets the rotation
//     keep moving forward call after call instead of restarting at
//     stall #1 every time a new order comes in.
//     re-enqueues it at the rear immediately (whether or not it was
//     usable). That single rule is what gives "continuous rotation
//     without restarting the cycle": position in the rotation is
//     preserved between calls.
//   - A stall is skipped (but stays in rotation for its next turn) if
//     AVAILABLE stall found, the order is left unassigned.
//     kept alongside each stall in the buffer, incremented every time
//     that stall is chosen, and shown in displayStallStatus().

class StallAssignment {
public:
    explicit StallAssignment(int numStalls);
    ~StallAssignment();

    void addStall(const Stall& stall);

    // Assigns the given order to the next available stall in rotation.
    // Mutates order.assignedStall / order.status.
    // Returns the stall name assigned, or "" if none available.
    std::string assignNextStall(Order& order);

    void setStallStatus(int stallId, StallStatus status);

    void displayStallStatus() const;

private:
    // One slot in the circular buffer: the stall itself plus a running
    // count of how many orders have been routed to it.
    struct StallRecord {
        Stall stall;
        int assignedCount = 0;
    };

    StallRecord* buffer;   // fixed-size circular array, sized to numStalls
    int capacity;           // max number of stalls the buffer can hold
    int frontIndex;         // index of the stall next in line for rotation
    int rearIndex;          // index of the most recently added stall
    int count;              // number of stalls currently held in the buffer

    // Finds the buffer slot for a given stall id, walking only the
    // 'count' live slots starting at frontIndex. Returns nullptr if
    // no stall with that id has been added.
    StallRecord* findRecordById(int stallId);
};