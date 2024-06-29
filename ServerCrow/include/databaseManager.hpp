#pragma once
#ifndef DATABASE_MANAGER_HPP
#define DATABASE_MANAGER_HPP

#include <map>
#include <string>
#include <memory>
#include "database.hpp"
#include "singletonTemplate.hpp"

class DatabaseManager : public SingletonTemplate<DatabaseManager>
{
public:
    // Allow Singleton<MyClass> to access the private constructor
    friend class SingletonTemplate<DatabaseManager>;

    ~DatabaseManager();

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
    std::unordered_map<std::string, std::shared_ptr<Database>> databases;
};

#endif