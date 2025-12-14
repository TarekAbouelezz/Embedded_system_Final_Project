/**
 * @file Product.h
 * @brief Product and Bill of Materials (BOM) definitions
 */

#ifndef PRODUCT_H
#define PRODUCT_H
/******************************Standard Libraries***************************************/
#include <string>
#include <map>
#include <vector>
/*************************************************************************************/

/****************************Product Structures***************************************/
/**
 * @struct ComponentRequirement
 * @brief Represents a component and its required quantity in the BOM
 */
struct ComponentRequirement {
    std::string component_id;
    int quantity;
    
    ComponentRequirement(const std::string& id, int qty) 
        : component_id(id), quantity(qty) {}
};

/**
 * @struct Product
 * @brief Represents a product with its BOM and base assembly time
 */
struct Product {
    std::string product_id;
    int base_assembly_time_minutes;  // T_base in minutes
    std::map<std::string, int> bom;  // component_id -> quantity
    
    Product() : base_assembly_time_minutes(0) {}
};
/*************************************************************************************/
#endif /* PRODUCT_H */


