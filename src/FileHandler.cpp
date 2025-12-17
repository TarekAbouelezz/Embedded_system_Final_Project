/**
 * @file FileHandler.cpp
 * @brief File I/O implementation
 */

/******************************Project Headers*****************************************/
#include "FileHandler.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <sys/stat.h>
#include <sys/types.h>
#ifdef _WIN32
#include <direct.h>
#include <io.h>
#else
#include <unistd.h>
#endif
/*************************************************************************************/

/****************************FileHandler Methods*************************************/

/**
 * @brief Read orders from a file
 * @param filename Path to the orders file
 * @param orders Vector to populate with read orders
 * @return true if successful, false otherwise
 */
bool FileHandler::read_orders_file(const std::string& filename, std::vector<Order>& orders) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return false;
    }
    
    std::string line;
    int order_id = 1;  // Start order IDs from 1
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        int hour, minute, priority = 0;
        std::string product_id;
        
        if (iss >> hour >> minute >> product_id) {
            iss >> priority;  // Optional priority
            
            Order order;
            order.order_id = order_id++;
            order.release_hour = hour;
            order.release_minute = minute;
            order.release_time_minutes = time_to_minutes(hour, minute);
            order.product_id = product_id;
            order.priority = priority;
            
            orders.push_back(order);
        }
    }
    
    file.close();
    return true;
}


/**
 * @brief Read Bill of Materials (BOM) from a file
 * @param filename Path to the BOM file
 * @param products Map to populate with read products
 * @return true if successful, false otherwise
 */
bool FileHandler::read_bom_file(const std::string& filename, std::map<std::string, Product>& products) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return false;
    }
    
    std::string line;
    std::string current_product_id;
    
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string tok;
        while (iss >> tok) tokens.push_back(tok);
        if (tokens.empty()) continue;
        
        // Formats supported:
        // 1) product_id base_time
        // 2) product_id component_id quantity
        // 3) component_id quantity (uses last current_product_id)
        if (tokens.size() == 2 && tokens[0][0] == 'P') {
            // product_id base_time
            std::istringstream bt(tokens[1]);
            int base_time = 0;
            if (bt >> base_time && bt.eof()) {
                current_product_id = tokens[0];
                Product& p = products[current_product_id];
                p.product_id = current_product_id;
                p.base_assembly_time_minutes = base_time;
            }
        } else if (tokens.size() == 3 && tokens[0][0] == 'P' && tokens[1][0] == 'C') {
            // product_id component_id quantity
            std::istringstream qss(tokens[2]);
            int qty = 0;
            if (qss >> qty && qss.eof()) {
                const std::string& pid = tokens[0];
                const std::string& cid = tokens[1];
                Product& p = products[pid];
                if (p.product_id.empty()) p.product_id = pid;
                p.bom[cid] = qty;
                current_product_id = pid;
            }
        } else if (tokens.size() == 2 && tokens[0][0] == 'C' && !current_product_id.empty()) {
            // component_id quantity for current product
            std::istringstream qss(tokens[1]);
            int qty = 0;
            if (qss >> qty && qss.eof()) {
                const std::string& cid = tokens[0];
                Product& p = products[current_product_id];
                p.bom[cid] = qty;
            }
        }
    }
    
    file.close();
    return true;
}


/**
 * @brief Read warehouse inventory from a file
 * @param filename Path to the warehouse file
 * @param inventory Map to populate with component quantities
 * @return true if successful, false otherwise
 */
bool FileHandler::read_warehouse_file(const std::string& filename,
                                       std::map<std::string, int>& inventory) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        std::string component_id;
        int quantity;
        
        if (iss >> component_id >> quantity) {
            inventory[component_id] = quantity;
        }
    }
    
    file.close();
    return true;
}



/**
 * @brief Write KPI report to file
 * @param filename Path to the output KPI report file
 * @param avg_lead_time Average lead time in minutes
 * @param station_utilization Assembly station utilization (0.0 - 1.0)
 * @param throughput Throughput in orders per hour
 * @param agv_utilization AGV utilization (0.0 - 1.0)
 * @return true if successful, false otherwise
 */
bool FileHandler::write_kpi_report(const std::string& filename,
                                    double avg_lead_time,
                                    double station_utilization,
                                    double throughput,
                                    double agv_utilization) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot create file " << filename << std::endl;
        return false;
    }
    
    file << "========================================\n";
    file << "  Key Performance Indicators Report    \n";
    file << "========================================\n\n";
    
    file << "Average Lead Time: " << avg_lead_time << " minutes\n";
    file << "Assembly Station Utilization: " << (station_utilization * 100) << "%\n";
    file << "Throughput: " << throughput << " orders/hour\n";
    file << "Average AGV Utilization: " << (agv_utilization * 100) << "%\n";
    
    file.close();
    return true;
}



/**
 * @brief Check if a file exists
 * @param filename Path to the file
 * @return true if the file exists, false otherwise
 */
bool FileHandler::file_exists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}


/**
 * @brief Split a string by a delimiter
 * @param str The input string
 * @param delimiter The delimiter character
 * @return Vector of split substrings
 */
std::vector<std::string> FileHandler::split_string(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}


/**
 * @brief Convert time in hours and minutes to total minutes
 * @param hour Hour component
 * @param minute Minute component
 * @return Total time in minutes
 */
int FileHandler::time_to_minutes(int hour, int minute) {
    return hour * 60 + minute;
}

/*************************************************************************************/