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
    
    Product p1("Drinks", 0.0, "#FEF95D", 1, 0);
    Product p2("Beers", 0.0, "#FEF95D", 1, 0);
    Product p3("Caffeteria", 0.0, "#FEF95D", 1, 0);
    Product p4("White wines", 0.0, "#FEF95D", 1, 0);
    Product p5("Red wines", 0.0, "#FEF95D", 1, 0);
    Product p6("Rosse wines", 0.0, "#FEF95D", 1, 0);
    Product p7("Sparkling wines", 0.0, "#FEF95D", 1, 0);
    saveProduct(p1);
    saveProduct(p2);
    saveProduct(p3);
    saveProduct(p4);
    saveProduct(p5);
    saveProduct(p6);
    saveProduct(p7);

    Product p8("Appetizers", 0.0, "#FEF95D", 2, 0);
    Product p9("Snacks", 0.0, "#FEF95D", 2, 0);
    Product p10("Salads", 0.0, "#FEF95D", 2, 0);
    Product p11("Burguers and Sandwiches", 0.0, "#FEF95D", 2, 0);
    Product p12("Kids Menu", 0.0, "#FEF95D", 2, 0);
    Product p13("Pizzas", 0.0, "#FEF95D", 2, 0);
    Product p14("Desserts", 0.0, "#FEF95D", 2, 0);
    saveProduct(p8);
    saveProduct(p9);
    saveProduct(p10);
    saveProduct(p11);
    saveProduct(p12);
    saveProduct(p13);
    saveProduct(p14);

    Product p15("Cocktails", 0.0, "#FEF95D", 5, 0);
    Product p16("Rum", 0.0, "#FEF95D", 5, 0);
    Product p17("Gin", 0.0, "#FEF95D", 5, 0);
    Product p18("Whisky", 0.0, "#FEF95D", 5, 0);
    Product p19("Vodka", 0.0, "#FEF95D", 5, 0);
    Product p20("Various", 0.0, "#FEF95D", 5, 0);
    Product p21("Juices", 0.0, "#FEF95D", 5, 0);
    saveProduct(p15);
    saveProduct(p16);
    saveProduct(p17);
    saveProduct(p18);
    saveProduct(p19);
    saveProduct(p20);
    saveProduct(p21);

    // Drinks
    Product p22("Water", 1.5, "#FFFFFF", 1, 1);
    Product p23("Cocacola", 2.0, "#FFFFFF", 1, 1);
    Product p24("Fanta", 2.0, "#FFFFFF", 1, 1);
    Product p25("Aquarius", 2.0, "#FFFFFF", 1, 1);
    Product p26("Nestea", 2.0, "#FFFFFF", 1, 1);
    Product p27("Sprite", 2.0, "#FFFFFF", 1, 1);
    saveProduct(p22);
    saveProduct(p23);
    saveProduct(p24);
    saveProduct(p25);
    saveProduct(p26);
    saveProduct(p27);

    // Beers
    Product p28("Guiness", 4.0, "#FFFFFF", 1, 2);
    Product p29("Paulaner", 4.0, "#FFFFFF", 1, 2);
    Product p30("Voll-Damm", 4.5, "#FFFFFF", 1, 2);
    Product p31("Estrella Galicia", 3.0, "#FFFFFF", 1, 2);
    Product p32("Heineken", 3.0, "#FFFFFF", 1, 2);
    saveProduct(p28);
    saveProduct(p29);
    saveProduct(p30);
    saveProduct(p31);
    saveProduct(p32);

    // Caffeteria
    Product p33("Espresso", 1.0, "#FFFFFF", 1, 3);
    Product p34("Latte", 2.0, "#FFFFFF", 1, 3);
    Product p35("Capuccino", 2.0, "#FFFFFF", 1, 3);
    Product p36("American", 2.0, "#FFFFFF", 1, 3);
    Product p37("Irish Coffee", 2.0, "#FFFFFF", 1, 3);
    saveProduct(p33);
    saveProduct(p34);
    saveProduct(p35);
    saveProduct(p36);
    saveProduct(p37);

    std::vector<productsMenus_t> pages(5);
    std::vector<Product> empty_vec;
    std::vector<Product> empty_deployable = { {Product("", 0.0, "#FFFFFF", 1, 0)} };

    // Page 1
    std::vector<Product> drinks = { {p22}, {p23}, {p24}, {p25}, {p26} , {p27} };
    std::vector<Product> beers = { {p28}, {p29}, {p30}, {p31}, {p32} };
    std::vector<Product> coffees = { {p33}, {p34}, {p35}, {p36}, {p37} };
    std::vector<Product> white_wines = empty_deployable;
    std::vector<Product> red_wines = empty_deployable;
    std::vector<Product> rosse_wines = empty_deployable;
    std::vector<Product> sparkling_wines = empty_deployable;

    std::tuple<Product, std::vector<Product>> tup1 = { p1, drinks };
    std::tuple<Product, std::vector<Product>> tup2 = { p2, beers };
    std::tuple<Product, std::vector<Product>> tup3 = { p3, coffees };
    std::tuple<Product, std::vector<Product>> tup4 = { p4, white_wines };
    std::tuple<Product, std::vector<Product>> tup5 = { p5, red_wines };
    std::tuple<Product, std::vector<Product>> tup6 = { p6, rosse_wines };
    std::tuple<Product, std::vector<Product>> tup7 = { p7, sparkling_wines };

    // Page 2
    std::tuple<Product, std::vector<Product>> tup8 = { p8, empty_deployable };
    std::tuple<Product, std::vector<Product>> tup9 = { p9, empty_deployable };
    std::tuple<Product, std::vector<Product>> tup10 = { p10, empty_deployable };
    std::tuple<Product, std::vector<Product>> tup11 = { p11, empty_deployable };
    std::tuple<Product, std::vector<Product>> tup12 = { p12, empty_deployable };
    std::tuple<Product, std::vector<Product>> tup13 = { p13, empty_deployable };
    std::tuple<Product, std::vector<Product>> tup14 = { p14, empty_deployable };

    // Page 3
    std::tuple<Product, std::vector<Product>> tup15 = { p15, empty_deployable };
    std::tuple<Product, std::vector<Product>> tup16 = { p16, empty_deployable };
    std::tuple<Product, std::vector<Product>> tup17 = { p17, empty_deployable };
    std::tuple<Product, std::vector<Product>> tup18 = { p18, empty_deployable };
    std::tuple<Product, std::vector<Product>> tup19 = { p19, empty_deployable };
    std::tuple<Product, std::vector<Product>> tup20 = { p20, empty_deployable };
    std::tuple<Product, std::vector<Product>> tup21 = { p21, empty_deployable };

    product_unordered_map order1 = { {p35, 1}, {p37, 1} };
    Table t1(60, 1, order1, 0.0);
    saveTable(t1);
    for (auto const& p : order1) {
        saveTableProduct(t1, p.first, p.second);
    }

    product_unordered_map order2 = { {p31, 3}, {p26, 1} };
    Table t2(12, 5, order2, 10.0);
    saveTable(t2);
    for (auto const& p : order2) {
        saveTableProduct(t2, p.first, p.second);
    }

    pages[0] = { tup1, tup2, tup3, tup4, tup5, tup6, tup7 };
    pages[1] = { tup8, tup9, tup10, tup11, tup12, tup13, tup14 };
    pages[2] = { tup15, tup16, tup17, tup18, tup19, tup20, tup21 };
    pages[3] = { };
    pages[4] = { };
    restaurant->pages = pages;

    //saveTableProduct(t2, p2, 1);
    //saveTableProduct(t2, p2, 1);
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

int Server::getProductIdByName(const std::string name) const { return _database->getProductIdByName(name); }

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


void Server::payTable(const int& n_table, const std::string& employee, const std::string& date) {
    Order o;
    Table t = getTableByNumber(n_table);

    o.copyDataFromTable(t);
    o.employee = employee;
    o.date = date;
    saveOrder(o);
}