#ifndef LINEARSCAN_HPP
#define LINEARSCAN_HPP

// don't include other headfiles
#include <string>
#include <vector>
#include <set>
#include <stack>

class Location {
public:
    // return a string that represents the location
    virtual std::string show() const = 0;
    virtual int getId() const = 0;
};

class Register : public Location {
private:
    int regId;
public:
    Register(int regId) : regId(regId) {}

    virtual std::string show() const {
        return "reg" + std::to_string(regId);
    }

    virtual int getId() const {
        return regId;
    }
};

class StackSlot : public Location {
public:
    StackSlot() {}

    virtual std::string show() const {
        return "stack";
    }

    virtual int getId() const {
        return -1;
    }
};

struct LiveInterval {
    int startpoint;
    int endpoint;
    Location* location = nullptr;
};

// Comparator for sorting active intervals by endpoint
struct CompareByEndpoint {
    bool operator()(const LiveInterval* a, const LiveInterval* b) const {
        return a->endpoint < b->endpoint;
    }
};

class LinearScanRegisterAllocator {
private:
    int regNum;
    std::stack<int> freeRegisters;  // Stack of free register IDs (FILO)
    std::set<LiveInterval*, CompareByEndpoint> active;  // Active intervals sorted by endpoint

    void expireOldIntervals(LiveInterval& i) {
        // Remove intervals from active that end before i's start point
        auto it = active.begin();
        while (it != active.end()) {
            LiveInterval* j = *it;
            if (j->endpoint >= i.startpoint) {
                break;  // Since active is sorted by endpoint, we can stop here
            }
            // Free the register used by j
            if (j->location->getId() != -1) {  // Not a stack slot
                freeRegisters.push(j->location->getId());
            }
            it = active.erase(it);
        }
    }

    void spillAtInterval(LiveInterval& i) {
        // Get the interval with the largest endpoint from active
        auto it = active.rbegin();
        LiveInterval* spill = *it;

        if (spill->endpoint > i.endpoint) {
            // Spill the last interval in active
            i.location = spill->location;
            spill->location = new StackSlot();
            active.erase(std::prev(active.end()));
            active.insert(&i);
        } else {
            // Spill i
            i.location = new StackSlot();
        }
    }

public:
    LinearScanRegisterAllocator(int regNum) : regNum(regNum) {
        // Initialize free registers stack
        // Push in reverse order so that reg0 is at the top
        for (int i = regNum - 1; i >= 0; i--) {
            freeRegisters.push(i);
        }
    }

    void linearScanRegisterAllocate(std::vector<LiveInterval>& intervalList) {
        for (auto& i : intervalList) {
            expireOldIntervals(i);

            if (!freeRegisters.empty()) {
                // Allocate a free register
                int regId = freeRegisters.top();
                freeRegisters.pop();
                i.location = new Register(regId);
                active.insert(&i);
            } else {
                // Need to spill
                spillAtInterval(i);
            }
        }
    }
};

#endif
