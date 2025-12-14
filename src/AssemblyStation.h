/**
 * @file AssemblyStation.h
 * @brief Assembly station that processes orders
 */

#ifndef ASSEMBLY_STATION_H
#define ASSEMBLY_STATION_H

#include "Order.h"
#include "Product.h"
#include "Warehouse.h"
#include <map>

// Forward declaration
class AGV;
#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>

class AssemblyStation {
private:
    Warehouse* warehouse;
    std::vector<AGV*>* agv_fleet;
    std::map<std::string, Product>* products;  // Reference to product BOM
    std::queue<Order> order_queue;
    std::mutex queue_mutex;
    std::condition_variable order_cv;
    std::thread station_thread;
    std::atomic<bool> running;
    std::atomic<int> current_sim_time_minutes;
    
    // Configuration
    int setup_time_minutes;  // T_setup
    
    void process_orders();
    bool request_components(const std::string& product_id);
    void wait_for_components(const std::string& product_id);
    int calculate_operation_time(const std::string& product_id);
    
    // Statistics
    int total_busy_time_minutes;
    int orders_completed;
    
public:
    AssemblyStation(Warehouse* wh, std::vector<AGV*>* fleet);
    ~AssemblyStation();
    
    void start();
    void stop();
    void add_order(const Order& order);
    void set_simulation_time(int minutes);
    void set_products(std::map<std::string, Product>* prods) { products = prods; }
    
    // Statistics
    int get_total_busy_time() const { return total_busy_time_minutes; }
    int get_orders_completed() const { return orders_completed; }
    bool is_processing() const;
};

#endif /* ASSEMBLY_STATION_H */

