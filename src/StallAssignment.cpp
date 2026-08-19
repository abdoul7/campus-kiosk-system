#include "StallAssignment.hpp"
#include <iostream>

// =====================================================================
// Member 2: Stall Assignment - Circular Queue implementation
// =====================================================================

StallAssignment::StallAssignment(int numStalls) {
    capacity = (numStalls > 0) ? numStalls : 1;
    buffer = new StallRecord[capacity];
    frontIndex = 0;
    rearIndex = -1;
    count = 0;
}

StallAssignment::~StallAssignment() {
    delete[] buffer;
}

// Finds the stall record for a given id among the 'count' live slots,
// starting from the current front and walking forward with wrap-around.
StallAssignment::StallRecord* StallAssignment::findRecordById(int stallId) {
    for (int i = 0, idx = frontIndex; i < count; i++, idx = (idx + 1) % capacity) {
        if (buffer[idx].stall.stallId == stallId)
            return &buffer[idx];
    }
    return nullptr;
}

// Adds a new stall to the rear of the circular buffer, i.e. it joins
// the rotation right after whichever stall was added most recently.
void StallAssignment::addStall(const Stall& stall) {
    if (count == capacity) {
        std::cout << "[StallAssignment] Buffer full (" << capacity
                  << " stalls) - cannot add '" << stall.name << "'.\n";
        return;
    }
    rearIndex = (rearIndex + 1) % capacity;
    buffer[rearIndex].stall = stall;
    buffer[rearIndex].assignedCount = 0;
    count++;
}

// Core round-robin allocation algorithm.
//
// rotation continuous across separate calls instead of restarting
// skipped (but stays in the rotation for its next turn) by moving on
std::string StallAssignment::assignNextStall(Order& order) {
    if (count == 0)
        return "";   // no stalls registered yet

    int attempts = 0;

    while (attempts < capacity) {
        StallRecord record = buffer[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
        count--;

        // keeps rotating forward regardless of the outcome below.
        rearIndex = (rearIndex + 1) % capacity;
        buffer[rearIndex] = record;
        count++;

        attempts++;

        if (record.stall.status == StallStatus::AVAILABLE) {
            // at rearIndex (the live copy of this stall in the buffer).
            buffer[rearIndex].assignedCount++;

            order.assignedStall = record.stall.name;
            order.status = OrderStatus::ASSIGNED;
            return record.stall.name;
        }
    }

    return "";
}

// Locates a stall by id anywhere in the buffer and updates its status
// (staff-driven, via the admin panel).
void StallAssignment::setStallStatus(int stallId, StallStatus status) {
    StallRecord* record = findRecordById(stallId);
    if (record == nullptr) {
        std::cout << "[StallAssignment] No stall with ID " << stallId << ".\n";
        return;
    }
    record->stall.status = status;
}

static std::string statusName(StallStatus status) {
    switch (status) {
        case StallStatus::AVAILABLE: return "Available";
        case StallStatus::BUSY:      return "Busy";
        case StallStatus::CLOSED:    return "Closed";
    }
    return "Unknown";
}

void StallAssignment::displayStallStatus() const {
    if (count == 0) {
        std::cout << "  (no stalls registered yet)\n";
        return;
    }
    for (int i = 0, idx = frontIndex; i < count; i++, idx = (idx + 1) % capacity) {
        const StallRecord& record = buffer[idx];
        std::cout << "  Stall #" << record.stall.stallId << " (" << record.stall.name
                  << ") - " << statusName(record.stall.status)
                  << " | orders assigned: " << record.assignedCount << "\n";
    }
}