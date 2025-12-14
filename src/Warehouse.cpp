/**
 * @file Warehouse.cpp
 * @brief Warehouse implementation
 */

/*****************************Project Headers*****************************************/
#include "Warehouse.h"

/*************************************************************************************/

/*****************************Standard Libraries**************************************/
#include <iostream>

/*************************************************************************************/

/*****************************Warehouse Methods***************************************/
/**
 * @brief Constructor for Warehouse
 */
Warehouse::Warehouse() {
    // Initialize empty warehouse
}


/**
 * @brief Check if required components are available in the warehouse
 * @param required Map of component_id to required quantity
 * @return true if all required components are available, false otherwise
 */
bool Warehouse::has_components(const std::map<std::string, int>& required) {
    std::lock_guard<std::mutex> lock(inventory_mutex);
    
    for (const auto& req : required) {
        auto it = components.find(req.first);
        if (it == components.end() || it->second < req.second) { // Not enough quantity
            return false;
        }
    }
    return true;
}


/**
 * @brief Reserve required components atomically
 * @param required Map of component_id to required quantity
 * @return true if reservation is successful, false otherwise
 */
bool Warehouse::reserve_components(const std::map<std::string, int>& required) {
    std::lock_guard<std::mutex> lock(inventory_mutex);
    
    // Check availability first
    for (const auto& req : required) {
        auto it = components.find(req.first);
        if (it == components.end() || it->second < req.second) {
            return false;
        }
    }
    
    // Reserve components
    for (const auto& req : required) {
        components[req.first] -= req.second; // Deduct reserved quantity
    }
    
    return true;
}


/**
 * @brief Add components to the warehouse inventory
 * @param component_id The ID of the component
 * @param quantity The quantity to add
 */
void Warehouse::add_component(const std::string& component_id, int quantity) {
    std::lock_guard<std::mutex> lock(inventory_mutex);
    components[component_id] += quantity;
}


/**
 * @brief Get the quantity of a specific component in the warehouse
 * @param component_id The ID of the component
 * @return The quantity available
 */
int Warehouse::get_component_quantity(const std::string& component_id) const {
    std::lock_guard<std::mutex> lock(inventory_mutex);
    auto it = components.find(component_id);
    return (it != components.end()) ? it->second : 0;
}


/**
 * @brief Add a finished product to the warehouse inventory
 * @param product_id The ID of the finished product
 */
void Warehouse::add_finished_product(const std::string& product_id) {
    std::lock_guard<std::mutex> lock(inventory_mutex);
    finished_products[product_id]++;
}



/**
 * @brief Get the count of a specific finished product in the warehouse
 * @param product_id The ID of the finished product
 * @return The count available
 */
int Warehouse::get_finished_product_count(const std::string& product_id) const {
    std::lock_guard<std::mutex> lock(inventory_mutex);
    auto it = finished_products.find(product_id);
    return (it != finished_products.end()) ? it->second : 0;
}



/**
 * @brief Print the current inventory status of the warehouse
 */
void Warehouse::print_inventory() const {
    std::lock_guard<std::mutex> lock(inventory_mutex);
    
    std::cout << "\n=== Warehouse Inventory ===\n";
    std::cout << "Components:\n";
    for (const auto& comp : components) {
        std::cout << "  " << comp.first << ": " << comp.second << std::endl;
    }
    
    std::cout << "\nFinished Products:\n";
    for (const auto& prod : finished_products) {
        std::cout << "  " << prod.first << ": " << prod.second << std::endl;
    }
}


/*************************************************************************************/