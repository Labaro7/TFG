#pragma once
#ifndef _SERVER_HPP_
#define _SERVER_HPP_

// Crow includes
#include "crow_all.h"

// Custom includes
#include "restaurant.h"
#include "database.h"
#include "server.h"
#include "table.h"

// Other includes
#include <sstream>
#include <string>
#include <memory>

class Server {
public:
    // Default constructor.
    Server() {
        database = std::make_unique<Database>();
        restaurant = std::make_unique<Restaurant>();
    }

    // Destructor.
    ~Server() { }

    // Save
    void saveTable(Table* table); // It works
    void saveEmployee(Employee* employee); // It works
    void saveOrder();

    // Get
    std::vector<Table> getTables(); // It works. //TODO: Change parameter to Table table
    Table getTableByNumber(int n_table); // It works
    std::vector<Employee> getEmployees(); // It works
    Employee getEmployeeByName(std::string name); // It works

    // Remove
    void removeTable();
    void removeOrder();


    std::unique_ptr<Database> database;
    std::unique_ptr<Restaurant> restaurant;

}; // class Server
#endif