#pragma once
#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include "crow_all.hpp"
#include "interface.hpp"
#include "database.hpp"
#include "databaseManager.hpp"
#include "domain.hpp"
#include "api.hpp"
#include "singletonTemplate.hpp"

#include <sstream>
#include <string>
#include <memory>
#include <functional>
#include <iostream>

class API;
class OrderAPI;
class BillAPI;
class NClientAPI;

class Server : public Interface, public SingletonTemplate<Server>
{
public:
	// Allow Singleton<MyClass> to access the private constructor
	friend class SingletonTemplate<Server>;

	~Server();

	// Getter
	std::shared_ptr<Database> db();


	// Init
	void initialize(Conn& conn) override;
	void dropAllTables() override;
	void setCurrentDatabase(const std::string& databaseName);


	// Update HTML file
	void updateTables(const int& table_id, const std::string& htmlContent);
	void updateHTML();

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
	void getDataFromDatabase(Conn& conn);
	std::pair<int, std::vector<Product>> getProductsAndIds(Conn& conn) override;
	std::vector<page_t> getDataFromPages(Conn& conn) override;
	int getRestaurantPagesSize(Conn& conn);

	std::vector<Table> getTables(Conn& conn) override;
	Table getTableByNumber(Conn& conn, const int n_table) override;
	std::string getLastModifiedFromTable(Conn& conn, const Table& table) override;

	std::vector<Employee> getEmployees(Conn& conn) override;
	Employee getEmployeeByName(Conn& conn, const std::string& fullName) override;
	Employee getEmployeeByAccount(Conn& conn, const std::string& usernname, const std::string& password_hash) override;
	Employee getEmployeeBySessionToken(Conn& conn, const std::string& session_token) override;

	std::vector<Product> getProducts(Conn& conn) override;
	std::unordered_map<int, Product> getProducts2(Conn& conn) override;
	Product getProductByName(Conn& conn, const std::string name) override;
	int getProductIdByName(Conn& conn, const std::string name) override;

	std::vector<Order> getOrders(Conn& conn) override;
	//Order getOrderByTime(const std::string time) override;

	std::vector<Ingredient> getIngredients(Conn& conn) override;
	Ingredient getIngredientByName(Conn& conn, const std::string& name) override;
	std::vector<Ingredient> getIngredientsFromProduct(Conn& conn, const Product& product) override;

	std::vector<Allergen> getAllergens(Conn& conn) override;
	Allergen getAllergenByName(Conn& conn, const std::string name) override;
	std::vector<Allergen> getAllergensFromProduct(Conn& conn, const Product& product) override;


	// Change
	void moveTable(Conn& conn, const int& current_n_table, const int& new_n_table) override;
	void changeTableProductAmount(Conn& conn, const Table& table, const Product& product, const int& new_amount) override;
	void modifyProduct(Conn& conn, const Product& oldProduct, const Product& newProduct) override;
	void changeNumClients(Conn& conn, const Table& table, const int& n_clients);


	// Remove
	void removeTable(Conn& conn, const Table& table) override;
	void removeEmployee(Conn& conn, const Employee& employee) override;
	void removeProduct(Conn& conn, const Product& product) override;
	void removeOrder(Conn& conn, const Order& order) override;
	void removeIngredient(Conn& conn, const Ingredient& ingredient) override;
	void removeAllergen(Conn& conn, const Allergen& allergen) override;

	void removeTableProduct(Conn& conn, const int& n_table, const Product& product, const int& times) override;
	void removeProductIngredient(Conn& conn, const Product& product, const Ingredient& ingredient) override;
	void removeProductIngredients(Conn& conn, const Product& product) override;
	void removeProductAllergens(Conn& conn, const Product& product) override;


	// Various
	void payTable(Conn& conn, const Order& order) override;
	std::string prepareOrdersJSON(const std::vector<Order>& orders);
	std::string hash(const std::string& s);
	std::string generateSessionToken() override;
	crow::json::wvalue retrieveRequest(Conn& conn, std::string& uri);
	void handleSubscription(crow::websocket::connection* conn);
	void handleUnsubscription(crow::websocket::connection* conn);
	void broadcastMessage(const std::string message);


	// Print
	void printTables(Conn& conn) override;
	void printEmployees(Conn& conn) override;
	void printProducts(Conn& conn) override;
	void printOrders(Conn& conn) override;
	void printIngredients(Conn& conn) override;
	void printAllergens(Conn& conn) override;

private:
	Server(); // Must be private for singleton

	DatabaseManager& dbManager; // It´s a ref because it is a singleton
	std::shared_ptr<API> api;

	std::mutex tablesMutex;
	std::unordered_map<int, std::string> tables;

	std::vector<crow::websocket::connection*> webSocketConnections;
	std::mutex webSocketConnectionsMutex;

public:
	std::shared_ptr<Restaurant> restaurant;
};

#endif // _SERVER_HPP_
