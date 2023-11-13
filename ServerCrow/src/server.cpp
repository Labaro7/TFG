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
    _database->initialize(); 

    // Make the restaurant instances that store different configs of products
    using productsMenus_t = std::vector<std::tuple<Product, std::vector<std::pair<std::string, int>>>>;
    
    Product p1("Bread", 1.0);
    Product p2("Ham", 2.0);
    Product p3("Salad", 3.0);
    Product p4("Pasta", 4.0);
    Product p5("Pizza", 5.0);
    Product p6("Hamburguer", 6.0);
    saveProduct(p1);
    saveProduct(p2);
    saveProduct(p3);
    saveProduct(p4);
    saveProduct(p5);
    saveProduct(p6);

    std::vector<productsMenus_t> pages(5);
    std::vector<std::pair<std::string, int>> vec;
    std::tuple<Product, std::vector<std::pair<std::string, int>>> tup1 = { p1, vec };
    std::tuple<Product, std::vector<std::pair<std::string, int>>> tup2 = { p2, vec };
    std::tuple<Product, std::vector<std::pair<std::string, int>>> tup3 = { p3, vec };
    std::tuple<Product, std::vector<std::pair<std::string, int>>> tup4 = { p4, vec };
    std::tuple<Product, std::vector<std::pair<std::string, int>>> tup5 = { p5, vec };
    std::tuple<Product, std::vector<std::pair<std::string, int>>> tup6 = { p6, vec };

    product_unordered_map ps = { {p1, 1}, {p2, 1} };

    Table t1(69, 1, ps, 0.0);
    saveTable(t1);
    for (auto const& p : ps) {
        saveTableProduct(t1, p.first);
    }

    Table t2(12, 5, ps, 10.0);
    saveTable(t2);
    for (auto const& p : ps) {
        saveTableProduct(t2, p.first);
    }

    pages[0] = { tup1, tup2, tup3, tup4, tup5, tup6 };
    restaurant->pages = pages;

    saveTableProduct(t2, p2);
    saveTableProduct(t2, p2);
}

void Server::dropAllTables() { _database->dropAllTables(); }


// Save
void Server::saveTable(const Table& table) { 
    Table t = getTableByNumber(table.n_table);

    if (t.isEmpty()) { // If this table is not saved
        restaurant->current_tables[table.n_table] = table; // We first store it in server instance
        _database->saveTable(table); // And the in the database
    }
    else {
        CROW_LOG_WARNING << "[EXCEPTION] Table is already in the database.";
    }
}

// TODO: The same as in saveTable but for the rest
void Server::saveEmployee(const Employee& employee) { _database->saveEmployee(employee); }

void Server::saveProduct(const Product& product) { _database->saveProduct(product); }

void Server::saveOrder(const Order& order) { _database->saveOrder(order); }

void Server::saveIngredient(const Ingredient& ingredient) { _database->saveIngredient(ingredient); }

void Server::saveAllergen(const Allergen& allergen) { _database->saveAllergen(allergen); }

void Server::saveTableProduct(Table& table, const Product& product) {
    table.products[product]++;
    table.bill += product.price;
    restaurant->current_tables[table.n_table] = table;

    _database->saveTableProduct(table, product); 
}


// Get
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

