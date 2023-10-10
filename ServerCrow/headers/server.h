#pragma once
#ifndef _SERVER_HPP_
#define _SERVER_HPP_

// Crow includes
#include "crow_all.h"

// Custom includes
#include "restaurant.h"
#include "dataBase.h"
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
        dataBase = std::make_unique<DataBase>();
        restaurant = std::make_unique<Restaurant>();
    }

    // Destructor.
    ~Server() { }

    void saveTable(int n_table); // It works
    void saveWorker(Worker* worker); // It works
    void saveOrder();
    void removeTable();
    void removeOrder();

    std::vector<Table> getTables(); // It works
    Table getTable(int n_table); // It works

    std::unique_ptr<DataBase> dataBase;
    std::unique_ptr<Restaurant> restaurant;

}; // class Server

#endif