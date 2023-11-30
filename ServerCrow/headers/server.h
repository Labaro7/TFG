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
    void saveTable(const Table& table);
    void saveEmployee(const Employee& employee);
    void saveProduct(const Product& product);
    void saveOrder(const Order& order);
    void saveIngredient(const Ingredient& ingredient);
    void saveAllergen(const Allergen& allergen);
    void saveTableProduct(Table& table, const Product& product, const int& times);


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
    void removeTable(const Table& table);
    void removeEmployee(const Employee& employee);
    void removeProduct(const Product& product);
    void removeOrder(const Order& order);
    void removeIngredient(const Ingredient& ingredient);
    void removeAllergen(const Allergen& allergen);


private:
    std::unique_ptr<Database> _database;
    std::unique_ptr<Restaurant> restaurant;

}; // class Server
#endif