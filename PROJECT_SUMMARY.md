# Project Summary: Flexible Assembly System Simulator

## Overview

This project implements a **Flexible Assembly System (FAS)** simulator without ROS extension. The system models an industrial assembly cell with concurrent subsystems operating in a multithreaded environment.

## Implementation Status

### ✅ Completed Components

1. **Object-Oriented Design (40% weight)**

   - ✅ `Order`: Production order structure
   - ✅ `Product`: Product and BOM definitions
   - ✅ `Warehouse`: Thread-safe inventory management
   - ✅ `AGV`: State machine-based vehicle implementation
   - ✅ `AssemblyStation`: Order processing station
   - ✅ `ControlCenter`: Scheduling and KPI computation
   - ✅ `FileHandler`: Input/output file operations
2. **Multithreading and Concurrency (25% weight)**

   - ✅ Multiple AGV threads (configurable, minimum 10)
   - ✅ Assembly Station thread
   - ✅ Control Center scheduler thread
   - ✅ Mutex-protected shared resources (warehouse, queues)
   - ✅ Condition variables for thread coordination
3. **Simulation Correctness (20% weight)**

   - ✅ Order release based on timestamps
   - ✅ Component request and delivery flow
   - ✅ AGV state machine (IDLE → TO_WAREHOUSE → PICKING → TO_STATION → DROPPING)
   - ✅ Assembly operation timing (T_base + T_setup)
   - ✅ Finished product storage
4. **KPI Computation (5% weight)**

   - ✅ Average Lead Time calculation
   - ✅ Assembly Station Utilization
   - ✅ Throughput (orders/hour)
   - ✅ AGV Utilization
   - ✅ Report generation (kpi_report.txt)
5. **File Handling (Without ROS)**

   - ✅ Reading: orders.txt, bom.txt, warehouse.txt
   - ✅ Writing: sim_log.txt, kpi_report.txt
   - ✅ Error handling and validation
6. **Documentation (10% weight)**

   - ✅ Comprehensive README.md
   - ✅ Build instructions
   - ✅ Code comments and documentation
   - ✅ Project structure documentation

## Architecture

### Threading Model

```
┌─────────────────────┐
│  Control Center     │  (Main thread + scheduler thread)
│  - Order scheduling │
│  - KPI computation  │
└──────────┬──────────┘
           │
    ┌──────┴──────┐
    │             │
┌───▼────────┐  ┌─▼────────────┐
│ Assembly   │  │ AGV Fleet    │
│ Station    │  │ (10+ threads)│
│ (1 thread) │  └──────────────┘
└─────┬──────┘
      │
┌─────▼──────┐
│ Warehouse  │  (Shared, mutex-protected)
└────────────┘
```

### Data Flow

1. **Order Release**: Control Center reads orders.txt and releases orders based on timestamps
2. **Component Request**: Assembly Station requests components based on BOM
3. **AGV Assignment**: Idle AGVs are assigned to transport components
4. **Transport**: AGVs move through states (warehouse → station)
5. **Assembly**: Station processes order once components arrive
6. **Completion**: Finished product stored in warehouse, KPIs updated

## File Structure

```
├── src/
│   ├── main.cpp              # Entry point and initialization
│   ├── ControlCenter.h/cpp   # Order scheduling, KPI computation
│   ├── AssemblyStation.h/cpp # Order processing
│   ├── Warehouse.h/cpp       # Inventory management
│   ├── AGV.h/cpp             # AGV state machine
│   ├── Order.h               # Order data structure
│   ├── Product.h             # Product/BOM definitions
│   └── FileHandler.h/cpp     # File I/O utilities
├── input/                    # Input files
│   ├── orders.txt
│   ├── bom.txt
│   └── warehouse.txt
├── output/                   # Generated files (runtime)
│   ├── sim_log.txt
│   └── kpi_report.txt
├── CMakeLists.txt            # Build configuration
├── README.md                 # Main documentation
├── BUILD_INSTRUCTIONS.md     # Platform-specific builds
└── PROJECT_SUMMARY.md        # This file
```

## Key Features

### Scheduling Policy

- **Implemented**: FIFO (First-In-First-Out)
- **Extensible**: Framework supports Priority, SPT, EDD

### AGV States

- `IDLE`: Waiting for task
- `TO_WAREHOUSE`: Traveling to warehouse
- `PICKING`: Collecting components
- `TO_STATION`: Traveling to assembly station
- `DROPPING`: Delivering components
- `RETURNING`: (Optional) Returning to parking

### Thread Safety

- Warehouse inventory: Protected by mutex
- Order queues: Protected by mutex + condition variables
- Logging: Protected by mutex
- Simulation time: Atomic variables

## Usage

1. **Prepare Input Files**: Place orders.txt, bom.txt, warehouse.txt in `input/` directory
2. **Build Project**: Follow BUILD_INSTRUCTIONS.md
3. **Run Simulation**: Execute `fas_simulator`
4. **Review Output**: Check `output/sim_log.txt` and `output/kpi_report.txt`

## Notes

- **Simulation Time**: Currently simplified - uses thread sleep for timing. For production, consider event-driven simulation with discrete event scheduler.
- **Component Reservation**: Warehouse reserves components atomically to prevent race conditions.
- **AGV Fleet Size**: Default is 10 AGVs (minimum requirement). Can be changed in main.cpp (`NUM_AGVS`).
- **Error Handling**: Basic error handling implemented. Consider adding more robust error recovery.

## Extension Points

1. **Advanced Scheduling**: Implement SPT, EDD, priority-based policies
2. **ROS Integration**: Add optional ROS nodes for visualization
3. **Performance Optimization**: Optimize for large-scale simulations
4. **Visualization**: Add real-time status dashboard
5. **Statistics**: Extended KPI tracking and analysis

## Build Requirements

- C++17 compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10+
- Thread support (pthread on Unix)

## Testing Recommendations

1. Start with 2 AGVs for debugging
2. Test with small order sets
3. Verify thread synchronization with concurrent operations
4. Check KPI calculations with known scenarios
5. Validate file I/O with various input formats

