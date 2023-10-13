#include "..\headers\server.h"
#include "..\headers\crow_all.h"
#include <sstream>

Server::Server() {
    database = std::make_unique<Database>();
    restaurant = std::make_unique<Restaurant>();
}

Server::~Server(){}


std::unique_ptr<Database>& Server::getDatabase()
{
    return database;
}

void Server::initialize() {
    getDatabase()->initialize();
}

class ExampleLogHandler : public crow::ILogHandler{
public:
    void log(std::string /*message*/, crow::LogLevel /*level*/) override
    {
        // cerr << "ExampleLogHandler -> " << message;
    }
};


// Save
void Server::saveTable(Table* table) { database->saveTable(table); }

void Server::saveEmployee(Employee* employee) { database->saveEmployee(employee); }

void Server::saveProduct(Product* product) { database->saveProduct(product); }

void Server::saveOrder(Order* order) { database->saveOrder(order); }

void Server::saveIngredient(Ingredient* ingredient) { database->saveIngredient(ingredient); }

void Server::saveAllergen(Allergen* allergen) { database->saveAllergen(allergen); }


// Get
std::vector<Table> Server::getTables() { return database->getTables(); }

Table Server::getTableByNumber(int n_table) { return database->getTableByNumber(n_table); }

std::vector<Employee> Server::getEmployees() { return database->getEmployees(); }

Employee Server::getEmployeeByName(std::string name) { return database->getEmployeeByName(name); }


// Remove
void Server::removeTable(Table* table) { database->removeTable(table); }

void Server::removeEmployee(Employee* employee) { database->removeEmployee(employee); }

void Server::removeProduct(Product* product) { database->removeProduct(product); }

void Server::removeOrder(Order* order) { database->removeOrder(order); }

void Server::removeIngredient(Ingredient* ingredient) { database->removeIngredient(ingredient); }

void Server::removeAllergen(Allergen* allergen) { database->removeAllergen(allergen); }

