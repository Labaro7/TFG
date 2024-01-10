#pragma once
#ifndef _DATABASE_HPP_
#define _DATABASE_HPP_

// MySQL includes.
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

// Other includes
#include "domain.h"
#include "constants.h"
#include <sstream>
#include <mutex>
#include <deque>

class Database {
public:
    Database();
    ~Database();



    // ------------------------------- MySQL queries ------------------------------- //
    // These are methods to create, update or remove databases and tables 

    // Database
    void MySqlCreateDatabase(const std::string name);
    void MySqlDropDatabase(const std::string name);
    void MySqlUseDatabase(const std::string name);
    void MySqlSaveChangesToDataBase(); // COMMIT    

    // Table
    void MySqlCreateTable(const std::string name, std::string definition);
    void MySqlDropTable(const std::string name);
    void MySqlModifyTable(const std::string name, std::string modifications);
    void MySqlEmptyTable(const std::string name);

    // ------------------------------- /MySQL queries ------------------------------- //


    void initializeEmployeesTable();
    void initializeProductsTable();
    void initializeIngredientsTable();
    void initializeAllergensTable();
    void initializeOrdersTable();
    void initialize(); // Makes the corresponding database and tables with their corresponding columns.
    void dropAllTables();



    // ALL THE METHODS BELOW MUST USE THE MUTEX ATTRIBUTE
    // Save
    void saveTable(const Table& table);
    void saveEmployee(const Employee& employee);
    void saveProduct(const Product& product);
    void saveOrder(const Order& order);
    void saveOrderProduct(const Order& order, const int& product_id, const int& amount);
    void saveIngredient(const Ingredient& ingredient);
    void saveAllergen(const Allergen& allergen);

    void saveTableProduct(Table& table, const Product& product, const int& times);
    void saveProductIngredient(const Product& product, const Ingredient& ingredient);
    void saveProductOrder(const Product& product, const Order& order);


    // Get
    std::vector<Table> getTables();
    Table getTableByNumber(const int n_table);

    std::vector<Employee> getEmployees();
    Employee getEmployeeByName(const std::string name);

    std::vector<Product> getProducts();
    Product getProductByName(const std::string name);
    int getProductIdByName(const std::string name);
    std::vector<Product> getProductsByDeployableId(int id);
    std::pair<int, std::vector<Product>> getProductsAndIds();

    std::vector<Order> getOrders();
    Order getOrderByTime(const std::string time);

    std::vector<Ingredient> getIngredients();
    Ingredient getIngredientByName(const std::string name);

    std::vector<Allergen> getAllergens();
    Allergen getAllergenByName(const std::string name);

    std::vector<page_t> getDataFromPages();


    // Print
    void printTables();
    void printEmployees();
    void printProducts();
    void printOrders();
    void printIngredients();
    void printAllergens();


    // Set
    void setTable_NTable();
    void setTable_NClients();
    void setTable_Products(const Table table, const std::unordered_map<std::string, int> products);
    void setTable_Bill();
    void setTable_Discount();
    void setEmployee_Name();
    void setEmployee_Level();
    void setEmployee_Start();
    void setEmployee_Finish();

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
    sql::ConnectOptionsMap connection_properties;
    sql::Driver* driver;

    std::shared_ptr<sql::Connection> con;
    sql::Statement* stmt;
    sql::PreparedStatement* pstmt;

    std::mutex mutex; // It is supposed to be asynchronous so it is needed.

}; // class Database

#endif