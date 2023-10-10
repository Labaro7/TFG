#pragma once
#ifndef _SERVER_HPP_
#define _SERVER_HPP_

// Crow includes
#include "crow_all.h"

// Custom includes
#include "restaurant.h"
#include "dataBase.h"
#include "server.h"

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

    void saveTable(int n_table);
    void saveWorker(Worker* worker);
    void saveOrder();
    void removeTable();
    void removeOrder();

    std::unique_ptr<DataBase> dataBase;
    std::unique_ptr<Restaurant> restaurant;

}; // class Server

#endif