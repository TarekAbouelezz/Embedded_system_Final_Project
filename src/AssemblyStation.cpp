/**
 * @file AssemblyStation.cpp
 * @brief Assembly Station implementation
 */

#include "AssemblyStation.h"
#include "AGV.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <map>
#include <string>

AssemblyStation::AssemblyStation(Warehouse* wh, std::vector<AGV*>* fleet)
    : warehouse(wh),
      agv_fleet(fleet),
      products(nullptr),
      running(false),
      current_sim_time_minutes(0),
      setup_time_minutes(5),
      total_busy_time_minutes(0),
      orders_completed(0) {
}

AssemblyStation::~AssemblyStation() {
    stop();
    if (station_thread.joinable()) {
        station_thread.join();
    }
}

void AssemblyStation::start() {
    running = true;
    station_thread = std::thread(&AssemblyStation::process_orders, this);
}

void AssemblyStation::stop() {
    running = false;
    order_cv.notify_all();
}

void AssemblyStation::process_orders() {
    while (running) {
        std::unique_lock<std::mutex> lock(queue_mutex);
        
        // Wait for orders
        order_cv.wait(lock, [this] { 
            return !running || !order_queue.empty(); 
        });
        
        if (!running && order_queue.empty()) break;
        
        if (order_queue.empty()) continue;
        
        Order order = order_queue.front();
        order_queue.pop();
        lock.unlock();
        
        // Request components - need product BOM
        if (request_components(order.product_id)) {
            // Calculate operation time
            int operation_time = calculate_operation_time(order.product_id);
            
            // Simulate assembly
            total_busy_time_minutes += operation_time;
            std::this_thread::sleep_for(std::chrono::milliseconds(operation_time * 10));
            
            // Mark order as complete
            order.is_completed = true;
            order.completion_time_minutes = current_sim_time_minutes + operation_time;
            orders_completed++;
            
            // Add finished product to warehouse
            warehouse->add_finished_product(order.product_id);
        }
    }
}

bool AssemblyStation::request_components(const std::string& product_id) {
    if (!products) {
        return false;
    }
    
    auto it = products->find(product_id);
    if (it == products->end()) {
        return false;
    }
    
    const Product& product = it->second;
    
    // Reserve components (this also checks availability)
    if (!warehouse->reserve_components(product.bom)) {
        return false;
    }
    
    // Assign AGVs to transport components
    if (!agv_fleet || agv_fleet->empty()) {
        warehouse->reserve_components(product.bom); // Rollback - simplified
        return false;
    }
    
    // Assign AGV tasks for each component
    int agv_index = 0;
    for (const auto& component : product.bom) {
        // Find idle AGV
        bool assigned = false;
        for (size_t i = 0; i < agv_fleet->size(); i++) {
            AGV* agv = (*agv_fleet)[(agv_index + i) % agv_fleet->size()];
            if (agv->is_idle()) {
                agv->assign_task(component.first, component.second, "ASSEMBLY_STATION");
                assigned = true;
                agv_index = (agv_index + i + 1) % agv_fleet->size();
                break;
            }
        }
        if (!assigned) {
            // Not enough AGVs available - simplified handling
        }
    }
    
    // Wait for components
    wait_for_components(product_id);
    
    return true;
}

void AssemblyStation::wait_for_components(const std::string& product_id) {
    // Simplified: just wait a bit to simulate component delivery
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

int AssemblyStation::calculate_operation_time(const std::string& product_id) {
    if (!products) {
        return 30 + setup_time_minutes; // Default fallback
    }
    
    auto it = products->find(product_id);
    if (it == products->end()) {
        return 30 + setup_time_minutes; // Default fallback
    }
    
    // T_op = T_base + T_setup
    return it->second.base_assembly_time_minutes + setup_time_minutes;
}

void AssemblyStation::add_order(const Order& order) {
    std::lock_guard<std::mutex> lock(queue_mutex);
    order_queue.push(order);
    order_cv.notify_one();
}

void AssemblyStation::set_simulation_time(int minutes) {
    current_sim_time_minutes = minutes;
}

bool AssemblyStation::is_processing() const {
    std::lock_guard<std::mutex> lock(queue_mutex);
    return !order_queue.empty();
}

