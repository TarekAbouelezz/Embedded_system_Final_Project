/**
 * @file Warehouse.h
 * @brief Warehouse inventory management
 */

#ifndef WAREHOUSE_H
#define WAREHOUSE_H

/*****************************Standard Libraries***************************************/
#include <string>
#include <map>
#include <mutex>
#include <condition_variable>
/*************************************************************************************/

/*****************************Warehouse Class Definition*******************************/
class Warehouse {
private:
    std::map<std::string, int> components;      // component_id -> quantity
    std::map<std::string, int> finished_products; // product_id -> quantity
    mutable std::mutex inventory_mutex;
    
public:
    Warehouse();
    
    // Component management
    bool has_components(const std::map<std::string, int>& required);
    bool reserve_components(const std::map<std::string, int>& required);  // Checks and reserves atomically
    void add_component(const std::string& component_id, int quantity);
    int get_component_quantity(const std::string& component_id) const;
    
    // Finished product management
    void add_finished_product(const std::string& product_id);
    int get_finished_product_count(const std::string& product_id) const;
    
    // Inventory status
    void print_inventory() const;
};
/*************************************************************************************/
#endif /* WAREHOUSE_H */

