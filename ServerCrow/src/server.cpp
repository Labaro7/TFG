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

    Product p1("a", 10);
    Product p2("b", 20);
    Product p3("c", 30);
    saveProduct(p1);
    saveProduct(p2);
    saveProduct(p3);

    std::unordered_map<std::string, int> ps = { {p1.name, p1.price}, {p2.name, p2.price} };

    Table t1(69, 1, ps, 0.0);
    saveTable(t1);
    for (auto const& p : ps) {
        database()->saveTableProduct(t1, Product(p.first, p.second));
    }

    ps[p3.name] = p3.price;
    Table t2(12, 5, ps, 10.0);
    saveTable(t2);
    for (auto const& p : ps) {
        database()->saveTableProduct(t2, Product(p.first, p.second));
    }

    pages[0] = { tup1, tup2, tup3, tup4, tup5, tup6 };
    restaurant->pages = pages;
}

void Server::dropAllTables() { database()->dropAllTables(); }


// Save
void Server::saveTable(const Table& table) { _database->saveTable(table); }

void Server::saveEmployee(const Employee& employee) { _database->saveEmployee(employee); }

void Server::saveProduct(const Product& product) { _database->saveProduct(product); }

void Server::saveOrder(const Order& order) { _database->saveOrder(order); }

void Server::saveIngredient(const Ingredient& ingredient) { _database->saveIngredient(ingredient); }

void Server::saveAllergen(const Allergen& allergen) { _database->saveAllergen(allergen); }


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
void Server::removeTable(const Table& table) { _database->removeTable(table); }

void Server::removeEmployee(const Employee& employee) { _database->removeEmployee(employee); }

void Server::removeProduct(const Product& product) { _database->removeProduct(product); }

void Server::removeOrder(const Order& order) { _database->removeOrder(order); }

void Server::removeIngredient(const Ingredient& ingredient) { _database->removeIngredient(ingredient); }

void Server::removeAllergen(const Allergen& allergen) { _database->removeAllergen(allergen); }

