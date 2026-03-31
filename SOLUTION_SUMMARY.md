# Problem 119 - Linear Scan Register Allocation - Solution Summary

## Problem Description
Implement a simplified Linear Scan Register Allocation algorithm in C++. The algorithm assigns variables (represented by live intervals) to either registers or stack slots following the linear scan approach.

## Solution Overview

### Key Components Implemented

1. **Location Classes**
   - `Register`: Represents a register with an ID (0 to regNum-1)
   - `StackSlot`: Represents a stack location for spilled variables
   - Both inherit from abstract `Location` class

2. **LinearScanRegisterAllocator**
   - Manages register allocation using the linear scan algorithm
   - Maintains a stack of free registers (FILO order)
   - Tracks active intervals sorted by endpoint
   - Implements the three main functions:
     - `expireOldIntervals()`: Frees registers from expired intervals
     - `spillAtInterval()`: Handles spilling when no registers are available
     - `linearScanRegisterAllocate()`: Main allocation loop

### Algorithm Implementation

The algorithm follows these steps:
1. Initialize all registers as free (pushed onto a stack in reverse order so reg0 is at the top)
2. For each interval (already sorted by start point):
   - Expire intervals that end before the current interval's start point
   - If free registers exist, allocate one (FILO order)
   - Otherwise, spill the interval with the largest endpoint (or current interval if it has the largest endpoint)
3. Maintain the `active` set sorted by endpoint using std::set with custom comparator

### Key Design Decisions

- Used `std::stack<int>` for free registers to ensure FILO allocation
- Used `std::set` with custom comparator `CompareByEndpoint` for active intervals
- Register allocation follows FILO principle: most recently freed register is allocated first
- Initial registers pushed in reverse order (regNum-1 down to 0) so reg0 is at top

## Test Results

### Submission 1 (ID: 766317)
- **Status**: Accepted ✅
- **Score**: 100/100
- **All test cases passed**:
  - Subtask 1 (20%): Location classes - Passed
  - Subtask 2 (10%): Single allocation per register, no spill - Passed
  - Subtask 3 (10%): Single allocation per register - Passed
  - Subtask 4 (20%): No spill cases - Passed
  - Subtask 5 (20%): General cases (regNum ≤ 32, intervals ≤ 1000) - Passed
  - Subtask 6 (20%): Stress test (regNum ≤ 1000, intervals ≤ 100000) - Passed

### Performance
- Maximum time: 82ms (well under 1000ms limit)
- Maximum memory: 8.3MB (well under 244MB limit)

## Local Testing

Created `test.cpp` with the provided sample:
- 5 intervals, 2 registers
- Expected output matched perfectly:
  ```
  interval 1: reg0
  interval 2: reg1
  interval 3: stack
  interval 4: reg0
  interval 5: reg1
  ```

## Submission Statistics
- **Total submissions**: 1
- **Remaining attempts**: 4
- **Final score**: 100/100 (Perfect score achieved on first attempt)

## Files in Repository
- `src.hpp`: Main solution file (submitted to OJ)
- `test.cpp`: Local test program
- `submit_acmoj/acmoj_client.py`: Modified to add submit_code method

## Conclusion
Successfully implemented a complete Linear Scan Register Allocator that passes all test cases with optimal performance. The solution correctly handles register allocation, expiration, and spilling according to the algorithm specification.
