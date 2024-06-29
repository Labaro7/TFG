#pragma once
#ifndef _INTERFACE_HPP_
#define _INTERFACE_HPP_

// Crow includes
#include "crow_all.hpp"

// Custom includes
#include "domain.hpp"

// MySQL includes
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

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

	using Conn = std::unique_ptr<sql::Connection>;

	// Init
	virtual void initialize(Conn& conn) = 0;
	virtual void dropAllTables() = 0;


	// Save
	virtual void saveTable(Conn& conn, const Table& table) = 0;
	virtual void saveEmployee(Conn& conn, const Employee& oldEmployee, const Employee& newEmployee) = 0;
	virtual void saveProduct(Conn& conn, const Product& product) = 0;
	virtual void saveOrder(Conn& conn, const Order& order) = 0;
	virtual void saveIngredient(Conn& conn, const Ingredient& ingredient) = 0;
	virtual void saveAllergen(Conn& conn, const Allergen& allergen) = 0;

	virtual void saveTableProduct(Conn& conn, Table& table, const Product& product, const int& amount, const std::string& details, const Employee& employee) = 0;
	virtual void saveOrderProduct(Conn& conn, const Order& order, const int& product_id, const int& amount) = 0;
	virtual void saveProductIngredient(Conn& conn, const Product& product, const Ingredient& ingredient) = 0;
	virtual void saveProductAllergen(Conn& conn, const Product& product, const Allergen& allergen) = 0;
	virtual void saveOrderedProduct(Conn& conn, const OrderedProduct& orderedProduct) = 0;


	// Get
	virtual std::vector<Product> getProductsByDeployableId(Conn& conn, const int& deployable_id) = 0;
	virtual std::pair<int, std::vector<Product>> getProductsAndIds(Conn& conn) = 0;
	virtual std::vector<page_t> getDataFromPages(Conn& conn) = 0;

	virtual std::vector<Table> getTables(Conn& conn) = 0;
	virtual Table getTableByNumber(Conn& conn, const int n_table) = 0;
	virtual std::string getLastModifiedFromTable(Conn& conn, const Table& table) = 0;

	virtual std::vector<Employee> getEmployees(Conn& conn) = 0;
	virtual Employee getEmployeeByName(Conn& conn, const std::string& fullName) = 0;
	virtual Employee getEmployeeByAccount(Conn& conn, const std::string& username, const std::string& password_hash) = 0;
	virtual Employee getEmployeeBySessionToken(Conn& conn, const std::string& session_token) = 0;

	virtual std::vector<Product> getProducts(Conn& conn) = 0;
	virtual std::unordered_map<int, Product> getProducts2(Conn& conn) = 0;
	virtual Product getProductByName(Conn& conn, const std::string name) = 0;
	virtual int getProductIdByName(Conn& conn, const std::string name) = 0;

	virtual std::vector<Order> getOrders(Conn& conn) = 0;
	//virtual Order getOrderByTime(const std::string time) = 0;

	virtual std::vector<Ingredient> getIngredients(Conn& conn) = 0;
	virtual Ingredient getIngredientByName(Conn& conn, const std::string& name) = 0;
	virtual std::vector<Ingredient> getIngredientsFromProduct(Conn& conn, const Product& product) = 0;

	virtual std::vector<Allergen> getAllergens(Conn& conn) = 0;
	virtual Allergen getAllergenByName(Conn& conn, const std::string name) = 0;
	virtual std::vector<Allergen> getAllergensFromProduct(Conn& conn, const Product& product) = 0;


	// Change
	virtual void moveTable(Conn& conn, const int& current_n_table, const int& new_n_table) = 0;
	virtual void changeTableProductAmount(Conn& conn, const Table& table, const Product& product, const int& new_amount) = 0;
	virtual void modifyProduct(Conn& conn, const Product& oldProduct, const Product& newProduct) = 0;


	// Remove
	virtual void removeTable(Conn& conn, const Table& table) = 0;
	virtual void removeEmployee(Conn& conn, const Employee& employee) = 0;
	virtual void removeProduct(Conn& conn, const Product& product) = 0;
	virtual void removeIngredient(Conn& conn, const Ingredient& ingredient) = 0;
	virtual void removeAllergen(Conn& conn, const Allergen& allergen) = 0;
	virtual void removeOrder(Conn& conn, const Order& order) = 0;

	virtual void removeTableProduct(Conn& conn, const int& n_table, const Product& product, const int& times) = 0;
	virtual void removeProductIngredient(Conn& conn, const Product& product, const Ingredient& ingredient) = 0;
	virtual void removeProductIngredients(Conn& conn, const Product& product) = 0;
	virtual void removeProductAllergens(Conn& conn, const Product& product) = 0;


	// Various
	virtual std::string generateSessionToken() = 0;
	virtual void payTable(Conn& conn, const Order& order) = 0;


	// Print
	virtual void printTables(Conn& conn) = 0;
	virtual void printEmployees(Conn& conn) = 0;
	virtual void printProducts(Conn& conn) = 0;
	virtual void printOrders(Conn& conn) = 0;
	virtual void printIngredients(Conn& conn) = 0;
	virtual void printAllergens(Conn& conn) = 0;
};

#endif // _INTERFACE_HPP_
