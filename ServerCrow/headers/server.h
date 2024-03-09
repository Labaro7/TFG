// Server.hpp
#pragma once
#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include "crow_all.h"
#include "interface.h"
#include "database.h"
#include "domain.h"

#include <sstream>
#include <string>
#include <memory>
#include <functional>
#include <iostream>

class Server : public Interface
{
public:
	Server();
	~Server();


	// Getter
	Database& db();


	// Init
	void initialize() override;
	void dropAllTables() override;


	// Save
	void saveTable(const Table& table) override;
	void saveEmployee(const Employee& oldEmployee, const Employee& newEmployee) override;
	void saveProduct(const Product& product) override;
	void saveOrder(const Order& order) override;
	void saveIngredient(const Ingredient& ingredient) override;
	void saveAllergen(const Allergen& allergen) override;

	void saveTableProduct(Table& table, const Product& product, const int& amount, const std::string& details) override;
	void saveOrderProduct(const Order& order, const int& product_id, const int& amount) override;
	void saveProductIngredient(const Product& product, const Ingredient& ingredient) override;
	void saveProductAllergen(const Product& product, const Allergen& allergen) override;


	// Get
	std::vector<Product> getProductsByDeployableId(const int& deployable_id) override;
	void getDataFromDatabase();
	std::pair<int, std::vector<Product>> getProductsAndIds() override;
	std::vector<page_t> getDataFromPages() override;
	int getRestaurantPagesSize();

	std::vector<Table> getTables() override;
	Table getTableByNumber(const int n_table) override;

	std::vector<Employee> getEmployees() override;
	Employee getEmployeeByName(const std::string& firstName, const std::string& lastName) override;
	Employee getEmployeeByAccount(const std::string& usernname, const std::string& password_hash) override;
	Employee getEmployeeBySessionToken(const std::string& session_token) override;

	std::vector<Product> getProducts() override;
	Product getProductByName(const std::string name) override;
	int getProductIdByName(const std::string name) override;

	std::vector<Order> getOrders() override;
	//Order getOrderByTime(const std::string time) override;

	std::vector<Ingredient> getIngredients() override;
	Ingredient getIngredientByName(const std::string& name) override;
	std::vector<Ingredient> getIngredientsFromProduct(const Product& product) override;

	std::vector<Allergen> getAllergens() override;
	Allergen getAllergenByName(const std::string name) override;
	std::vector<Allergen> getAllergensFromProduct(const Product& product) override;


	// Change
	void moveTable(const int& current_n_table, const int& new_n_table) override;
	void changeTableProductAmount(const Table& table, const Product& product, const int& new_amount) override;
	void modifyProduct(const Product& oldProduct, const Product& newProduct) override;


	// Remove
	void removeTable(const Table& table) override;
	void removeEmployee(const Employee& employee) override;
	void removeProduct(const Product& product) override;
	void removeOrder(const Order& order) override;
	void removeIngredient(const Ingredient& ingredient) override;
	void removeAllergen(const Allergen& allergen) override;

	void removeTableProduct(const int& n_table, const Product& product, const int& times) override;
	void removeProductIngredient(const Product& product, const Ingredient& ingredient) override;


	// Various
	void payTable(const int& n_table, const std::string& employee, const std::string& date);
	std::string prepareOrdersJSON(const std::vector<Order>& orders);
	std::string hash(const std::string& s);
	std::string generateSessionToken() override;


	// Print
	void printTables() override;
	void printEmployees() override;
	void printProducts() override;
	void printOrders() override;
	void printIngredients() override;
	void printAllergens() override;


private:
	std::unique_ptr<Database> database;
public:
	std::unique_ptr<Restaurant> restaurant;
};

#endif // _SERVER_HPP_
