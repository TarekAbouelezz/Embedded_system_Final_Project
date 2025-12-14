/**
 * @file AGV.h
 * @brief Automated Guided Vehicle (AGV) definition with state machine
 */

#ifndef AGV_H
#define AGV_H

#include <string>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>

enum class AGVState {
    IDLE,
    TO_WAREHOUSE,
    PICKING,
    TO_STATION,
    DROPPING,
    RETURNING
};

struct AGVTask {
    std::string component_id;
    int quantity;
    std::string destination;  // "ASSEMBLY_STATION" or "WAREHOUSE"
    bool is_complete;
    
    AGVTask() : quantity(0), is_complete(false) {}
};

class AGV {
private:
    int agv_id;
    AGVState state;
    AGVTask current_task;
    mutable std::mutex state_mutex;
    std::condition_variable task_cv;
    std::atomic<bool> running;
    std::thread agv_thread;
    
    // Timing parameters (in simulated minutes)
    int travel_time_warehouse_minutes;
    int travel_time_station_minutes;
    int picking_time_minutes;
    int dropping_time_minutes;
    
    void run();
    void transition_to(AGVState new_state);
    
public:
    AGV(int id);
    ~AGV();
    
    void start();
    void stop();
    void assign_task(const std::string& component_id, int quantity, 
                     const std::string& destination);
    bool is_idle() const;
    AGVState get_state() const;
    int get_id() const { return agv_id; }
    AGVTask get_current_task() const;
    
    // Statistics
    int total_operations;
    int busy_time_minutes;
};

#endif /* AGV_H */

