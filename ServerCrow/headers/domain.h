#pragma once
#ifndef _DOMAIN_HPP_
#define _DOMAIN_HPP_

#include <string>
#include <vector>
#include <unordered_map>
#include <tuple>


/* ------------------------------------------- EMPLOYEE ------------------------------------------- */
struct Employee {
    std::string name;
    int level;
    std::string start;
    std::string finish;

    Employee();
    Employee(const std::string& _name, int _level, const std::string& _start, const std::string& _finish);

    bool isEmpty();
};



/* ------------------------------------------- PRODUCT ------------------------------------------- */
struct Product {
    std::string name;
    double price;

    Product();
    Product(const std::string& _name, double _price);

    bool isEmpty();

    bool operator==(const Product& p) const {
        return (name == p.name) && (price == p.price);
    }
};

struct ProductHashFunction {
    std::size_t operator()(const Product& p) const {
        return std::hash<std::string>()(p.name) ^ (std::hash<double>()(p.price) << 1);
    }
};

// [Product -> int (number of products)]
using product_unordered_map = std::unordered_map<Product, int, ProductHashFunction>;


/* ------------------------------------------- TABLE ------------------------------------------- */
struct Table {
    int n_table;
    int n_clients;
    product_unordered_map products;
    double bill;
    double discount;
    double bill_with_discount;

    Table();
    Table(int n_table);
    Table(int n_table, int n_clients, product_unordered_map products, double discount);

    bool isEmpty();

    bool operator==(const Table& t) const {
        return 
            (n_table == t.n_table) && 
            (n_clients == t.n_clients) && 
            (products == t.products) &&
            (bill == t.bill) &&
            (discount == t.discount) &&
            (bill_with_discount == t.bill_with_discount) ;
    }
};

struct TableHashFunction {
    std::size_t operator()(const Table& t) const {
        return
            std::hash<int>()(t.n_table) ^
            (std::hash<int>()(t.n_clients) << 1) ^
            //(std::hash<Product>()(t.products[0]) << 1) ^
            (std::hash<double>()(t.bill) << 1) ^
            (std::hash<double>()(t.discount) << 1) ^
            (std::hash<double>()(t.bill_with_discount) << 1);
    }
};

// [n_table -> Table]
using table_unordered_map = std::unordered_map<int, Table, TableHashFunction>;



/* ------------------------------------------- ORDER ------------------------------------------- */
struct Order {
    std::string time;
    std::string message;

    Order();
    Order(std::string time, std::string message);

    bool isEmpty();
};



/* ------------------------------------------- INGREDIENT ------------------------------------------- */
struct Ingredient {
    std::string name;

    Ingredient();
    Ingredient(const std::string& _name);

    bool isEmpty();
};



/* ------------------------------------------- ALLERGEN ------------------------------------------- */
struct Allergen {
    std::string name;

    Allergen();
    Allergen(const std::string& _name);

    bool isEmpty();
};

#endif



/* ------------------------------------------- RESTAURANT ------------------------------------------- */
using productsMenus_t = std::vector<std::tuple<Product, std::vector<Product>>>;

struct Restaurant {
    // vector[name, price, list of products]. If it is a product, list is empty. Else, if it is a menu, price is 0.
    std::vector<productsMenus_t> pages;
    table_unordered_map current_tables;

    Restaurant();
    Restaurant(std::vector<productsMenus_t> pages, table_unordered_map current_tables);

    bool isEmpty();

    productsMenus_t getDataFromPage(int n_page);

};
