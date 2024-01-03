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

    // Make the restaurant instances that store different cfigs of products
    using productsMenus_t = std::vector<std::tuple<Product, std::vector<Product>>>;
    
    Product p1("Bread", 1.0, "#FFFFFF", 1, false);
    Product p2("Russian salad", 2.0, "#FFFFFF", 1, false);
    Product p3("Drinks", 0.0, "#FEF95D", 1, true);
    Product p19("White wines", 0.0, "#FEF95D", 1, true);
    Product p20("Cheese", 2.0, "#FFFFFF", 1, false);
    Product p4("French Fries", 4.0, "#FFFFFF", 2, false);
    Product p5("Sweet Potato Fries", 5.0, "#FFFFFF", 2, false);
    Product p6("Tuna Tartar", 6.0, "#FFFFFF", 2, false);
    Product p7("Steak Tartar", 7.0, "#FFFFFF", 3, false);
    Product p8("Goat Cheese and Honey Toast", 8.0, "#FFFFFF", 3, false);
    Product p9("Tomahawk", 9.0, "#FFFFFF", 4, false);
    Product p10("Foie", 10.0, "#FFFFFF", 4, false);
    Product p11("Sandwhich", 11.0, "#FFFFFF", 4, false);
    Product p12("Salmon", 12.0, "#FFFFFF", 4, false);
    saveProduct(p1);
    saveProduct(p2);
    saveProduct(p3);
    saveProduct(p4);
    saveProduct(p5);
    saveProduct(p6);
    saveProduct(p7);
    saveProduct(p8);
    saveProduct(p9);
    saveProduct(p10);
    saveProduct(p11);
    saveProduct(p12);
    saveProduct(p19);
    saveProduct(p20);

    Product p13("Water", 1.0, "#FFFFFF", 1, false);
    Product p14("Cocacola", 2.0, "#FFFFFF", 1, false);
    Product p15("Fanta", 2.0, "#FFFFFF", 1, false);
    Product p16("Aquarius", 2.0, "#FFFFFF", 1, false);
    Product p17("Nestea", 2.0, "#FFFFFF", 1, false);
    Product p18("Sprite", 2.0, "#FFFFFF", 1, false);
    saveProduct(p13);
    saveProduct(p14);
    saveProduct(p15);
    saveProduct(p16);
    saveProduct(p17);
    saveProduct(p18);

    std::vector<productsMenus_t> pages(5);
    std::vector<Product> empty_vec;
    std::vector<Product> drinks = { {p13}, {p14}, {p15}, {p16}, {p17} , {p18} };
    std::vector<Product> white_wines = { {Product("", 0.0, "#FFFFFF", 1, false)}};

    // Page 0
    std::tuple<Product, std::vector<Product>> tup1 = { p1, empty_vec };
    std::tuple<Product, std::vector<Product>> tup2 = { p2, empty_vec };
    std::tuple<Product, std::vector<Product>> tup3 = { p3, drinks };
    std::tuple<Product, std::vector<Product>> tup4 = { p4, empty_vec };
    std::tuple<Product, std::vector<Product>> tup5 = { p5, empty_vec };
    std::tuple<Product, std::vector<Product>> tup6 = { p6, empty_vec };
    std::tuple<Product, std::vector<Product>> tup7 = { p7, empty_vec };
    std::tuple<Product, std::vector<Product>> tup8 = { p8, empty_vec };
    std::tuple<Product, std::vector<Product>> tup9 = { p9, empty_vec };
    std::tuple<Product, std::vector<Product>> tup10 = { p10, empty_vec };
    std::tuple<Product, std::vector<Product>> tup11 = { p11, empty_vec };
    std::tuple<Product, std::vector<Product>> tup12 = { p12, empty_vec };
    std::tuple<Product, std::vector<Product>> tup19 = { p19, white_wines };
    std::tuple<Product, std::vector<Product>> tup20 = { p20, empty_vec };

    product_unordered_map ps = { {p1, 1}, {p2, 1} };

    Table t1(60, 1, ps, 0.0);
    saveTable(t1);
    for (auto const& p : ps) {
        saveTableProduct(t1, p.first, p.second);
    }

    Table t2(12, 5, ps, 10.0);
    saveTable(t2);
    for (auto const& p : ps) {
        saveTableProduct(t2, p.first, p.second);
    }

    pages[0] = { tup1, tup2, tup3, tup19, tup20 };
    pages[1] = { tup4, tup5, tup6 };
    pages[2] = { tup7, tup8, tup9 };
    pages[3] = { tup10, tup11 };
    pages[4] = { tup12 };
    restaurant->pages = pages;

    saveTableProduct(t2, p2, 1);
    saveTableProduct(t2, p2, 1);
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

void Server::saveTableProduct(Table& table, const Product& product, const int& times) {
    table.bill += product.price;
    restaurant->current_tables[table.n_table] = table;

    _database->saveTableProduct(table, product, times); 
}


// Get
productsMenus_t Server::getDataFromPage(int n_page) { return restaurant->getDataFromPage(n_page); }

int Server::getRestaurantPagesSize() const { return restaurant->pages.size(); }

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



// Change
void Server::moveTable(int current_n_table, const int new_n_table) {
    Table current_table = restaurant->current_tables[current_n_table];
    Table new_table = restaurant->current_tables[new_n_table];

    restaurant->current_tables.erase(current_n_table);

    if (new_table.isEmpty()) {
        current_table.n_table = new_n_table;
        restaurant->current_tables[new_n_table] = current_table;
    }
    else {
        for (auto curr_table_product : current_table.products) {
            auto found = new_table.products.find(curr_table_product.first);
            new_table.bill += (curr_table_product.first.price * curr_table_product.second * new_table.discount);

            if (found != new_table.products.end()) {
                found->second += curr_table_product.second;
            }
            else {
                new_table.products[curr_table_product.first] = curr_table_product.second;
            }
        }
    }

    database()->moveTable(current_n_table, new_n_table);
}


// Remove
void Server::removeTable(const Table& table) { _database->removeTable(table); }

void Server::removeEmployee(const Employee& employee) { _database->removeEmployee(employee); }

void Server::removeProduct(const Product& product) { _database->removeProduct(product); }

void Server::removeOrder(const Order& order) { _database->removeOrder(order); }

void Server::removeIngredient(const Ingredient& ingredient) { _database->removeIngredient(ingredient); }

void Server::removeAllergen(const Allergen& allergen) { _database->removeAllergen(allergen); }

