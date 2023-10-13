#include "..\headers\server.h"
#include "..\headers\crow_all.h"
#include <sstream>


class ExampleLogHandler : public crow::ILogHandler{
public:
    void log(std::string /*message*/, crow::LogLevel /*level*/) override
    {
        // cerr << "ExampleLogHandler -> " << message;
    }
};


void Server::saveTable(Table* table) { database->saveTable(table); }

void Server::saveEmployee(Employee* employee) { database->saveEmployee(employee); }

void Server::saveProduct(Product* product) { database->saveProduct(product); }

void Server::saveOrder(Order* order) { database->saveOrder(order); }

void Server::saveIngredient(Ingredient* ingredient) { database->saveIngredient(ingredient); }

void Server::saveAllergen(Allergen* allergen) { database->saveAllergen(allergen); }

std::vector<Table> Server::getTables() { return database->getTables(); }

Table Server::getTableByNumber(int n_table) { return database->getTableByNumber(n_table); }

std::vector<Employee> Server::getEmployees() { return database->getEmployees(); }

Employee Server::getEmployeeByName(std::string name) { return database->getEmployeeByName(name); }
