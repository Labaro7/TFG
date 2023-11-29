#pragma once
#ifndef _SERVER_HPP_
#define _SERVER_HPP_

// Crow includes
#include "crow_all.h"

// Custom includes
#include "database.h"

// Other includes
#include <sstream>
#include <string>
#include <memory>


class Server {  
public:
    Server();
    ~Server();


    void initialize(); // Initializes the database to the model of the restaurant.
    std::unique_ptr<Database>& database(); // Returns the database to operate with its methods
    void dropAllTables();


    // Save
    void saveTable(const Table& table); // It works
    void saveEmployee(const Employee& employee); // It works
    void saveProduct(const Product& product); // It works
    void saveOrder(const Order& order); // It works
    void saveIngredient(const Ingredient& ingredient); // It works
    void saveAllergen(const Allergen& allergen); // It works
    void saveTableProduct(Table& table, const Product& product);


    // Get
    productsMenus_t getDataFromPage(const int n_page);
    int getRestaurantPagesSize() const;

    std::vector<Table> getTables() const; // It works. 
    Table getTableByNumber(const int n_table) const; // It works 

    std::vector<Employee> getEmployees() const; // It works
    Employee getEmployeeByName(const std::string name) const; // It works

    std::vector<Product> getProducts() const; // It works
    Product getProductByName(const std::string name) const; // It works

    std::vector<Ingredient> getIngredients() const; // It works
    Ingredient getIngredientByName(const std::string name) const;

    std::vector<Allergen> getAllergens() const; // It works
    Allergen getAllergenByName(const std::string name) const; // It works

    // Change
    void moveTable(int current_n_table, const int new_n_table);

    // Remove
    void removeTable(const Table& table); // It works
    void removeEmployee(const Employee& employee); // It works
    void removeProduct(const Product& product); // It works
    void removeOrder(const Order& order); // It works
    void removeIngredient(const Ingredient& ingredient); // It works
    void removeAllergen(const Allergen& allergen); // It works


private:
    std::unique_ptr<Database> _database;
    std::unique_ptr<Restaurant> restaurant;

}; // class Server
#endif