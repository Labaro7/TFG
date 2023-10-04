#pragma once
#ifndef _DATABASE_HPP_
#define _DATABASE_HPP_

// MySQL includes.
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

// MySQL constants.
const std::string SERVER = "tcp://127.0.0.1:3306";
const std::string USERNAME = "user";
const std::string PASSWORD = "user";
const std::string DATABASE_NAME = "example_db";

class DataBase {
public:
    // Default constructor.
    DataBase() : stmt(0), pstmt(0) {
        try
        {
            driver = get_driver_instance();
            con = driver->connect(server, username, password);
        }
        catch (sql::SQLException e)
        {
            std::cout << "Could not connect to server. Error message: " << e.what() << std::endl;
            system("pause");
            exit(1);
        }

        con->setSchema(dataBaseName);
    }

    // Destructor.
    ~DataBase() {
        delete con;
        delete stmt;
        delete pstmt;
    }

    // Methods.
    sql::Driver* getDriver();
    sql::Connection* getCon();
    sql::Statement* getStmt();
    sql::PreparedStatement* getPstmt();

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


// Methods implementations.
// Getters.
sql::Driver* DataBase::getDriver() { return driver; }
sql::Connection* DataBase::getCon() { return con; }
sql::Statement* DataBase::getStmt() { return stmt; }
sql::PreparedStatement* DataBase::getPstmt() { return pstmt; }

// Setters.


#endif