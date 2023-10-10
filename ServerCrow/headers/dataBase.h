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
#include <sstream>

// MySQL constants.
const std::string SERVER = "tcp://127.0.0.1:3306";
const std::string USERNAME = "user";
const std::string PASSWORD = "user";
const std::string DATABASE_NAME = "example_db";

class DataBase {
public:
    DataBase();
    ~DataBase();

    sql::Driver* getDriver();
    sql::Connection* getCon();
    sql::Statement* getStmt();
    sql::PreparedStatement* getPstmt();

    // ------------------------------- MySQL methods ------------------------------- //
    // 
    // Database
    void MySqlCreateDatabase(); // CREATE DATABASE
    void MySqlDropDatabase(); // DROP DATABASE
    void MySqlUseDatabase(); // USE
    void MySqlSaveChangesToDataBase(); // COMMIT    
    //
    // Table
    void MySqlCreateTable(); // CREATE TABLE
    void MySqlDropTable(); // DROP TABLE IF EXISTS
    void MySqlModifyTable(); // ALTER TABLE
    void MySqlEmptyTable(); // DELETE FROM
    //
    // Row
    void MySqlInsertRowIntoTable(); // INSERT INTO
    void MySqlSelectAllFromTable(); // SELECT *
    void MySqlSelectRowFromTable(); // SELECT
    void MySqlUpdateRowFromTable(); // UPDATE
    void MySqlDeleteAllRowsFromTable(); // TRUNCATE TABLE
    //
    // ------------------------------- MySQL methods ------------------------------- //


    void saveTable(int n_table); // It works
    void saveWorker(Worker* worker); // It works
    void saveProduct(Product* product);
    void saveOrder();
    void saveAllergen();
    void saveIngredient();

    void removeTable(int n_table);
    void removeOrder(Worker* worker);
    void removeWorker(Product* product);
    void removeProduct();
    void removeAllergen();
    void removeIngredient();

    std::vector<Table> getTables(); // It works
    Table getTable(int n_table); // It works
    std::vector<Worker> getWorkers();
    Worker getWorker(Worker* worker);


private:
	std::string server = SERVER;
	std::string username = USERNAME;
	std::string password = PASSWORD;
    std::string dataBaseName = DATABASE_NAME;
    
	sql::Driver* driver;
	sql::Connection* con;
	sql::Statement* stmt;
	sql::PreparedStatement* pstmt;

}; // class DataBase
#endif