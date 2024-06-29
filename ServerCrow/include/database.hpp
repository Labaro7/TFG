#pragma once
#ifndef _DATABASE_HPP_
#define _DATABASE_HPP_

// MySQL includes.
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

// Other includes
#include "domain.hpp"
#include "constants.hpp"
#include <sstream>
#include <mutex>
#include <deque>
#include <iomanip>
#include <iostream>
#include <random>
#include "interface.hpp"

class Database : public Interface
{
public:

	Database();
	Database(const Database& database);
	Database(const std::string& hostName,
			 const int& port,
			 const std::string& username,
			 const std::string& password,
			 const std::string& databaseName);
	Database(const std::shared_ptr<Database> database);
	~Database();

	// Database
	void MySqlCreateDatabase(const std::string name);
	void MySqlDropDatabase(const std::string name);
	void MySqlUseDatabase(const std::string name);
	void MySqlSaveChangesToDataBase();
	std::string getName();
	std::unique_ptr<sql::Connection> getConnection();
	std::shared_ptr<sql::ResultSet> executeStmt(Conn& conn, const std::string& query);
	std::unique_ptr<sql::PreparedStatement> prepareStatement(Conn& conn, const std::string& preparedStatement); // Make the sql::prepareStatement be returned as a unique_ptr
	std::shared_ptr<sql::ResultSet> executePstmt(Conn& conn, const std::unique_ptr<sql::PreparedStatement>& pstmt);


	// Tables
	void MySqlCreateTable(const std::string name, std::string definition);
	void MySqlDropTable(const std::string name);
	void MySqlModifyTable(const std::string name, std::string modifications);
	void MySqlEmptyTable(const std::string name);


	// Init
	void initialize(Conn& conn) override;
	void dropAllTables() override;
	void initializeEmployeesTable();
	void initializeProductsTable();
	void initializeIngredientsTable();
	void initializeAllergensTable();
	void initializeOrdersTable();


	// ALL THE METHODS BELOW MUST USE THE MUTEX ATTRIBUTE

	// Save
	void saveTable(Conn& conn, const Table& table) override;
	void saveEmployee(Conn& conn, const Employee& oldEmployee, const Employee& newEmployee) override;
	void saveProduct(Conn& conn, const Product& product) override;
	void saveOrder(Conn& conn, const Order& order) override;
	void saveIngredient(Conn& conn, const Ingredient& ingredient) override;
	void saveAllergen(Conn& conn, const Allergen& allergen) override;

	void saveTableProduct(Conn& conn, Table& table, const Product& product, const int& amount, const std::string& details, const Employee& employee) override;
	void saveOrderProduct(Conn& conn, const Order& order, const int& product_id, const int& amount) override;
	void saveProductIngredient(Conn& conn, const Product& product, const Ingredient& ingredient) override;
	void saveProductAllergen(Conn& conn, const Product& product, const Allergen& allergen) override;
	void saveOrderedProduct(Conn& conn, const OrderedProduct& orderedProduct) override;


	// Get
	std::vector<Product> getProductsByDeployableId(Conn& conn, const int& deployable_id) override;
	std::pair<int, std::vector<Product>> getProductsAndIds(Conn& conn) override;
	std::vector<page_t> getDataFromPages(Conn& conn) override;

	std::vector<Table> getTables(Conn& conn) override;
	Table getTableByNumber(Conn& conn, const int n_table) override;
	std::string getLastModifiedFromTable(Conn& conn, const Table& table) override;

	std::vector<Employee> getEmployees(Conn& conn) override;
	Employee getEmployeeByName(Conn& conn, const std::string& fullName) override;
	Employee getEmployeeByAccount(Conn& conn, const std::string& username, const std::string& password_hash) override;
	Employee getEmployeeBySessionToken(Conn& conn, const std::string& session_token) override;

	std::vector<Product> getProducts(Conn& conn) override;
	std::unordered_map<int, Product> getProducts2(Conn& conn) override;
	Product getProductByName(Conn& conn, const std::string name) override;
	int getProductIdByName(Conn& conn, const std::string name) override;

	std::vector<Order> getOrders(Conn& conn) override;
	Order getOrderById(Conn& conn, const int& id);
	std::vector<Order> getOrdersByDate(Conn& conn, const std::string& date, const std::string& mode);
	std::vector<Order> getOrdersByEmployee(Conn& conn, const std::string& employeeName);
	std::vector<Order> getOrdersByMethod(Conn& conn, const std::string& method);
	std::vector<Order> getOrdersByNTable(Conn& conn, const std::string& n_table);
	std::vector<BillAndPaid> getBillsAndPaids(Conn& conn);
	std::vector<BillAndPaid> getBillsAndPaidsByDate(Conn& conn, const std::string& date, const std::string& mode);
	std::vector<BillAndPaid> getBillsAndPaidsByEmployee(Conn& conn, const std::string& employeeName);
	int getNClients(Conn& conn);
	int getNClientsByDate(Conn& conn, const std::string& date, const std::string& mode);
	int getNClientsByEmployee(Conn& conn, const std::string& employeeName);

	std::unordered_map<int, OrderedProduct> getOrderedProducts(Conn& conn);
	OrderedProduct getOrderedProductById(Conn& conn, const int& id);
	OrderedProduct getOrderedProductByName(Conn& conn, const std::string& name);
	std::unordered_map<int, OrderedProduct> getOrderedProductsByPage(Conn& conn, const int& page);
	std::unordered_map<int, OrderedProduct> getOrderedProductsByMenu(Conn& conn, const std::string& menu);
	std::unordered_map<int, OrderedProduct> getOrderedProductsByPrice(Conn& conn, const double& price);
	std::unordered_map<int, OrderedProduct> getOrderedProductsByDate(Conn& conn, const std::string& date, const std::string& mode);

	std::vector<Ingredient> getIngredients(Conn& conn) override;
	Ingredient getIngredientByName(Conn& conn, const std::string& name) override;
	std::vector<Ingredient> getIngredientsFromProduct(Conn& conn, const Product& product) override;

	std::vector<Allergen> getAllergens(Conn& conn) override;
	Allergen getAllergenByName(Conn& conn, const std::string name) override;
	std::vector<Allergen> getAllergensFromProduct(Conn& conn, const Product& product) override;


	// Change
	void moveTable(Conn& conn, const int& current_n_table, const int& new_n_table)  override;
	void changeTableProductAmount(Conn& conn, const Table& table, const Product& product, const int& new_amount)  override;
	void modifyProduct(Conn& conn, const Product& oldProduct, const Product& newProduct) override;
	void changeNumClients(Conn& conn, const Table& table, const int& n_clients);


	// Remove
	void removeTable(Conn& conn, const Table& table)  override;
	void removeEmployee(Conn& conn, const Employee& employee)  override;
	void removeProduct(Conn& conn, const Product& product)  override;
	void removeOrder(Conn& conn, const Order& order)  override;
	void removeIngredient(Conn& conn, const Ingredient& ingredient)  override;
	void removeAllergen(Conn& conn, const Allergen& allergen)  override;

	void removeTableProduct(Conn& conn, const int& n_table, const Product& product, const int& times)  override;
	void removeProductIngredient(Conn& conn, const Product& product, const Ingredient& ingredient) override;
	void removeProductIngredients(Conn& conn, const Product& product) override;
	void removeProductAllergens(Conn& conn, const Product& product) override;


	// Various
	std::string generateSessionToken() override;
	void payTable(Conn& conn, const Order& order) override;


	// Print
	void printTables(Conn& conn) override;
	void printEmployees(Conn& conn) override;
	void printProducts(Conn& conn) override;
	void printOrders(Conn& conn) override;
	void printIngredients(Conn& conn) override;
	void printAllergens(Conn& conn) override;


private:
	sql::ConnectOptionsMap connection_properties;
	sql::Driver* driver;
	std::string name;

	std::unique_ptr<sql::Connection> con;
	//sql::Statement* stmt;
	//sql::PreparedStatement* pstmt;

	std::shared_mutex mutex;

}; // class Database

#endif