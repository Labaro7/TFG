#include "..\headers\domain.h"

/* ------------------------------------------- TABLE ------------------------------------------- */
Table::Table() : n_table(0), n_clients(0), products(), bill(0.0), discount(0.0) {}

Table::Table(int n_table) : n_table(n_table), n_clients(0), products(), bill(0.0), discount(0.0) {}

Table::Table(int n_table, int n_clients, std::unordered_map<std::string, int> products, double bill, double discount)
    : n_table(n_table), n_clients(n_clients), products(products), bill(bill), discount(discount) {}

bool Table::isEmpty() {
    return n_table == 0 && n_clients == 0 && products.empty() && bill == 0.0 && discount == 0.0;
}



/* ------------------------------------------- EMPLOYEE ------------------------------------------- */
Employee::Employee() : name(), level(0), start(), finish() {}

Employee::Employee(const std::string& _name, int _level, const std::string& _start, const std::string& _finish)
    : name(_name), level(_level), start(_start), finish(_finish) {}

bool Employee::isEmpty() {
    return name.empty() && level == 0 && start.empty() && finish.empty();
}



/* ------------------------------------------- PRODUCT ------------------------------------------- */
Product::Product() : name(), price(0.0) {}

Product::Product(const std::string& _name, double _price) : name(_name), price(_price) {}

bool Product::isEmpty() { return name.empty() && price == 0.0; }


/* ------------------------------------------- ORDER ------------------------------------------- */
Order::Order() : time(), message() {}

Order::Order(std::string time, std::string message) : message(), time() {}

bool Order::isEmpty() {
    return time.empty() && message.empty();
}



/* ------------------------------------------- INGREDIENT ------------------------------------------- */
Ingredient::Ingredient() : name() {}

Ingredient::Ingredient(const std::string& _name) : name(_name) {}

bool Ingredient::isEmpty() {
    return name.empty();
}



/* ------------------------------------------- ALLERGEN ------------------------------------------- */
Allergen::Allergen() : name() {}

Allergen::Allergen(const std::string& _name) : name(_name) {}

bool Allergen::isEmpty() {
    return name.empty();
}



/* ------------------------------------------- RESTAURANT ------------------------------------------- */
using productsMenus_t = std::vector<std::tuple<std::string, int, std::vector<std::pair<std::string, int>>>>;

Restaurant::Restaurant() 
    : n_pages(0), pages() {}

Restaurant::Restaurant(std::vector<productsMenus_t> pages)
    : n_pages(pages.size()), pages(pages) {}

bool Restaurant::isEmpty(){ 
    return n_pages == 0 && pages.empty();
}

auto Restaurant::getPages() {
    return pages;
}

productsMenus_t Restaurant::getDataFromPage(int n_page) {
    return pages[n_page]; // TODO: Check n_page < pages.size()-1
}