/**
 * @file Order.h
 * @brief Order data structure for production orders
 */

#ifndef ORDER_H
#define ORDER_H

/*****************************Standard Libraries***************************************/
#include <stdint.h>
#include <string>
#include <ctime>
/*************************************************************************************/

/*****************************Order Structure Definition*******************************/
struct Order {
    int release_hour;
    int release_minute;
    int release_time_minutes;  // Total minutes from simulation start
    std::string product_id;
    int priority;
    int due_date_minutes;      // Optional: due date in minutes from start
    int completion_time_minutes;
    bool is_completed;         // Flag indicating if order is completed
    
    Order() : release_hour(0), release_minute(0), release_time_minutes(0),
              priority(0), due_date_minutes(-1), completion_time_minutes(-1),
              is_completed(false) {} 
};
/*************************************************************************************/
#endif /* ORDER_H */


