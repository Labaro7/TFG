#pragma once
#ifndef _DATABASE_HPP_
#define _DATABASE_HPP_

// MySQL includes.
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

// Other includes
#include "worker.h"
#include "product.h"
#include "table.h"
#include "order.h"
#include "ingredient.h"
#include "allergen.h"
#include <sstream>

// MySQL constants.
const std::string SERVER = "tcp://127.0.0.1:3306";
const std::string USERNAME = "user";
const std::string PASSWORD = "user";
const std::string DATABASE_NAME = "example_db";

class Database {
public:
    Database();
    ~Database();


    // Get attributes
    sql::Driver* getDriver();
    sql::Connection* getCon();
    sql::Statement* getStmt();
    sql::PreparedStatement* getPstmt();


    // ------------------------------- MySQL queries ------------------------------- //
    // These are methods to create, update or remove databases and tables 

    // Database
    void MySqlCreateDatabase(std::string name); // CREATE DATABASE
    void MySqlDropDatabase(std::string name); // DROP DATABASE
    void MySqlUseDatabase(std::string name); // USE
    void MySqlSaveChangesToDataBase(); // COMMIT    
    
    // Table
    void MySqlCreateTable(std::string name, std::string definition); // CREATE TABLE
    void MySqlDropTable(std::string name); // DROP TABLE IF EXISTS
    void MySqlModifyTable(std::string name, std::string modifications); // ALTER TABLE
    void MySqlEmptyTable(std::string name); // DELETE FROM
    
    // ------------------------------- /MySQL queries ------------------------------- //

    void initialize(); // Makes the corresponding database and tables with their corresponding columns.

    // Save
    // TODO: Check if the row is in the table for each save method
    void saveTable(Table* table);
    void saveWorker(Worker* worker); // It works
    void saveProduct(Product* product);
    void saveOrder(Order* order);
    void saveIngredient(Ingredient* ingredient);
    void saveAllergen(Allergen* allergen);

    // Get
    std::vector<Table> getTables(); // It works
    Table getTableByNumber(int n_table); // It works
    std::vector<Worker> getWorkers(); // It works
    Worker getWorkerByName(std::string name); // It works

    // Set
    void setTable_NTable();
    void setTable_NClients();
    void setTable_Bill();
    void setTable_Discount();
    void setWorker_Name();
    void setWorker_Rank();
    void setWorker_Start();
    void setWorker_Finish();

    // Remove
    void removeTable(Table* table);
    void removeWorker(Worker* worker);
    void removeProduct(Product* product);
    void removeOrder(Order* order);
    void removeIngredient(Ingredient* ingredient);
    void removeAllergen(Allergen* allergen);

private:
	std::string server = SERVER;
	std::string username = USERNAME;
	std::string password = PASSWORD;
    std::string dataBaseName = DATABASE_NAME;
    
	sql::Driver* driver;
	sql::Connection* con;
	sql::Statement* stmt;
	sql::PreparedStatement* pstmt;

}; // class Database
#endif