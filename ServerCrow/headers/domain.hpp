#pragma once
#ifndef _DOMAIN_HPP_
#define _DOMAIN_HPP_

#include <string>
#include <vector>
#include <unordered_map>
#include <tuple>


/* ------------------------------------------- EMPLOYEE ------------------------------------------- */
struct Employee
{
	std::string firstName;
	std::string lastName;
	std::string email;
	std::string id;
	std::string mobileNumber;
	int level;
	std::string username;
	std::string password_hash;
	std::string session_token;

	Employee();
	Employee(const std::string& firstName, const std::string& lastName, const std::string& email, const std::string& id, const std::string& mobileNumber, const int& level, const std::string& username, const std::string& password_hash, const std::string& session_token);

	bool isEmpty();
};


/* ------------------------------------------- PRODUCT ------------------------------------------- */
struct Product
{
	std::string name;
	double price;
	std::string color;
	int page;
	int deployable;
	std::string details;

	Product();
	Product(const std::string& name, const double& price, const std::string& color, const int& page, const int& deployable);
	Product(const std::string& name, const double& price, const std::string& color, const int& page, const int& deployable, const std::string& details);

	bool isEmpty();

	bool operator==(const Product& p) const
	{
		return (name == p.name) && (price == p.price) && (details == p.details);
	}
};

struct ProductHashFunction
{
	std::size_t operator()(const Product& p) const
	{
		return std::hash<std::string>()(p.name) ^ (std::hash<double>()(p.price) << 1) ^ (std::hash<std::string>()(p.details));
	}
};

// {Product, int} (int represents the number of products)
using product_unordered_map = std::unordered_map<Product, int, ProductHashFunction>;


/* ------------------------------------------- TABLE ------------------------------------------- */
struct Table
{
	int n_table;
	int n_clients;
	product_unordered_map products;
	double bill;
	double discount;
	double bill_with_discount;
	std::string last_modified;

	Table();
	Table(int n_table);
	Table(int n_table, int n_clients, product_unordered_map products, double discount);

	bool isEmpty();

	bool operator==(const Table& t) const
	{
		return
			(n_table == t.n_table) &&
			(n_clients == t.n_clients) &&
			(products == t.products) &&
			(bill == t.bill) &&
			(discount == t.discount) &&
			(bill_with_discount == t.bill_with_discount);
	}
};


struct TableHashFunction
{
	std::size_t operator()(const Table& t) const
	{
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
struct Order
{
	int id;
	int n_table;
	int n_clients;
	double bill;
	double paid;
	double discount;
	std::string method;
	std::vector<std::pair<Product, int>> products;
	std::string employee;
	std::string date;

	Order();
	Order(const int& id, const int& n_table, const int& n_clients, const double& bill, const double& paid, const double& discount, const std::string& method, const std::vector<std::pair<Product, int>>& products, const std::string& employee, const std::string& date);

	bool operator==(const Order& other) const
	{
		return n_table == other.n_table &&
			n_clients == other.n_clients &&
			bill == other.bill &&
			paid == other.paid &&
			discount == other.discount &&
			method == other.method &&
			products == other.products &&
			employee == other.employee &&
			date == other.date;
	}

	bool isEmpty();
	void copyDataFromTable(const Table& t);
};

struct BillAndPaid
{
	double bill;
	double paid;
	std::string date;

	BillAndPaid();
	BillAndPaid(const double& bill, const double& paid, const std::string& date);
};



/* ------------------------------------------- INGREDIENT ------------------------------------------- */
struct Ingredient
{
	std::string name;

	Ingredient();
	Ingredient(const std::string& _name);

	bool isEmpty();
};



/* ------------------------------------------- ALLERGEN ------------------------------------------- */
struct Allergen
{
	std::string name;

	Allergen();
	Allergen(const std::string& _name);

	bool isEmpty();
};



/* ------------------------------------------- RESTAURANT ------------------------------------------- */
using page_t = std::vector<std::pair<Product, std::vector<Product>>>;

struct Restaurant
{
	// vector[name, price, list of products]. If it is a product, list is empty. Else, if it is a menu, price is 0.
	std::vector<page_t> pages;
	table_unordered_map current_tables;

	Restaurant();
	Restaurant(std::vector<page_t> pages, table_unordered_map current_tables);

	bool isEmpty();

	page_t getDataFromPage(int n_page);

};


#endif