#pragma once
#ifndef _DOMAIN_HPP_
#define _DOMAIN_HPP_

#include <string>
#include <vector>
#include <unordered_map>
#include <tuple>

/* ------------------------------------------- TABLE ------------------------------------------- */
struct Table {
    int n_table;
    int n_clients;
    std::unordered_map<std::string, int> products;
    double bill;
    double discount;

    Table();
    Table(int n_table);
    Table(int n_table, int n_clients, std::unordered_map<std::string, int> products, double bill, double discount);

    bool isEmpty();
};



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
};



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
struct Restaurant {
    // vector[name, price, list of products]. If it is a product, list is empty. Else, if it is a menu, price is 0.
    using productsMenus_t = std::vector<std::tuple<std::string, int, std::vector<std::pair<std::string, int>>>>;
    std::vector<productsMenus_t> pages;

    int n_pages;

    Restaurant();
    Restaurant(std::vector<productsMenus_t> pages);

    bool isEmpty();
    auto getPages();
    productsMenus_t getDataFromPage(int n_page);
};