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
    void saveTable(int n_table); // It works
    void saveWorker(Worker* worker); // It works
    void saveOrder();

    // Get
    std::vector<Table> getTables(); // It works
    Table getTableByNumber(int n_table); // It works
    std::vector<Worker> getWorkers(); // It works
    Worker getWorkerByName(std::string name); // It works

    // Remove
    void removeTable();
    void removeOrder();


    std::unique_ptr<Database> database;
    std::unique_ptr<Restaurant> restaurant;

}; // class Server

#endif