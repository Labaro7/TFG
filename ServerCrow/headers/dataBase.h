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
#include <sstream>
#include <mutex>
#include <deque>

// MySQL constants.
const std::string HOST = "127.0.0.1";
const int PORT = 3306;
const std::string USERNAME = "user";
const std::string PASSWORD = "user";
const std::string DATABASE_NAME = "example_db";

const int POOL_SIZE = 10;

class Database;

struct Connection {
    Database* database;
    int con_id;
    std::shared_ptr<sql::Connection> con;
    sql::Statement* stmt;
    sql::PreparedStatement* pstmt;

    // TODO: const
    Connection(Database* database, int con_id, std::shared_ptr<sql::Connection> con);
    ~Connection();
};

class Database {
public:
    Database();
    ~Database();


    // ------------------------------- MySQL queries ------------------------------- //
    // These are methods to create, update or remove databases and tables 

    // Database
    void MySqlCreateDatabase(const std::string name); // It works CREATE DATABASE
    void MySqlDropDatabase(const std::string name); // It works DROP DATABASE
    void MySqlUseDatabase(const std::string name); // USE
    void MySqlSaveChangesToDataBase(); // COMMIT    

    // Table
    void MySqlCreateTable(const std::string name, std::string definition); // It works CREATE TABLE
    void MySqlDropTable(const std::string name); // It works DROP TABLE IF EXISTS
    void MySqlModifyTable(const std::string name, std::string modifications); // ALTER TABLE
    void MySqlEmptyTable(const std::string name); // DELETE FROM

    // ------------------------------- /MySQL queries ------------------------------- //


    void initializeEmployeesTable();
    void initializeProductsTable();
    void initializeIngredientsTable();
    void initializeAllergensTable();
    void initializeOrdersTable();
    void initialize(); // Makes the corresponding database and tables with their corresponding columns.
    void dropAllTables();

    // Save
    void saveTable(const Table& table); // It works
    void saveEmployee(const Employee& employee); // It works
    void saveProduct(const Product& product); // It works
    void saveOrder(const Order& order); // It works
    void saveIngredient(const Ingredient& ingredient); // It works
    void saveAllergen(const Allergen& allergen); // It works

    void saveTableProduct(Table& table, const Product& product);
    void saveProductIngredient(const Product& product, const Ingredient& ingredient);
    void saveProductOrder(const Product& product, const Order& order);


    // Get
    std::vector<Table> getTables(); // It works
    Table getTableByNumber(const int n_table); // It works

    std::vector<Employee> getEmployees() const; // It works
    Employee getEmployeeByName(const std::string name) const; // It works

    std::vector<Product> getProducts() const; // It works
    Product getProductByName(const std::string name) const; // It works

    std::vector<Order> getOrders() const; // It works
    Order getOrderByTime(const std::string time) const; // It works

    std::vector<Ingredient> getIngredients() const; // It works
    Ingredient getIngredientByName(const std::string name) const; // It works

    std::vector<Allergen> getAllergens() const; // It works
    Allergen getAllergenByName(const std::string name) const; // It works


    // Print
    void printTables(); // It works
    void printEmployees(); // It works
    void printProducts(); // It works
    void printOrders(); // It works
    void printIngredients(); // It works
    void printAllergens(); // It works


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


    // Remove
    void removeTable(const Table& table); // It works
    void removeEmployee(const Employee& employee); // It works
    void removeProduct(const Product& product); // It works
    void removeOrder(const Order& order); // It works
    void removeIngredient(const Ingredient& ingredient); // It works
    void removeAllergen(const Allergen& allergen); // It works

private:
    sql::ConnectOptionsMap connection_properties;
    sql::Driver* driver;

    std::shared_ptr<sql::Connection> main_con;
    sql::Statement* main_stmt;
    sql::PreparedStatement* main_pstmt;

    std::mutex mutex; // It is supposed to be asynchronous so it is needed.

}; // class Database

#endif