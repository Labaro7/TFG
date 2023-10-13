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
    Server();
    ~Server();

    void initialize(); // Initializes the database to the model of the restaurant.


    // Save
    void saveTable(Table* table); // It works
    void saveEmployee(Employee* employee); // It works
    void saveProduct(Product* product); // It works
    void saveOrder(Order* order); // It works
    void saveIngredient(Ingredient* ingredient); // It works
    void saveAllergen(Allergen* allergen); // It works

    // Get
    std::unique_ptr<Database>& getDatabase();
    std::vector<Table> getTables(); // It works. 
    Table getTableByNumber(int n_table); // It works //TODO: Change parameter to Table table
    std::vector<Employee> getEmployees(); // It works
    Employee getEmployeeByName(std::string name); // It works

    // Remove
    void removeTable(Table* table); // It works
    void removeEmployee(Employee* employee); // It works
    void removeProduct(Product* product); // It works
    void removeOrder(Order* order); // It works
    void removeIngredient(Ingredient* ingredient); // It works
    void removeAllergen(Allergen* allergen); // It works

private:
    std::unique_ptr<Database> database;
    std::unique_ptr<Restaurant> restaurant;

}; // class Server
#endif