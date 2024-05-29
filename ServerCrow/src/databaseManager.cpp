#include "../include/databaseManager.hpp"

DatabaseManager::DatabaseManager()
{
    currentDatabase = std::make_shared<Database>();
}

DatabaseManager& DatabaseManager::getInstance()
{
    static DatabaseManager dbManager;

    return dbManager;
}

std::string DatabaseManager::getDatabases()
{
    std::stringstream ss;

    for (const auto entry : databases)
    {
        ss << entry.second->getName() << std::endl;
    }

    return ss.str();
}

std::shared_ptr<Database> DatabaseManager::getCurrentDatabase()
{
    return currentDatabase;
}

std::shared_ptr<Database> DatabaseManager::getDatabase(const std::string& databaseName)
{
    if (currentDatabase != nullptr)
    {
        return currentDatabase;
    }

    return nullptr;
}

void DatabaseManager::setCurrentDatabase(const std::string& databaseName)
{
    auto it = databases.find(databaseName);
    if (it != databases.end())
    {
        currentDatabase = it->second;
    }
}

std::shared_ptr<Database> DatabaseManager::createDatabase(const std::string& hostName,
                                          const int& port,
                                          const std::string& username,
                                          const std::string& password,
                                          const std::string& databaseName)
{
    if (databases.find(databaseName) == databases.end())
    {
        databases[databaseName] = std::make_shared<Database>(hostName,
                                                     port,
                                                     username,
                                                     password,
                                                     databaseName);
    }

    return databases[databaseName];
}

DatabaseManager::~DatabaseManager()
{
   
}
