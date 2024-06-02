#pragma once
#ifndef DATABASE_MANAGER_HPP
#define DATABASE_MANAGER_HPP

#include <map>
#include <string>
#include <memory>
#include "database.hpp"

class DatabaseManager
{
public:
    ~DatabaseManager();

    DatabaseManager(const DatabaseManager&) = delete;

    //DatabaseManager& operator=(const DatabaseManager&) = delete;

    static DatabaseManager& getInstance();

    std::string getDatabases();

    std::shared_ptr<Database> getCurrentDatabase();

    std::shared_ptr<Database> getDatabase(const std::string& databaseName);

    void setCurrentDatabase(const std::string& databaseName);

    std::shared_ptr<Database> createDatabase(const std::string& hostName,
                             const int& port,
                             const std::string& username,
                             const std::string& password,
                             const std::string& databaseName);


private:
    DatabaseManager(); 

    std::shared_ptr<Database> currentDatabase;
    std::map<std::string, std::shared_ptr<Database>> databases;
};

#endif