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
    void getDataFromDatabase();
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
    //page_t getDataFromPage(const int n_page);
    std::vector<page_t> getDataFromPages();
    int getRestaurantPagesSize() const;

    std::vector<Table> getTables() const;
    Table getTableByNumber(const int n_table) const; 

    std::vector<Employee> getEmployees() const;
    Employee getEmployeeByName(const std::string name) const;

    std::vector<Product> getProducts() const;
    Product getProductByName(const std::string name) const;
    int getProductIdByName(const std::string name) const;

    std::vector<Order> getOrders() const;

    std::vector<Ingredient> getIngredients() const;
    Ingredient getIngredientByName(const std::string name) const;

    std::vector<Allergen> getAllergens() const;
    Allergen getAllergenByName(const std::string name) const;

    // Change
    void moveTable(int current_n_table, const int new_n_table);

    // Remove
    void removeTable(const Table& table);
    void removeEmployee(const Employee& employee);
    void removeProduct(const Product& product);
    void removeOrder(const Order& order);
    void removeIngredient(const Ingredient& ingredient);
    void removeAllergen(const Allergen& allergen);

    void payTable(const int& n_table, const std::string& employee, const std::string& date);


private:
    std::unique_ptr<Database> _database;
public:
    std::unique_ptr<Restaurant> restaurant;

    //std::mutex mutex;

}; // class Server
#endif