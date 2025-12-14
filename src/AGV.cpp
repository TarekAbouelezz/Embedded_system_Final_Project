/**
 * @file AGV.cpp
 * @brief AGV implementation with state machine
 */

#include "AGV.h"
#include <iostream>
#include <chrono>
#include <thread>

AGV::AGV(int id) 
    : agv_id(id), 
      state(AGVState::IDLE), 
      running(false),
      travel_time_warehouse_minutes(2),
      travel_time_station_minutes(3),
      picking_time_minutes(1),
      dropping_time_minutes(1),
      total_operations(0),
      busy_time_minutes(0) {
}

AGV::~AGV() {
    stop();
    if (agv_thread.joinable()) {
        agv_thread.join();
    }
}

void AGV::start() {
    running = true;
    agv_thread = std::thread(&AGV::run, this);
}

void AGV::stop() {
    running = false;
    task_cv.notify_all();
}

void AGV::run() {
    while (running) {
        std::unique_lock<std::mutex> lock(state_mutex);
        
        // Wait for task assignment
        task_cv.wait(lock, [this] { 
            return !running || !current_task.component_id.empty(); 
        });
        
        if (!running) break;
        
        if (current_task.component_id.empty()) {
            continue;
        }
        
        // Execute task
        int task_start_time = 0; // Should get from simulation clock
        
        // Transition to TO_WAREHOUSE
        transition_to(AGVState::TO_WAREHOUSE);
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(
            travel_time_warehouse_minutes * 100)); // Scaled for simulation
        
        // Picking
        lock.lock();
        transition_to(AGVState::PICKING);
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(
            picking_time_minutes * 100));
        
        // Travel to station
        lock.lock();
        transition_to(AGVState::TO_STATION);
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(
            travel_time_station_minutes * 100));
        
        // Dropping
        lock.lock();
        transition_to(AGVState::DROPPING);
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(
            dropping_time_minutes * 100));
        
        // Complete task
        lock.lock();
        busy_time_minutes += (travel_time_warehouse_minutes + 
                             picking_time_minutes + 
                             travel_time_station_minutes + 
                             dropping_time_minutes);
        total_operations++;
        
        // Reset task
        current_task = AGVTask();
        transition_to(AGVState::IDLE);
        lock.unlock();
    }
}

void AGV::transition_to(AGVState new_state) {
    state = new_state;
}

void AGV::assign_task(const std::string& component_id, int quantity,
                      const std::string& destination) {
    std::lock_guard<std::mutex> lock(state_mutex);
    
    if (state == AGVState::IDLE && current_task.component_id.empty()) {
        current_task.component_id = component_id;
        current_task.quantity = quantity;
        current_task.destination = destination;
        current_task.is_complete = false;
        task_cv.notify_one();
    }
}

bool AGV::is_idle() const {
    std::lock_guard<std::mutex> lock(state_mutex);
    return state == AGVState::IDLE && current_task.component_id.empty();
}

AGVState AGV::get_state() const {
    std::lock_guard<std::mutex> lock(state_mutex);
    return state;
}

AGVTask AGV::get_current_task() const {
    std::lock_guard<std::mutex> lock(state_mutex);
    return current_task;
}


