# Flexible Assembly System (FAS) Simulator

A multithreaded simulation system for modeling a flexible assembly cell in an industrial plant, including order management, component handling, warehouse logistics, and AGV fleet coordination.

## Project Overview

This project simulates a flexible assembly system with the following components:

- **Control Center**: Manages order scheduling and KPI computation.
- **Assembly Station**: Processes orders one at a time.
- **AGV Fleet**: Multiple AGVs (minimum 10) transporting components.
- **Warehouse**: Stores components and finished products.

## Features

- **Multithreaded Architecture**: Concurrent execution of multiple subsystems.
- **File-Based Configuration**: Input files for orders, BOM, and warehouse inventory.
- **Scheduling Policies**: FIFO (implemented), with support for Priority, SPT, and EDD.
- **Performance Metrics**: Automated KPI computation (lead time, utilization, throughput).
- **Event Logging**: Detailed simulation logs for analysis.


## System Architecture

### Threading Model

- **Control Center Thread**: Order scheduling and release.
- **Assembly Station Thread**: Order processing.
- **Multiple AGV Threads**: One per AGV for concurrent transport.

### AGV State Machine

AGVs operate with the following states:

- `IDLE`: Waiting for task assignment.
- `TO_WAREHOUSE`: Traveling to warehouse.
- `PICKING`: Collecting components.
- `TO_STATION`: Traveling to assembly station.
- `DROPPING`: Delivering components.
- `RETURNING`: Returning to parking location.

### Synchronization

- Mutexes protect shared resources (warehouse inventory, order queues).
- Condition variables coordinate thread activities.
- Atomic variables track simulation time and state.

## Key Performance Indicators (KPIs)

1. **Average Lead Time**: Time from order release to completion.
2. **Assembly Station Utilization**: Percentage of simulation time station is busy.
3. **Throughput**: Orders completed per hour.
4. **AGV Utilization**: Average percentage of time AGVs are busy.

## Project Structure

```
.
├── src/
│   ├── main.cpp              # Entry point
│   ├── ControlCenter.h/cpp   # Order scheduling and KPI computation
│   ├── AssemblyStation.h/cpp # Order processing
│   ├── Warehouse.h/cpp       # Inventory management
│   ├── AGV.h/cpp             # AGV state machine
│   ├── Order.h               # Order data structure
│   ├── Product.h             # Product and BOM definitions
│   └── FileHandler.h/cpp     # File I/O utilities
├── input/                    # Input files directory
│   ├── orders.txt
│   ├── bom.txt
│   └── warehouse.txt
├── output/                   # Output files directory (created at runtime)
│   ├── sim_log.txt
│   └── kpi_report.txt
├── CMakeLists.txt            # Build configuration
└── README.md                 # This file
```

## Design Decisions

1. **Object-Oriented Design**: Clear separation of concerns with dedicated classes.
2. **Thread Safety**: All shared data protected with mutexes.
3. **File-Based I/O**: Flexible configuration without recompilation.
4. **Extensible Scheduling**: Easy to add new scheduling policies.


