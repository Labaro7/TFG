#include "..\headers\server.h"
#include "..\headers\crow_all.h"
#include <sstream>

Server::Server() {
    _database = std::make_unique<Database>();
    restaurant = std::make_unique<Restaurant>();
}

Server::~Server(){}


std::unique_ptr<Database>& Server::database()
{
    return _database;
}

void Server::initialize() {
    database()->initialize();
}


// Save
void Server::saveTable(Table* table) { _database->saveTable(table); }

void Server::saveEmployee(Employee* employee) { _database->saveEmployee(employee); }

void Server::saveProduct(Product* product) { _database->saveProduct(product); }

void Server::saveOrder(Order* order) { _database->saveOrder(order); }

void Server::saveIngredient(Ingredient* ingredient) { _database->saveIngredient(ingredient); }

void Server::saveAllergen(Allergen* allergen) { _database->saveAllergen(allergen); }


// Get
std::vector<Table> Server::getTables() const { return _database->getTables(); }

Table Server::getTableByNumber(int n_table) const { return _database->getTableByNumber(n_table); }

std::vector<Employee> Server::getEmployees() const { return _database->getEmployees(); }

Employee Server::getEmployeeByName(std::string name) const { return _database->getEmployeeByName(name); }

std::vector<Product> Server::getProducts() const { return _database->getProducts(); }

Product Server::getProductByName(std::string name) const { return _database->getProductByName(name); }

std::vector<Ingredient> Server::getIngredients() const { return _database->getIngredients(); }

Ingredient Server::getIngredientByName(std::string name) const { return _database->getIngredientByName(name); }

std::vector<Allergen> Server::getAllergens() const { return _database->getAllergens(); }

Allergen Server::getAllergenByName(std::string name) const { return _database->getAllergenByName(name); }


// Remove
void Server::removeTable(Table* table) { _database->removeTable(table); }

void Server::removeEmployee(Employee* employee) { _database->removeEmployee(employee); }

void Server::removeProduct(Product* product) { _database->removeProduct(product); }

void Server::removeOrder(Order* order) { _database->removeOrder(order); }

void Server::removeIngredient(Ingredient* ingredient) { _database->removeIngredient(ingredient); }

void Server::removeAllergen(Allergen* allergen) { _database->removeAllergen(allergen); }

