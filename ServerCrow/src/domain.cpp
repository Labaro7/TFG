#include "..\headers\domain.h"
#include <iostream>

/* ------------------------------------------- TABLE ------------------------------------------- */
Table::Table() : 
    n_table(0), 
    n_clients(0), 
    products(), 
    bill(0.0), 
    discount(0.0),
    bill_with_discount(0.0) {}

Table::Table(int n_table) : 
    n_table(n_table), 
    n_clients(0), 
    products(), 
    bill(0.0), 
    discount(0.0), 
    bill_with_discount(0.0) {}

Table::Table(int n_table, int n_clients, product_unordered_map products, double discount) :
    n_table(n_table), 
    n_clients(n_clients), 
    products(products), 
    bill(0.0),
    discount(discount),
    bill_with_discount(0.0) {}

bool Table::isEmpty() {
    return n_table == 0 && n_clients == 0 && products.empty() && bill == 0.0 && discount == 0.0 && bill_with_discount == 0.0;
}



/* ------------------------------------------- EMPLOYEE ------------------------------------------- */
Employee::Employee() : 
    name(), 
    level(0), 
    start(), 
    finish() {}

Employee::Employee(const std::string& name, int level, const std::string& start, const std::string& finish) : 
    name(name), 
    level(level), 
    start(start), 
    finish(finish) {}

Employee::Employee(const std::string& name, int level, const std::string& start, const std::string& finish, std::string password_hash, std::string session_token) :
    name(name),
    level(level),
    start(start),
    finish(finish),
    password_hash(password_hash),
    session_token(session_token) {}


bool Employee::isEmpty() {
    return name.empty() && level == 0 && start.empty() && finish.empty();
}



/* ------------------------------------------- PRODUCT ------------------------------------------- */
Product::Product() : 
    name(), 
    price(0.0),
    color("#FFFFFF"),
    page(1),
    deployable(false){}

Product::Product(const std::string& name, double price, std::string color, int page, int deployable) :
    name(name), 
    price(price),
    color(color),
    page(page),
    deployable(deployable){}

bool Product::isEmpty() { return name.empty() && price == 0.0; }

/*Products::Products() :
    products() {}

bool Products::empty() {
    products.clear();
}*/



/* ------------------------------------------- ORDER ------------------------------------------- */
Order::Order() : 
    time(), 
    message() {}

Order::Order(std::string time, std::string message) : 
    message(), 
    time() {}

bool Order::isEmpty() {
    return time.empty() && message.empty();
}



/* ------------------------------------------- INGREDIENT ------------------------------------------- */
Ingredient::Ingredient() : 
    name() {}

Ingredient::Ingredient(const std::string& _name) : 
    name(_name) {}

bool Ingredient::isEmpty() {
    return name.empty();
}



/* ------------------------------------------- ALLERGEN ------------------------------------------- */
Allergen::Allergen() : 
    name() {}

Allergen::Allergen(const std::string& _name) : 
    name(_name) {}

bool Allergen::isEmpty() {
    return name.empty();
}



/* ------------------------------------------- RESTAURANT ------------------------------------------- */
Restaurant::Restaurant() : 
    pages(),
    current_tables() {}

Restaurant::Restaurant(std::vector<productsMenus_t> pages, table_unordered_map current_tables) :  
    pages(pages),
    current_tables(current_tables) {}

bool Restaurant::isEmpty(){ 
    return pages.empty() && current_tables.empty();
}

productsMenus_t Restaurant::getDataFromPage(int n_page) {
    return pages[n_page]; // TODO: Check n_page < pages.size()-1
}