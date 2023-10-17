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


    // Save
    void saveTable(Table* table); // It works
    void saveEmployee(Employee* employee); // It works
    void saveProduct(Product* product); // It works
    void saveOrder(Order* order); // It works
    void saveIngredient(Ingredient* ingredient); // It works
    void saveAllergen(Allergen* allergen); // It works


    // Get
    std::vector<Table> getTables() const; // It works. 
    Table getTableByNumber(int n_table) const; // It works 

    std::vector<Employee> getEmployees() const; // It works
    Employee getEmployeeByName(std::string name) const; // It works

    std::vector<Product> getProducts() const; // It works
    Product getProductByName(std::string name) const; // It works

    std::vector<Ingredient> getIngredients() const; // It works
    Ingredient getIngredientByName(std::string name) const;

    std::vector<Allergen> getAllergens() const; // It works
    Allergen getAllergenByName(std::string name) const; // It works


    // Remove
    void removeTable(Table* table); // It works
    void removeEmployee(Employee* employee); // It works
    void removeProduct(Product* product); // It works
    void removeOrder(Order* order); // It works
    void removeIngredient(Ingredient* ingredient); // It works
    void removeAllergen(Allergen* allergen); // It works


private:
    std::unique_ptr<Database> _database;
    std::unique_ptr<Restaurant> restaurant;

}; // class Server
#endif