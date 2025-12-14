/**
 * @file FileHandler.h
 * @brief File I/O utilities for reading input files and writing reports
 */

#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

/******************************Project Headers*****************************************/
#include "Order.h"
#include "Product.h"
#include <string>
#include <vector>
/**************************************************************************************/

/****************************FileHandler Class Definition*****************************/

/**
 * @class FileHandler
 * @brief Provides static methods for file reading and writing
 */
class FileHandler {
public:
    // Input file readers
    static bool read_orders_file(const std::string& filename, std::vector<Order>& orders);
    static bool read_bom_file(const std::string& filename, std::map<std::string, Product>& products);
    static bool read_warehouse_file(const std::string& filename, 
                                     std::map<std::string, int>& inventory);
    
    // Output file writers
    static bool write_kpi_report(const std::string& filename,
                                  double avg_lead_time,
                                  double station_utilization,
                                  double throughput,
                                  double agv_utilization);
    
    // Utility functions
    static bool file_exists(const std::string& filename);
    static std::vector<std::string> split_string(const std::string& str, char delimiter);
    static int time_to_minutes(int hour, int minute);
};
/*************************************************************************************/

#endif /* FILE_HANDLER_H */


