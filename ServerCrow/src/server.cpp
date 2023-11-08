#include "..\headers\server.h"
#include "..\headers\crow_all.h"
#include <sstream>

Server::Server() {
    _database = std::make_unique<Database>();
    restaurant = std::make_unique<Restaurant>();
}

Server::~Server(){}


std::unique_ptr<Database>& Server::database(){ return _database; }

void Server::initialize() { 
    database()->initialize(); 

    // Make the restaurant instances that store different configs of products
    using productsMenus_t = std::vector<std::tuple<std::string, int, std::vector<std::pair<std::string, int>>>>;
    
    std::vector<productsMenus_t> pages(5);
    std::vector<std::pair<std::string, int>> vec;
    std::tuple<std::string, int, std::vector<std::pair<std::string, int>>> tup1 = { "a", 1.0, vec };
    std::tuple<std::string, int, std::vector<std::pair<std::string, int>>> tup2 = { "b", 2.0, vec };
    std::tuple<std::string, int, std::vector<std::pair<std::string, int>>> tup3 = { "c", 3.0, vec };
    std::tuple<std::string, int, std::vector<std::pair<std::string, int>>> tup4 = { "d", 4.0, vec };
    std::tuple<std::string, int, std::vector<std::pair<std::string, int>>> tup5 = { "e", 5.0, vec };
    std::tuple<std::string, int, std::vector<std::pair<std::string, int>>> tup6 = { "f", 6.0, vec };

    pages[0] = { tup1, tup2, tup3, tup4, tup5, tup6 };
    restaurant->pages = pages;
}

void Server::dropAllTables() { database()->dropAllTables(); }


// Save
void Server::saveTable(Table* table) { _database->saveTable(table); }

void Server::saveEmployee(Employee* employee) { _database->saveEmployee(employee); }

void Server::saveProduct(Product* product) { _database->saveProduct(product); }

void Server::saveOrder(Order* order) { _database->saveOrder(order); }

void Server::saveIngredient(Ingredient* ingredient) { _database->saveIngredient(ingredient); }

void Server::saveAllergen(Allergen* allergen) { _database->saveAllergen(allergen); }


// Get
using productsMenus_t = std::vector<std::tuple<std::string, int, std::vector<std::pair<std::string, int>>>>;
productsMenus_t Server::getDataFromPage(int n_page) {
    return restaurant->getDataFromPage(n_page);
}

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

