#pragma once
#ifndef _INTERFACE_HPP_
#define _INTERFACE_HPP_

// Crow includes
#include "crow_all.h"

// Custom includes
#include "domain.h"

// Other includes
#include <sstream>
#include <string>
#include <memory>
#include <functional>
#include <iostream>

class Interface
{
public:
	virtual ~Interface() = default;


	// Init
	virtual void initialize() = 0;
	virtual void dropAllTables() = 0;


	// Save
	virtual void saveTable(const Table& table) = 0;
	virtual void saveEmployee(const Employee& oldEmployee, const Employee& newEmployee) = 0;
	virtual void saveProduct(const Product& product) = 0;
	virtual void saveOrder(const Order& order) = 0;
	virtual void saveIngredient(const Ingredient& ingredient) = 0;
	virtual void saveAllergen(const Allergen& allergen) = 0;

	virtual void saveTableProduct(Table& table, const Product& product, const int& amount, const std::string& details) = 0;
	virtual void saveOrderProduct(const Order& order, const int& product_id, const int& amount) = 0;
	virtual void saveProductIngredient(const Product& product, const Ingredient& ingredient) = 0;
	virtual void saveProductAllergen(const Product& product, const Allergen& allergen) = 0;


	// Get
	virtual std::vector<Product> getProductsByDeployableId(const int& deployable_id) = 0;
	virtual std::pair<int, std::vector<Product>> getProductsAndIds() = 0;
	virtual std::vector<page_t> getDataFromPages() = 0;

	virtual std::vector<Table> getTables() = 0;
	virtual Table getTableByNumber(const int n_table) = 0;

	virtual std::vector<Employee> getEmployees() = 0;
	virtual Employee getEmployeeByName(const std::string& firstName, const std::string& lastName) = 0;
	virtual Employee getEmployeeByAccount(const std::string& username, const std::string& password_hash) = 0;
	virtual Employee getEmployeeBySessionToken(const std::string& session_token) = 0;

	virtual std::vector<Product> getProducts() = 0;
	virtual Product getProductByName(const std::string name) = 0;
	virtual int getProductIdByName(const std::string name) = 0;

	virtual std::vector<Order> getOrders() = 0;
	//virtual Order getOrderByTime(const std::string time) = 0;

	virtual std::vector<Ingredient> getIngredients() = 0;
	virtual Ingredient getIngredientByName(const std::string& name) = 0;
	virtual std::vector<Ingredient> getIngredientsFromProduct(const Product& product) = 0;

	virtual std::vector<Allergen> getAllergens() = 0;
	virtual Allergen getAllergenByName(const std::string name) = 0;


	// Change
	virtual void moveTable(const int& current_n_table, const int& new_n_table) = 0;
	virtual void changeTableProductAmount(const Table& table, const Product& product, const int& new_amount) = 0;
	virtual void modifyProduct(const Product& oldProduct, const Product& newProduct) = 0;


	// Remove
	virtual void removeTable(const Table& table) = 0;
	virtual void removeEmployee(const Employee& employee) = 0;
	virtual void removeProduct(const Product& product) = 0;
	virtual void removeIngredient(const Ingredient& ingredient) = 0;
	virtual void removeAllergen(const Allergen& allergen) = 0;
	virtual void removeOrder(const Order& order) = 0;

	virtual void removeTableProduct(const int& n_table, const Product& product, const int& times) = 0;
	virtual void removeProductIngredient(const Product& product, const Ingredient& ingredient) = 0;


	// Various
	virtual std::string generateSessionToken(Employee e) = 0;


	// Print
	virtual void printTables() = 0;
	virtual void printEmployees() = 0;
	virtual void printProducts() = 0;
	virtual void printOrders() = 0;
	virtual void printIngredients() = 0;
	virtual void printAllergens() = 0;
};

#endif // _INTERFACE_HPP_
