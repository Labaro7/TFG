#pragma once
#ifndef _DATABASE_HPP_
#define _DATABASE_HPP_

// MySQL includes.
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

// Other includes
#include "domain.h"
#include "constants.h"
#include <sstream>
#include <mutex>
#include <deque>
#include <iomanip>
#include <iostream>
#include <random>
#include "interface.h"

class Database : public Interface
{
public:
	Database();
	Database(const Database& database);
	~Database();


	// Database
	void MySqlCreateDatabase(const std::string name);
	void MySqlDropDatabase(const std::string name);
	void MySqlUseDatabase(const std::string name);
	void MySqlSaveChangesToDataBase();


	// Tables
	void MySqlCreateTable(const std::string name, std::string definition);
	void MySqlDropTable(const std::string name);
	void MySqlModifyTable(const std::string name, std::string modifications);
	void MySqlEmptyTable(const std::string name);


	// Init
	void initialize() override;
	void dropAllTables() override;
	void initializeEmployeesTable();
	void initializeProductsTable();
	void initializeIngredientsTable();
	void initializeAllergensTable();
	void initializeOrdersTable();


	// ALL THE METHODS BELOW MUST USE THE MUTEX ATTRIBUTE

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
	std::pair<int, std::vector<Product>> getProductsAndIds() override;
	std::vector<page_t> getDataFromPages() override;

	std::vector<Table> getTables() override;
	Table getTableByNumber(const int n_table) override;

	std::vector<Employee> getEmployees() override;
	Employee getEmployeeByName(const std::string& firstName, const std::string& lastName) override;
	Employee getEmployeeByAccount(const std::string& username, const std::string& password_hash) override;
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
	void moveTable(const int& current_n_table, const int& new_n_table)  override;
	void changeTableProductAmount(const Table& table, const Product& product, const int& new_amount)  override;
	void modifyProduct(const Product& oldProduct, const Product& newProduct) override;


	// Remove
	void removeTable(const Table& table)  override;
	void removeEmployee(const Employee& employee)  override;
	void removeProduct(const Product& product)  override;
	void removeOrder(const Order& order)  override;
	void removeIngredient(const Ingredient& ingredient)  override;
	void removeAllergen(const Allergen& allergen)  override;

	void removeTableProduct(const int& n_table, const Product& product, const int& times)  override;
	void removeProductIngredient(const Product& product, const Ingredient& ingredient) override;


	// Various
	std::string generateSessionToken() override;


	// Print
	void printTables() override;
	void printEmployees() override;
	void printProducts() override;
	void printOrders() override;
	void printIngredients() override;
	void printAllergens() override;


private:
	sql::ConnectOptionsMap connection_properties;
	sql::Driver* driver;

	std::shared_ptr<sql::Connection> con;
	sql::Statement* stmt;
	sql::PreparedStatement* pstmt;

	std::mutex mutex;

}; // class Database

#endif