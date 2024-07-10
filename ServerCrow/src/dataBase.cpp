#include "..\include\database.hpp"
#include "..\include\crow_all.hpp"
#include "..\include\domain.hpp"
#include <unordered_map>

Database::Database() : /*pstmt(),*/ name(cts::DATABASE_NAME)
{
	try
	{
		connection_properties = {
			{"hostName", cts::HOST},
			{"port", cts::PORT},
			{"userName", cts::USERNAME},
			{"password", cts::PASSWORD},
			{"schema", cts::DATABASE_NAME},
			{"OPT_RECONNECT", true}
		};

		driver = get_driver_instance();
		con = static_cast<std::unique_ptr<sql::Connection>>(driver->connect(connection_properties));
		//stmt = con->createStatement();
	}
	catch (const sql::SQLException e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not construct database." << e.what();
		exit(1);
	}
}

Database::Database(const Database& database)
{
	name = database.name;

	connection_properties = database.connection_properties;

	driver = database.driver;

	con = static_cast<std::unique_ptr<sql::Connection>>(driver->connect(connection_properties));

	//stmt = database.stmt;

	//pstmt = database.pstmt;
}

Database::Database(const std::string& hostName,
				   const int& port,
				   const std::string& username,
				   const std::string& password,
				   const std::string& databaseName) : name(databaseName)
{
	try
	{
		connection_properties = {
			{"hostName", hostName},
			{"port", port},
			{"userName", username},
			{"password", password},
			{"schema", databaseName},
			{"OPT_RECONNECT", true}
		};

		driver = get_driver_instance();
		con = static_cast<std::unique_ptr<sql::Connection>>(driver->connect(connection_properties));
		//stmt = con->createStatement();
	}
	catch (const sql::SQLException e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not construct custom database." << e.what();
		exit(1);
	}
}

Database::Database(const std::shared_ptr<Database> database)
{
	connection_properties = database->connection_properties;

	driver = database->driver;

	con = static_cast<std::unique_ptr<sql::Connection>>(driver->connect(connection_properties));

	//stmt = database->stmt;

	//pstmt = database->pstmt;

}

Database::~Database()
{
	if (con)
	{
		con->close();
	}

	// These have to be explicitly deleted. con is shared_ptr so it doesnt need to
	//delete stmt;
	//delete pstmt;
}



// ------------------------------- MySQL queries ------------------------------- //

// Database
void Database::MySqlCreateDatabase(std::string name)
{
	try
	{
		sql::Statement* stmt = con->createStatement();
		stmt->execute("CREATE DATABASE " + name);
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not create database. Error message: " << e.what();
	}
}

void Database::MySqlDropDatabase(std::string name)
{
	try
	{
		sql::Statement* stmt = con->createStatement();
		stmt->execute("DROP DATABASE " + name);
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not drop database. Error message: " << e.what();
	}
}

void Database::MySqlUseDatabase(std::string name)
{
	try
	{
		sql::Statement* stmt = con->createStatement();
		stmt->execute("USE " + name);
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not use database. Error message: " << e.what();
	}
}

void Database::MySqlSaveChangesToDataBase()
{
	try
	{
		sql::Statement* stmt = con->createStatement();
		stmt->execute("COMMIT");
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not save changes to the database. Error message: " << e.what();
	}
}

// Table
void Database::MySqlCreateTable(std::string name,
								std::string definition)
{
	try
	{
		sql::Statement* stmt = con->createStatement();
		stmt->execute("CREATE TABLE " + name + " (" + definition + ")");
		CROW_LOG_INFO << "MySQL table created: " << name;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not create table. Error message: " << e.what();
	}
}

void Database::MySqlDropTable(std::string name)
{
	try
	{
		sql::Statement* stmt = con->createStatement();
		stmt->execute("DROP TABLE IF EXISTS " + name);
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not drop table. Error message: " << e.what();
	}
}

void Database::MySqlModifyTable(std::string name,
								std::string modification)
{
	try
	{
		sql::Statement* stmt = con->createStatement();
		stmt->execute("ALTER TABLE " + name + " " + modification);
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not modify table. Error message: " << e.what();
	}
}

void Database::MySqlEmptyTable(std::string name)
{
	try
	{
		sql::Statement* stmt = con->createStatement();
		stmt->execute("DELETE FROM " + name);
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not empty table. Error message: " << e.what();
	}
}

std::string Database::getName()
{
	return this->name;
}

std::unique_ptr<sql::Connection> Database::getConnection()
{
	return static_cast<std::unique_ptr<sql::Connection>>(driver->connect(connection_properties));;
}

std::shared_ptr<sql::ResultSet> Database::executeStmt(Conn& conn, const std::string& query)
{
	std::unique_ptr<sql::Statement> stmt(conn->createStatement());

	return std::shared_ptr<sql::ResultSet>(stmt->executeQuery(query));
}

std::unique_ptr<sql::PreparedStatement> Database::prepareStatement(Conn& conn, const std::string& preparedStatement)
{
	return std::unique_ptr<sql::PreparedStatement>(conn->prepareStatement(preparedStatement));
}

std::shared_ptr<sql::ResultSet> Database::executePstmt(Conn& conn, const std::unique_ptr<sql::PreparedStatement>& pstmt)
{
	return std::shared_ptr<sql::ResultSet>(pstmt->executeQuery());
}


// ------------------------------- /MySQL queries ------------------------------- //


void Database::initializeEmployeesTable()
{
	/*std::vector<std::string> firstNames = {"Herrera", "Escribano", "Peinado", "Trillo", "Herrera", "Herrera", "Zapata"};
	std::vector<int> levels = { 3, 2, 2, 4, 4, 2, 1 };
	std::vector<std::string> starts = { "2023 - 10 - 04 15:30 : 45" };
	std::vector<std::string> finishes = { "2023-10-04 15:30:45" };

	for (int i = 0; i < names.size(); i++)
	{
		Employee e = { names[i], levels[i], starts[0], finishes[0] };
		saveEmployee(e);
	}*/
}

void Database::initializeProductsTable()
{
	/*std::vector<std::string> productNames = {"DRINKS", "BEERS", "CAFETERIA",
	"WHITE WINES", "RED WINES", "ROSSE WINES",
	"SPARKLING WINES", "COCKTAILS", "RUM",
	"GIN", "WHISKY", "VODKA" };

	std::vector<double> prices = { 10.99, 15.99, 8.50,
	10.99, 15.99, 8.50,
	10.99, 15.99, 8.50,
	10.99, 15.99, 8.50 };

	for (size_t i = 0; i < productNames.size(); i++) {
		Product product = { productNames[i], prices[i], "#FFFFFF", 1, false};
		saveProduct(product);
	}*/
}

void Database::initializeOrdersTable()
{
	/*std::vector<std::string> times = { "2023-10-04 15:45:00", "2023-10-05 12:30:00", "2023-10-06 18:15:00" };
	std::vector<std::string> messages = { "Order 1", "Order 2", "Order 3" };

	for (size_t i = 0; i < times.size(); i++) {
		Order order = { times[i], messages[0] };

		saveOrder(order);
	}*/
}

void Database::initializeIngredientsTable()
{
	std::vector<std::string> ingredientNames = { "Salt", "Pepper", "Sugar" };

	for (const auto& name : ingredientNames)
	{
		const Ingredient ingredient = { name };
		saveIngredient(con, ingredient);
	}
}

void Database::initializeAllergensTable()
{
	std::vector<std::string> allergenNames = { "Gluten", "Peanuts", "Dairy" };

	for (const auto& name : allergenNames)
	{
		const Allergen allergen = { name };
		saveAllergen(con, allergen);
	}
}

void Database::initialize(Conn& conn)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	// Create the tables to define the domain
	// TODO: Make n_table primary key so there are no duplicate tables
	MySqlCreateTable("tables", "table_id INT AUTO_INCREMENT PRIMARY KEY, n_table INT, n_clients INT, bill DOUBLE, discount DOUBLE, last_modified VARCHAR(45)");
	MySqlCreateTable("employees", "employee_id INT AUTO_INCREMENT PRIMARY KEY, firstName VARCHAR(45), lastName VARCHAR(45), email VARCHAR(45), id VARCHAR(45), mobileNumber VARCHAR(45), level INT, username VARCHAR(45), password VARCHAR(45), session_token VARCHAR(45)");
	MySqlCreateTable("products", "product_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45), price DOUBLE, color VARCHAR(45), page INT, deployable BOOLEAN");
	MySqlCreateTable("orders", "order_id INT AUTO_INCREMENT PRIMARY KEY, n_table INT, n_clients INT, bill DOUBLE, paid DOUBLE, discount DOUBLE, method VARCHAR(45), employee VARCHAR(45), date VARCHAR(45)");
	MySqlCreateTable("ingredients", "ingredient_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45)");
	MySqlCreateTable("allergens", "allergen_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45)");
	MySqlCreateTable("restaurants", "restaurant_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45)");

	// TODO: Create Junction tables
	// Create the junction tables to the ManyToMany relationships
	MySqlCreateTable("tableproduct", "table_id INT, product_id INT, amount INT, details VARCHAR(45), PRIMARY KEY(table_id, product_id, details), FOREIGN KEY(table_id) REFERENCES tables(table_id), FOREIGN KEY(product_id) REFERENCES products(product_id)");
	MySqlCreateTable("orderproduct", "order_id INT, product_id INT, amount INT, PRIMARY KEY(order_id, product_id)");
	MySqlCreateTable("productingredient", "product_id INT, ingredient_id INT, PRIMARY KEY(product_id, ingredient_id), FOREIGN KEY(product_id) REFERENCES products(product_id), FOREIGN KEY(ingredient_id) REFERENCES ingredients(ingredient_id)");
	MySqlCreateTable("ingredientallergen", "product_id INT, ingredient_id INT, PRIMARY KEY(product_id, ingredient_id), FOREIGN KEY(product_id) REFERENCES products(product_id), FOREIGN KEY(ingredient_id) REFERENCES ingredients(ingredient_id)");
	MySqlCreateTable("productallergen", "product_id INT, allergen_id INT, PRIMARY KEY(product_id, allergen_id), FOREIGN KEY(product_id) REFERENCES products(product_id), FOREIGN KEY(allergen_id) REFERENCES allergens(allergen_id)");

	// Populate the tables
	initializeEmployeesTable();
	initializeProductsTable();
	initializeIngredientsTable();
	initializeAllergensTable();
	initializeOrdersTable();
}

void Database::dropAllTables()
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	try
	{
		//std::unique_lock<std::shared_mutex> lock(mutex);

		// We must drop first the tables that have a reference to another
		executeStmt(con, "DROP TABLE IF EXISTS tableproduct;");
		executeStmt(con, "DROP TABLE IF EXISTS orderproduct;");
		executeStmt(con, "DROP TABLE IF EXISTS productingredient;");
		executeStmt(con, "DROP TABLE IF EXISTS productorder;");

		executeStmt(con, "DROP TABLE IF EXISTS orders;");
		executeStmt(con, "DROP TABLE IF EXISTS ingredients;");
		executeStmt(con, "DROP TABLE IF EXISTS allergens;");
		executeStmt(con, "DROP TABLE IF EXISTS employees;");
		executeStmt(con, "DROP TABLE IF EXISTS products;");
		executeStmt(con, "DROP TABLE IF EXISTS tables;");
		executeStmt(con, "DROP TABLE IF EXISTS restaurants;");
		executeStmt(con, "DROP TABLE IF EXISTS orderedproducts;");

		CROW_LOG_INFO << "[REMOVED] All tables dropped.";
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not drop tables. Error message: " << e.what();
	}
}


// Save
void Database::saveTable(Conn& conn, const Table& table)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	try
	{
		//std::unique_lock<std::shared_mutex> lock(mutex, std::adopt_lock);

		auto pstmt = prepareStatement(conn, "INSERT INTO tables(n_table, n_clients, bill, discount) VALUES(?,?,?,?)");
		pstmt->setInt(1, table.n_table);
		pstmt->setInt(2, table.n_clients);
		pstmt->setDouble(3, table.bill);
		pstmt->setDouble(4, table.discount);

		executePstmt(conn, pstmt);

		CROW_LOG_INFO << "[ADDED] Table " << table.n_table <<
			" with " << table.n_clients <<
			" clients inserted into tables.";
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not save table. Error message: " << e.what();
	}
}

void Database::saveEmployee(Conn& conn, const Employee& oldEmployee, const Employee& newEmployee)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	try
	{
		//std::unique_lock<std::shared_mutex> lock(mutex);

		const std::string oldEmployeeFullName = oldEmployee.firstName + " " + oldEmployee.lastName;
		const std::string newEmployeeFullName = oldEmployee.firstName + " " + oldEmployee.lastName;

		if (getEmployeeByName(conn, oldEmployeeFullName).isEmpty() && getEmployeeByName(conn, newEmployeeFullName).isEmpty())
		{
			auto pstmt = prepareStatement(conn, "INSERT INTO employees(firstName, lastName, email, id, mobileNumber, level, username, password, session_token) VALUES(?,?,?,?,?,?,?,?,?)");
			pstmt->setString(1, newEmployee.firstName);
			pstmt->setString(2, newEmployee.lastName);
			pstmt->setString(3, newEmployee.email);
			pstmt->setString(4, newEmployee.id);
			pstmt->setString(5, newEmployee.mobileNumber);
			pstmt->setInt(6, newEmployee.level);
			pstmt->setString(7, newEmployee.username);
			pstmt->setString(8, newEmployee.password_hash);
			pstmt->setString(9, generateSessionToken());

			executePstmt(conn, pstmt);

			CROW_LOG_INFO << "[ADDED] Employee " << newEmployee.firstName << " " << newEmployee.lastName <<
				" with level " << newEmployee.level;
		}
		else if (!getEmployeeByName(conn, oldEmployeeFullName).isEmpty())
		{
			auto pstmt = prepareStatement(conn, "UPDATE employees SET firstName = ?, lastName = ?, email = ?, id = ?, mobileNumber = ?, level = ?, username = ?, password = ?, session_token = ? WHERE firstName = ? AND lastName = ? AND level = ?");
			pstmt->setString(1, newEmployee.firstName);
			pstmt->setString(2, newEmployee.lastName);
			pstmt->setString(3, newEmployee.email);
			pstmt->setString(4, newEmployee.id);
			pstmt->setString(5, newEmployee.mobileNumber);
			pstmt->setInt(6, newEmployee.level);
			pstmt->setString(7, newEmployee.username);
			pstmt->setString(8, newEmployee.password_hash);
			pstmt->setString(9, generateSessionToken());
			pstmt->setString(10, oldEmployee.firstName);
			pstmt->setString(11, oldEmployee.lastName);
			pstmt->setInt(12, oldEmployee.level);

			executePstmt(conn, pstmt);

			CROW_LOG_INFO << "[UPDDATED] Employee " << oldEmployee.firstName << " " << oldEmployee.lastName <<
				" with level " << oldEmployee.level <<
				" to employee " << newEmployee.firstName << " " << newEmployee.lastName <<
				" with level " << newEmployee.level;
		}
		else
		{
			CROW_LOG_WARNING << "No employee was added.";
		}
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not save employee. Error message: " << e.what();
	}
}

void Database::saveProduct(Conn& conn, const Product& product)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	try
	{
		//std::unique_lock<std::shared_mutex> lock(mutex);

		const std::string name = product.name;
		const double price = product.price;
		const std::string color = product.color;
		const int page = product.page;
		const int deployable = product.deployable;

		if (getProductByName(conn, name).isEmpty())
		{
			auto pstmt = prepareStatement(conn, "INSERT INTO products(name, price, color, page, deployable) VALUES(?,?,?,?,?)");
			pstmt->setString(1, name);
			pstmt->setDouble(2, price);
			pstmt->setString(3, color);
			pstmt->setInt(4, page);
			pstmt->setInt(5, deployable);

			executePstmt(conn, pstmt);

			CROW_LOG_INFO << "[ADDED] Product " << name <<
				" with price " << price <<
				" inserted into products.";
		}
		else
		{
			CROW_LOG_WARNING << "[EXCEPTION] Product is already in the database.";
		}
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not save product. Error message: " << e.what();
	}
}

void Database::saveOrder(Conn& conn, const Order& order)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	try
	{
		//std::unique_lock<std::shared_mutex> lock(mutex, std::adopt_lock);

		auto pstmt = prepareStatement(conn, "INSERT INTO orders(n_table, n_clients, bill, paid, discount, method, employee, date) VALUES(?,?,?,?,?,?,?,?)");
		pstmt->setInt(1, order.n_table);
		pstmt->setInt(2, order.n_clients);
		pstmt->setDouble(3, order.bill);
		pstmt->setDouble(4, order.paid);
		pstmt->setDouble(5, order.discount);
		pstmt->setString(6, order.method);
		pstmt->setString(7, order.employee);
		pstmt->setString(8, order.date);

		executePstmt(conn, pstmt);

		std::stringstream ss;
		ss << "SELECT table_id FROM tables WHERE n_table = '" << order.n_table << "'";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		if (res->next())
		{
			const int table_id = res->getInt("table_id");

			ss << "SELECT * FROM tableproduct WHERE table_id = '" << table_id << "'";
			res = executeStmt(conn, ss.str());
			ss.str("");

			while (res->next())
			{
				int product_id = res->getInt("product_id");
				int amount = res->getInt("amount");

				saveOrderProduct(conn, order, product_id, amount);
			}

			CROW_LOG_INFO << "[ADDED] Order with n_table " << order.n_table <<
				" with bill " << order.bill <<
				" with paid " << order.paid <<
				" with discount " << order.discount <<
				" with method " << order.method <<
				" by employee " << order.employee <<
				" at date " << order.date;
		}
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not save order. Error message: " << e.what();
	}
}

// Do not use the mutex here!. This function is called from another one that uses it.
void Database::saveOrderProduct(Conn& conn, const Order& order, const int& product_id, const int& amount)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	try
	{
		//std::unique_lock<std::shared_mutex> lock(mutex, std::adopt_lock);

		std::stringstream ss;
		ss << "SELECT order_id FROM orders WHERE n_table = '" << order.n_table << "' AND n_clients = '" << order.n_clients << "' AND bill = '" << order.bill << "' AND discount = '" << order.discount << "' AND employee = '" << order.employee << "' AND date = '" << order.date << "'";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		if (res->next())
		{
			const int order_id = res->getInt("order_id");

			auto pstmt = prepareStatement(conn, "INSERT INTO orderproduct(order_id, product_id, amount) VALUES(?,?,?)");
			pstmt->setInt(1, order_id);
			pstmt->setInt(2, product_id);
			pstmt->setInt(3, amount);
			pstmt->execute();
		}
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not save order/product. Error message: " << e.what();
	}
}

void Database::saveIngredient(Conn& conn, const Ingredient& ingredient)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	try
	{
		//std::unique_lock<std::shared_mutex> lock(mutex);

		if (getIngredientByName(conn, ingredient.name).isEmpty())
		{
			auto pstmt = prepareStatement(conn, "INSERT INTO ingredients(name) VALUES(?)");
			pstmt->setString(1, ingredient.name);
			pstmt->execute();

			CROW_LOG_INFO << "[ADDED] Ingredient with name " << ingredient.name <<
				" inserted into ingredients.";
		}
		else
		{
			CROW_LOG_WARNING << "[EXCEPTION] Ingredient is already in the database.";
		}
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not save ingredient. Error message: " << e.what();
	}
}

void Database::saveAllergen(Conn& conn, const Allergen& allergen)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	try
	{
		//std::unique_lock<std::shared_mutex> lock(mutex);

		if (getAllergenByName(conn, allergen.name).isEmpty())
		{
			auto pstmt = prepareStatement(conn, "INSERT INTO allergens(name) VALUES(?)");
			pstmt->setString(1, allergen.name);
			pstmt->execute();

			CROW_LOG_INFO << "[ADDED] Allergen with name " << allergen.name;
		}
		else
		{
			CROW_LOG_WARNING << "[EXCEPTION] Allergen is already in the database.";
		}
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not save allergen. Error message: " << e.what();
	}
}

std::string getLastNameInitials(const std::string lastName)
{
	std::string initials = "";

	if (!lastName.empty())
	{
		initials += toupper(lastName[0]);
		initials += ". ";

		for (int i = 0; i < lastName.length(); i++)
		{
			if (lastName[i] == ' ')
			{
				if (i < lastName.length() - 1 && lastName[i + 1] != ' ')
				{
					initials += toupper(lastName[i + 1]);
					initials += ". ";
				}
			}
		}
	}

	return initials;
}

// Do not use the mutex here!. This function is called from another one that uses it.
void Database::saveTableProduct(Conn& conn, 
								Table& table,
								const Product& product,
								const int& amount,
								const std::string& details,
								const Employee& employee)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		int table_id = 0;
		int product_id = 0;

		auto pstmt1 = prepareStatement(conn, "SELECT * FROM tables WHERE n_table = ?");
		pstmt1->setInt(1, table.n_table);

		auto res = executePstmt(conn, pstmt1);

		if (res->next())
		{
			table_id = res->getInt("table_id");
			const double new_bill = res->getDouble("bill") + (product.price * amount);
			const std::string employeeName = employee.firstName + " " + getLastNameInitials(employee.lastName);

			auto pstmt2 = prepareStatement(conn, "UPDATE tables SET bill = ?, last_modified = ? WHERE table_id = ?");
			pstmt2->setDouble(1, new_bill);
			pstmt2->setString(2, employeeName);
			pstmt2->setInt(3, table_id);

			executePstmt(conn, pstmt2);

			CROW_LOG_INFO << "[UPDATED] Table " << table.n_table <<
				" updated bill is " << new_bill;

			auto pstmt3 = prepareStatement(conn, "SELECT product_id FROM products WHERE name = ? ");
			pstmt3->setString(1, product.name);

			res = executePstmt(conn, pstmt3);

			if (res->next())
			{
				product_id = res->getInt("product_id");

				auto pstmt4 = prepareStatement(conn, "SELECT amount FROM tableproduct WHERE table_id = ? AND product_id = ? AND details = ?");
				pstmt4->setInt(1, table_id);
				pstmt4->setInt(2, product_id);
				pstmt4->setString(3, details);
				
				res = executePstmt(conn, pstmt4);

				if (res->next())
				{
					const int new_amount = res->getInt("amount") + amount;

					auto pstmt5 = prepareStatement(conn, "UPDATE tableproduct SET amount = ? WHERE table_id = ? AND product_id = ? AND details = ?");
					pstmt5->setInt(1, new_amount);
					pstmt5->setInt(2, table_id);
					pstmt5->setInt(3, product_id);
					pstmt5->setString(4, details);

					executePstmt(conn, pstmt5);

					CROW_LOG_INFO << "[UPDATED] Tableproduct with table_id " << table_id <<
						" and product_id " << product_id <<
						"and details " << details <<
						" to amount " << new_amount;
				}
				else
				{
					auto pstmt6 = prepareStatement(conn, "INSERT INTO tableproduct(table_id, product_id, amount, details) VALUES(?,?,?,?)");
					pstmt6->setInt(1, table_id);
					pstmt6->setInt(2, product_id);
					pstmt6->setInt(3, amount);
					pstmt6->setString(4, details);

					executePstmt(conn, pstmt6);

					CROW_LOG_INFO << "[ADDED] Tableproduct with table_id " << table_id <<
						" and product_id " << product_id << " and amount " << amount << " and details " << details;
				}
			}
		}
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not save table/product. Error message: " << e.what();
	}
}

// It resets the productingredient table. This is more "set" than "save"
void Database::saveProductIngredient(Conn& conn, 
									 const Product& product,
									 const Ingredient& ingredient)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;

		ss << "SELECT product_id FROM products WHERE name = '" << product.name << "' AND price = '" << product.price << "'";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		if (res->next())
		{
			const int product_id = res->getInt("product_id");

			ss << "SELECT ingredient_id FROM ingredients WHERE name = '" << ingredient.name << "'";
			res = executeStmt(conn, ss.str());
			ss.str("");

			if (res->next())
			{
				const int ingredient_id = res->getInt("ingredient_id");

				auto pstmt = prepareStatement(conn, "INSERT INTO productingredient(product_id, ingredient_id) VALUES(?,?)");
				pstmt->setInt(1, product_id);
				pstmt->setInt(2, ingredient_id);
				pstmt->execute();

				CROW_LOG_INFO << "[ADDED] Productingredient with product_id " << product_id <<
					" and ingredient_id " << ingredient_id;
			}
		}
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not save product/ingredient. Error message: " << e.what();
	}
}

void Database::saveProductAllergen(Conn& conn, 
								   const Product& product,
								   const Allergen& allergen)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;

		ss << "SELECT product_id FROM products WHERE name = '" << product.name << "' AND price = '" << product.price << "'";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		if (res->next())
		{
			const int product_id = res->getInt("product_id");

			ss << "SELECT allergen_id FROM allergens WHERE name = '" << allergen.name << "'";
			res = executeStmt(conn, ss.str());
			ss.str("");

			if (res->next())
			{
				const int allergen_id = res->getInt("allergen_id");

				auto pstmt = prepareStatement(conn, "INSERT INTO productallergen(product_id, allergen_id) VALUES(?,?)");
				pstmt->setInt(1, product_id);
				pstmt->setInt(2, allergen_id);
				pstmt->execute();

				CROW_LOG_INFO << "[ADDED] Productingredient with product_id " << product_id <<
					" and allergen_id " << allergen_id;
			}
		}
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not save product/allergen. Error message: " << e.what();
	}
}

void Database::saveOrderedProduct(Conn& conn, 
								  const OrderedProduct& orderedProduct)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	try
	{
		//////std::unique_lock<std::shared_mutex> lock(mutex);

		if (true/*getOrderedProductByName(orderedProduct.name).isEmpty()*/)
		{
			auto pstmt = prepareStatement(conn, "INSERT INTO orderedproducts(name, price, percent, revenue, totalRevenue) VALUES(?, ?, ?, ?, ?)");
			pstmt->setString(1, orderedProduct.name);
			pstmt->setDouble(2, orderedProduct.price);
			pstmt->setDouble(3, orderedProduct.percent);
			pstmt->setDouble(4, orderedProduct.revenue);
			pstmt->setDouble(5, orderedProduct.totalRevenue);
			pstmt->execute();

			CROW_LOG_INFO << "[ADDED] OrderedProduct with name " << orderedProduct.name;
		}
		else
		{
			CROW_LOG_WARNING << "[EXCEPTION] Allergen is already in the database.";
		}
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not save orderedProduct. Error message: " << e.what();
	}
}


//Get
std::vector<Table> Database::getTables(Conn& conn)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	std::vector<Table> tables;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;

		ss << "SELECT * FROM tables";
		auto res = executeStmt(conn, ss.str());
		ss.str("");
		
		while (res->next())
		{
			const int table_id = res->getInt("table_id");
			const int n_table = res->getInt("n_table");
			const int n_clients = res->getInt("n_clients");
			const double bill = res->getDouble("bill");
			const double discount = res->getDouble("discount");

			domain::product_unordered_map products;
			ss << "SELECT * FROM tableproduct WHERE table_id = " << table_id;
			auto res2 = executeStmt(conn, ss.str());
			ss.str("");

			while (res2->next())
			{
				const int product_id = res2->getInt("product_id");
				const int amount = res2->getInt("amount");

				ss << "SELECT * FROM products WHERE product_id = " << product_id;
				auto res3 = executeStmt(conn, ss.str());
				ss.str("");

				while (res3->next())
				{

					const std::string name = res3->getString("name");
					const double price = res3->getDouble("price");
					const std::string color = res3->getString("color");
					const int page = res3->getInt("page");
					const int deployable = res3->getInt("deployable");

					const Product product = {name, price, color, page, deployable};
					products[product] = amount;
				}
			}

			const Table table = { n_table, n_clients, products, bill, discount };

			tables.push_back(table);
		}

		// Sort the tables in ascending order
		std::sort(tables.begin(), tables.end(), [](Table a, Table b)
				  {
					  return a.n_table < b.n_table;
				  });

		return tables;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get tables. Error message: " << e.what();
		return tables;
	}
}

Table Database::getTableByNumber(Conn& conn, 
								 const int n_table)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	Table table;

	try
	{
		std::shared_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;

		ss << "SELECT * FROM tables WHERE n_table = " << n_table; // TODO: change tables for a varible that corresponds to the table name
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		if (res->next())
		{
			const int table_id = res->getInt("table_id");
			const int n = res->getInt("n_table");
			const int n_clients = res->getInt("n_clients");
			const double bill = res->getDouble("bill");
			const double discount = res->getDouble("discount");

			ss << "SELECT * FROM tableproduct WHERE table_id = " << table_id;
			res = executeStmt(conn, ss.str());
			ss.str("");

			domain::product_unordered_map products;
			while (res->next())
			{
				const int product_id = res->getInt("product_id");
				const int amount = res->getInt("amount");
				std::string details = res->getString("details");

				ss << "SELECT * FROM products WHERE product_id = " << product_id;
				auto res2 = executeStmt(conn, ss.str());
				ss.str("");

				if (res2->next())
				{
					const Product p(res2->getString("name"), res2->getDouble("price"), res2->getString("color"), res2->getInt("page"), res2->getBoolean("deployable"), details);

					products[p] = amount;
				}
			}

			table.n_table = n;
			table.n_clients = n_clients;
			table.products = products;
			table.bill = bill;
			table.discount = discount;
		}

		return table;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get table by number. Error message: " << e.what();
		return table;
	}
}

std::string Database::getLastModifiedFromTable(Conn& conn, 
											   const Table& table)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	std::string last_modified;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;

		ss << "SELECT last_modified FROM tables WHERE n_table = " << table.n_table; // TODO: change tables for a varible that corresponds to the table name
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		if (res->next())
		{
			last_modified = res->getString("last_modified");
		}

		return last_modified;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get last_modified. Error message: " << e.what();
		return last_modified;
	}
}


std::vector<Employee> Database::getEmployees(Conn& conn)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	std::vector<Employee> employees;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		auto res = executeStmt(conn, "SELECT * FROM employees"); // TODO: change employees for a varible that corresponds to the table name

		while (res->next())
		{
			const std::string firstName = res->getString("firstName");
			const std::string lastName = res->getString("lastName");
			const std::string email = res->getString("email");
			const std::string id = res->getString("id");
			const std::string mobileNumber = res->getString("mobileNumber");
			const int level = res->getInt("level");
			const std::string username = res->getString("username");
			const std::string password_hash = res->getString("password");
			const std::string session_token = res->getString("session_token");

			const Employee employee = { firstName, lastName, email, id, mobileNumber, level, username, password_hash, session_token };

			//std::cout << "Id: " << id << ". Employee name: " << name << " with level " << level << " and start time " << start << std::endl;
			employees.push_back(employee);
		}

		return employees;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get employees. Error message: " << e.what();
		return employees;
	}
}

Employee Database::getEmployeeByName(Conn& conn, 
									 const std::string& fullName)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	Employee employee;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT * FROM employees WHERE CONCAT(firstName, ' ', lastName) = '" << fullName << "'"; // String needs to be inside '' // TODO: change employees for a varible that corresponds to the table name
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		if (res->next())
		{
			employee.firstName = res->getString("firstName");
			employee.lastName = res->getString("lastName");
			employee.email = res->getString("email");
			employee.id = res->getString("id");
			employee.mobileNumber = res->getString("mobileNumber");
			employee.level = res->getInt("level");
			employee.username = res->getString("username");
			employee.password_hash = res->getString("password");
			employee.session_token = res->getString("session_token");
		}

		return employee;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get employee by name. Error message: " << e.what();
		return employee;
	}
}

Employee Database::getEmployeeByAccount(Conn& conn, 
										const std::string& username,
										const std::string& password_hash)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	Employee employee;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT * FROM employees WHERE username = '" << username << "' AND password ='" << password_hash << "'";
		auto res = executeStmt(conn, ss.str());

		if (res->next())
		{
			employee.firstName = res->getString("firstName");
			employee.lastName = res->getString("lastName");
			employee.email = res->getString("email");
			employee.id = res->getString("id");
			employee.mobileNumber = res->getString("mobileNumber");
			employee.level = res->getInt("level");
			employee.username = res->getString("username");
			employee.password_hash = res->getString("password");
			employee.session_token = res->getString("session_token");
		}

		return employee;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get employee by account. Error message: " << e.what();
		return employee;
	}
}

Employee Database::getEmployeeBySessionToken(Conn& conn, 
											 const std::string& session_token)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	Employee employee;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT * FROM employees WHERE session_token = '" << session_token << "'";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		if (res->next())
		{
			employee.firstName = res->getString("firstName");
			employee.lastName = res->getString("lastName");
			employee.email = res->getString("email");
			employee.id = res->getString("id");
			employee.mobileNumber = res->getString("mobileNumber");
			employee.level = res->getInt("level");
			employee.username = res->getString("username");
			employee.password_hash = res->getString("password");
			employee.session_token = res->getString("session_token");
		}

		return employee;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get employee by account. Error message: " << e.what();
		return employee;
	}
}

std::vector<Product> Database::getProducts(Conn& conn)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	std::vector<Product> products;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		auto res = executeStmt(conn, "SELECT * FROM products"); // TODO: change employees for a varible that corresponds to the table name

		while (res->next())
		{
			//int id = res->getInt("product_id");
			const std::string name = res->getString("name");
			const double price = res->getDouble("price");
			const std::string color = res->getString("color");
			const int page = res->getInt("page");
			const int deployable = res->getInt("deployable");

			const Product product = { name, price, color, page, deployable };

			//std::cout << "Id: " << id << ". Employee name: " << name << " with level " << level << " and start time " << start << std::endl;
			products.push_back(product);
		}

		return products;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get products. Error message: " << e.what();
		return products;
	}
}

std::unordered_map<int, Product> Database::getProducts2(Conn& conn)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	std::unordered_map<int, Product> products;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		auto res = executeStmt(conn, "SELECT * FROM products"); // TODO: change employees for a varible that corresponds to the table name

		while (res->next())
		{
			int id = res->getInt("product_id");
			const std::string name = res->getString("name");
			const double price = res->getDouble("price");
			const std::string color = res->getString("color");
			const int page = res->getInt("page");
			const int deployable = res->getInt("deployable");

			const Product product = { name, price, color, page, deployable };

			//std::cout << "Id: " << id << ". Employee name: " << name << " with level " << level << " and start time " << start << std::endl;
			products[id] = product;
		}

		return products;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get products2. Error message: " << e.what();
		return products;
	}
}


Product Database::getProductByName(Conn& conn, const std::string name)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	Product product;
	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT * FROM products WHERE name = '" << name << "'";
		auto res = executeStmt(conn, ss.str());

		if (res->next())
		{
			product.name = res->getString("name");
			product.price = res->getDouble("price");
			product.color = res->getString("color");
			product.page = res->getInt("page");
			product.deployable = res->getInt("deployable");
		}

		return product;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get product by name. Error message: " << e.what();
		return product; // Return an empty Product on error
	}
}

int Database::getProductIdByName(Conn& conn, const std::string name)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	int id = 0;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT product_id FROM products WHERE name = '" << name << "'";
		auto res = executeStmt(conn, ss.str());

		if (res->next())
		{
			id = res->getInt("product_id");
		}

		return id;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get product id by name. Error message: " << e.what();
		return id; // Return an empty Product on error
	}
}

std::vector<Product> Database::getProductsByDeployableId(Conn& conn, const int& deployable_id)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	std::vector<Product> products;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT * FROM products WHERE deployable = '" << deployable_id << "'";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		while (res->next())
		{
			const std::string name = res->getString("name");
			const double price = res->getDouble("price");
			const std::string color = res->getString("color");
			const int page = res->getInt("page");
			const int deployable = res->getInt("deployable");

			const Product p(name, price, color, page, deployable);
			products.push_back(p);
		}

		return products;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get products by deployable id. Error message: " << e.what();
		return products; // Return an empty vector on error
	}
}

std::pair<int, std::vector<Product>> Database::getProductsAndIds(Conn& conn)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	////std::unique_lock<std::shared_mutex> lock(mutex);

	const std::vector<Product> products = getProducts(conn);

	return std::pair<int, std::vector<Product>>();
}

std::vector<Order> Database::getOrders(Conn& conn)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	std::vector<Order> orders;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT * FROM orders";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		while (res->next())
		{
			const int order_id = res->getInt("order_id");
			const int n_table = res->getInt("n_table");
			const int n_clients = res->getInt("n_clients");
			const double bill = res->getDouble("bill");
			const double paid = res->getDouble("paid");
			const double discount = res->getDouble("discount");
			const std::string method = res->getString("method");
			const std::string employee = res->getString("employee");
			const std::string date = res->getString("date");

			ss << "SELECT * from orderproduct WHERE order_id = '" << order_id << "'";
			auto res2 = executeStmt(conn, ss.str());
			ss.str("");

			std::vector<std::pair<Product, int>> products;
			while (res2->next())
			{
				const int product_id = res2->getInt("product_id");
				const int amount = res2->getInt("amount");

				ss << "SELECT * from products WHERE product_id = '" << product_id << "'";
				auto res3 = executeStmt(conn, ss.str());
				ss.str("");

				if (res3->next())
				{
					const std::string name = res3->getString("name");
					const double price = res3->getDouble("price");
					const std::string color = res3->getString("color");
					const int page = res3->getInt("page");
					const int deployable = res3->getInt("deployable");

					const Product p(name, price, color, page, deployable);
					products.push_back({ p, amount });
				}
			}

			const Order o = { order_id, n_table, n_clients, bill, paid, discount, method, products, employee, date };
			orders.push_back(o);
		}

		return orders;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get orders. Error message: " << e.what();

		return orders;
	}
}

/*Order Database::getOrderByTime(const std::string time)
{
	/*Order order;

	try {
		//////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT * FROM orders WHERE time = '" << time << "'";
		auto res = executeStmt(conn, ss.str());

		if (res->next()) {
			std::string time = res->getString("time");
			std::string message = res->getString("message");

			order.time = time;
			order.message = message;
		}
	}
	catch (const sql::SQLException& e) {
		CROW_LOG_WARNING << "[EXCEPTION] Could not get order by time. Error message: " << e.what();
	}

	return order;
}*/

Order Database::getOrderById(Conn& conn, const int& id)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	Order order;

	try
	{
		//////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT * FROM orders WHERE order_id = '" << id << "'";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		if (res->next())
		{
			const int order_id = id;
			const int n_table = res->getInt("n_table");
			const int n_clients = res->getInt("n_clients");
			const double bill = res->getDouble("bill");
			const double paid = res->getDouble("paid");
			const double discount = res->getDouble("discount");
			const std::string method = res->getString("method");
			const std::string employee = res->getString("employee");
			const std::string date = res->getString("date");

			ss << "SELECT * from orderproduct WHERE order_id = '" << order_id << "'";
			auto res2 = executeStmt(conn, ss.str());
			ss.str("");

			std::vector<std::pair<Product, int>> products;
			while (res2->next())
			{
				const int product_id = res2->getInt("product_id");
				const int amount = res2->getInt("amount");

				ss << "SELECT * from products WHERE product_id = '" << product_id << "'";
				auto res3 = executeStmt(conn, ss.str());
				ss.str("");

				if (res3->next())
				{
					const std::string name = res3->getString("name");
					const double price = res3->getDouble("price");
					const std::string color = res3->getString("color");
					const int page = res3->getInt("page");
					const int deployable = res3->getInt("deployable");

					const Product p(name, price, color, page, deployable);
					products.push_back({ p, amount });
				}
			}

			order = { order_id, n_table, n_clients, bill, paid, discount, method, products, employee, date };
		}

		return order;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get order by id. Error message: " << e.what();

		return order;
	}
}


std::vector<Order> Database::getOrdersByDate(Conn& conn, const std::string& date, const std::string& mode)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	std::vector<Order> orders;

	try
	{
		CROW_LOG_DEBUG << "[DB] " << __func__ << date << ".";
		//////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT * FROM orders WHERE " << mode << "(date) = '" << date << "'";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		while (res->next())
		{
			const int order_id = res->getInt("order_id");
			const int n_table = res->getInt("n_table");
			const int n_clients = res->getInt("n_clients");
			const double bill = res->getDouble("bill");
			const double paid = res->getDouble("paid");
			const double discount = res->getDouble("discount");
			const std::string method = res->getString("method");
			const std::string employee = res->getString("employee");
			const std::string date = res->getString("date");

			ss << "SELECT * from orderproduct WHERE order_id = '" << order_id << "'";
			auto res2 = executeStmt(conn, ss.str());
			ss.str("");

			std::vector<std::pair<Product, int>> products;
			while (res2->next())
			{
				const int product_id = res2->getInt("product_id");
				const int amount = res2->getInt("amount");

				ss << "SELECT * from products WHERE product_id = '" << product_id << "'";
				auto res3 = executeStmt(conn, ss.str());
				ss.str("");

				if (res3->next())
				{
					const std::string name = res3->getString("name");
					const double price = res3->getDouble("price");
					const std::string color = res3->getString("color");
					const int page = res3->getInt("page");
					const int deployable = res3->getInt("deployable");

					const Product p(name, price, color, page, deployable);
					products.push_back({ p, amount });
				}
			}

			const Order o = { order_id, n_table, n_clients, bill, paid, discount, method, products, employee, date };
			orders.push_back(o);
		}

		return orders;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get orders by date. Error message: " << e.what();

		return orders;
	}
}

std::vector<Order> Database::getOrdersByEmployee(Conn& conn, const std::string& employeeName)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	std::vector<Order> orders;

	try
	{
		//////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT * FROM orders WHERE employee = '" << employeeName << "'";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		while (res->next())
		{
			const int order_id = res->getInt("order_id");
			const int n_table = res->getInt("n_table");
			const int n_clients = res->getInt("n_clients");
			const double bill = res->getDouble("bill");
			const double paid = res->getDouble("paid");
			const double discount = res->getDouble("discount");
			const std::string method = res->getString("method");
			const std::string employee = res->getString("employee");
			const std::string date = res->getString("date");

			ss << "SELECT * from orderproduct WHERE order_id = '" << order_id << "'";
			auto res2 = executeStmt(conn, ss.str());
			ss.str("");

			std::vector<std::pair<Product, int>> products;
			while (res2->next())
			{
				const int product_id = res2->getInt("product_id");
				const int amount = res2->getInt("amount");

				ss << "SELECT * from products WHERE product_id = '" << product_id << "'";
				auto res3 = executeStmt(conn, ss.str());
				ss.str("");

				if (res3->next())
				{
					const std::string name = res3->getString("name");
					const double price = res3->getDouble("price");
					const std::string color = res3->getString("color");
					const int page = res3->getInt("page");
					const int deployable = res3->getInt("deployable");

					const Product p(name, price, color, page, deployable);
					products.push_back({ p, amount });
				}
			}

			const Order o = { order_id, n_table, n_clients, bill, paid, discount, method, products, employee, date };
			orders.push_back(o);
		}

		return orders;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get orders by employee. Error message: " << e.what();

		return orders;
	}
}

std::vector<Order> Database::getOrdersByMethod(Conn& conn, const std::string& method)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	std::vector<Order> orders;

	try
	{
		//////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT * FROM orders WHERE method = '" << method << "'";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		while (res->next())
		{
			const int order_id = res->getInt("order_id");
			const int n_table = res->getInt("n_table");
			const int n_clients = res->getInt("n_clients");
			const double bill = res->getDouble("bill");
			const double paid = res->getDouble("paid");
			const double discount = res->getDouble("discount");
			const std::string method = res->getString("method");
			const std::string employee = res->getString("employee");
			const std::string date = res->getString("date");

			ss << "SELECT * from orderproduct WHERE order_id = '" << order_id << "'";
			auto res2 = executeStmt(conn, ss.str());
			ss.str("");

			std::vector<std::pair<Product, int>> products;
			while (res2->next())
			{
				const int product_id = res2->getInt("product_id");
				const int amount = res2->getInt("amount");

				ss << "SELECT * from products WHERE product_id = '" << product_id << "'";
				auto res3 = executeStmt(conn, ss.str());
				ss.str("");

				if (res3->next())
				{
					const std::string name = res3->getString("name");
					const double price = res3->getDouble("price");
					const std::string color = res3->getString("color");
					const int page = res3->getInt("page");
					const int deployable = res3->getInt("deployable");

					const Product p(name, price, color, page, deployable);
					products.push_back({ p, amount });
				}
			}

			const Order o = { order_id, n_table, n_clients, bill, paid, discount, method, products, employee, date };
			orders.push_back(o);
		}

		return orders;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get orders by method. Error message: " << e.what();

		return orders;
	}
}

std::vector<Order> Database::getOrdersByNTable(Conn& conn, const std::string& n_table)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	int nTable = 0;
	try
	{
		nTable = std::stoi(n_table);
	}
	catch (const std::invalid_argument& e){}

	std::vector<Order> orders;
	try
	{
		//////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT * FROM orders WHERE n_table = '" << nTable << "'";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		while (res->next())
		{
			const int order_id = res->getInt("order_id");
			const int n_table = res->getInt("n_table");
			const int n_clients = res->getInt("n_clients");
			const double bill = res->getDouble("bill");
			const double paid = res->getDouble("paid");
			const double discount = res->getDouble("discount");
			const std::string method = res->getString("method");
			const std::string employee = res->getString("employee");
			const std::string date = res->getString("date");

			ss << "SELECT * from orderproduct WHERE order_id = '" << order_id << "'";
			auto res2 = executeStmt(conn, ss.str());
			ss.str("");

			std::vector<std::pair<Product, int>> products;
			while (res2->next())
			{
				const int product_id = res2->getInt("product_id");
				const int amount = res2->getInt("amount");

				ss << "SELECT * from products WHERE product_id = '" << product_id << "'";
				auto res3 = executeStmt(conn, ss.str());
				ss.str("");

				if (res3->next())
				{
					const std::string name = res3->getString("name");
					const double price = res3->getDouble("price");
					const std::string color = res3->getString("color");
					const int page = res3->getInt("page");
					const int deployable = res3->getInt("deployable");

					const Product p(name, price, color, page, deployable);
					products.push_back({ p, amount });
				}
			}

			const Order o = { order_id, n_table, n_clients, bill, paid, discount, method, products, employee, date };
			orders.push_back(o);
		}

		return orders;
	}
	catch (const sql::SQLException& e)
		{
			CROW_LOG_WARNING << "[EXCEPTION] Could not get orders by n_table. Error message: " << e.what();

			return orders;
		}
}


std::vector<BillAndPaid> Database::getBillsAndPaids(Conn& conn)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	std::vector<BillAndPaid> billsAndPaids;

	try
	{
		//////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT bill, paid FROM orders";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		while (res->next())
		{
			const double bill = res->getDouble("bill");
			const double paid = res->getDouble("paid");
			const std::string date = res->getString("date");

			billsAndPaids.push_back({ bill, paid, date });
		}

		return billsAndPaids;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get bills. Error message: " << e.what();

		return billsAndPaids;
	}
}

std::vector<BillAndPaid> Database::getBillsAndPaidsByDate(Conn& conn, const std::string& date, const std::string& mode)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	std::vector<BillAndPaid> billsAndPaids;

	try
	{
		//////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT bill, paid, date FROM orders WHERE " << mode << "(date) = " << date;
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		while (res->next())
		{
			const double bill = res->getDouble("bill");
			const double paid = res->getDouble("paid");
			const std::string date = res->getString("date");

			billsAndPaids.push_back({ bill, paid, date });
		}

		return billsAndPaids;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get bills by date. Error message: " << e.what();

		return billsAndPaids;
	}
}

std::vector<BillAndPaid> Database::getBillsAndPaidsByEmployee(Conn& conn, const std::string& employeeName)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	std::vector<BillAndPaid> billsAndPaids;

	try
	{
		//////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT bill, paid, date FROM orders WHERE employee = '" << employeeName << "'";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		while (res->next())
		{
			const double bill = res->getDouble("bill");
			const double paid = res->getDouble("paid");
			const std::string date = res->getString("date");

			billsAndPaids.push_back({ bill, paid, date });
		}

		return billsAndPaids;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get bills by employee. Error message: " << e.what();

		return billsAndPaids;
	}
}

int Database::getNClients(Conn& conn)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	int totalNClients = 0;

	try
	{
		//////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT n_clients FROM orders";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		while (res->next())
		{
			totalNClients += res->getInt("n_clients");
		}

		return totalNClients;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get number of clients. Error message: " << e.what();

		return totalNClients;
	}
}

int Database::getNClientsByDate(Conn& conn, const std::string& date, const std::string& mode)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	int totalNClients = 0;

	try
	{
		//////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT n_clients FROM orders WHERE " << mode << "(date) = '" << date << "'";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		while (res->next())
		{
			totalNClients += res->getInt("n_clients");
		}

		return totalNClients;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get number of clients by date. Error message: " << e.what();

		return totalNClients;
	}
}

int Database::getNClientsByEmployee(Conn& conn, const std::string& employeeName)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	int totalNClients = 0;

	try
	{
		//////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT n_clients FROM orders WHERE employee = '" << employeeName << "'";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		while (res->next())
		{
			totalNClients += res->getInt("n_clients");
		}

		return totalNClients;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get number of clients by employee. Error message: " << e.what();

		return totalNClients;
	}
}

std::unordered_map<int, OrderedProduct> Database::getOrderedProducts(Conn& conn)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	using ProductId = int;
	std::unordered_map<ProductId, OrderedProduct> products;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT SUM(amount) AS totalAmount FROM orderproduct";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		int totalAmount = 0;
		if (res->next())
		{
			totalAmount = res->getInt("totalAmount");

			ss << "SELECT * FROM orders";
			res = executeStmt(conn, ss.str());
			ss.str("");

			while (res->next())
			{
				const int order_id = res->getInt("order_id");

				ss << "SELECT * from orderproduct WHERE order_id = '" << order_id << "'";
				auto res2 = executeStmt(conn, ss.str());
				ss.str("");

				while (res2->next())
				{
					const int product_id = res2->getInt("product_id");
					const int amount = res2->getInt("amount");

					ss << "SELECT * from products WHERE product_id = '" << product_id << "'";
					auto res3 = executeStmt(conn, ss.str());
					ss.str("");

					if (res3->next())
					{
						products[product_id].id = product_id;
						products[product_id].name = res3->getString("name");
						products[product_id].page = res3->getInt("page");
						products[product_id].price = res3->getDouble("price");
						products[product_id].sold += amount;
						products[product_id].percent = (products[product_id].sold / (double)totalAmount) * 100.0;
						products[product_id].revenue = 0;
						products[product_id].totalRevenue += (amount * products[product_id].revenue);

						const int deployable = res3->getInt("deployable");
						ss << "SELECT name from products WHERE product_id = '" << deployable << "'";
						res3 = executeStmt(conn, ss.str());
						ss.str("");

						std::string menu = "";
						if (res3->next())
						{
							menu = res3->getString("name");
						}

						products[product_id].menu = menu;
					}
				}
			}
		}

		return products;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get orders. Error message: " << e.what();

		return products;
	}
}

OrderedProduct Database::getOrderedProductById(Conn& conn, const int& product_id)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	OrderedProduct product;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT SUM(amount) AS totalAmount FROM orderproduct";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		int totalAmount = 0;
		if (res->next())
		{
			totalAmount = res->getInt("totalAmount");

			ss << "SELECT * from orderproduct WHERE product_id = '" << product_id << "'";
			res = executeStmt(conn, ss.str());
			ss.str("");

			product.id = product_id;

			while (res->next())
			{
				const int product_id = res->getInt("product_id");
				const int amount = res->getInt("amount");

				ss << "SELECT * from products WHERE product_id = '" << product_id << "'";
				auto res2 = executeStmt(conn, ss.str());
				ss.str("");

				if (res2->next() && res2->getInt("deployable") != 0)
				{
					product.name = res2->getString("name");
					product.price = res2->getDouble("price");
					product.page = res2->getInt("page");
					product.sold += amount;

					product.percent = (product.sold / (double)totalAmount) * 100.0;
					product.revenue = 0;
					product.totalRevenue = (amount * product.revenue);
				}
			}
		}

		return product;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get orderedProduct by id. Error message: " << e.what();

		return product;
	}
}

OrderedProduct Database::getOrderedProductByName(Conn& conn, const std::string& name)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	OrderedProduct product;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT SUM(amount) AS totalAmount FROM orderproduct";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		int totalAmount = 0;
		if (res->next())
		{
			totalAmount = res->getInt("totalAmount");

			ss << "SELECT * FROM products WHERE name = '" << name << "'";
			res = executeStmt(conn, ss.str());
			ss.str("");

			if (res->next() && res->getInt("deployable") != 0)
			{
				int product_id = res->getInt("product_id");
				const int deployable = res->getInt("deployable");

				product.id = product_id;
				product.name = name;
				product.page = res->getInt("page");
				product.price = res->getDouble("price");
				product.revenue = 0;

				ss << "SELECT * FROM orderproduct WHERE product_id = " << product_id;
				res = executeStmt(conn, ss.str());
				ss.str("");

				while (res->next())
				{
					ss << "SELECT * from orderproduct WHERE product_id = '" << product_id << "'";
					auto res2 = executeStmt(conn, ss.str());
					ss.str("");

					product.sold += res->getInt("amount");
					product.percent = (product.sold / (double)totalAmount) * 100.0;
					product.totalRevenue = (product.sold * product.revenue);	
				}

				ss << "SELECT name from products WHERE product_id = '" << deployable << "'";
				res = executeStmt(conn, ss.str());
				ss.str("");

				if (res->next())
				{
					product.menu = res->getString("name");
				}
			}
		}

		return product;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get orderedProduct by name. Error message: " << e.what();

		return product;
	}
}

std::unordered_map<int, OrderedProduct> Database::getOrderedProductsByPage(Conn& conn, 
																		   const int& page)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	using ProductId = int;
	std::unordered_map<ProductId, OrderedProduct> products;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT SUM(amount) AS totalAmount FROM orderproduct";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		int totalAmount = 0;
		if (res->next())
		{
			totalAmount = res->getInt("totalAmount");

			ss << "SELECT * FROM products WHERE page = '" << page << "'";
			res = executeStmt(conn, ss.str());
			ss.str("");

			while (res->next())
			{
				const int deployable = res->getInt("deployable");

				if (deployable != 0)
				{
					const int product_id = res->getInt("product_id");

					products[product_id].id = product_id;
					products[product_id].name = res->getString("name");
					products[product_id].page = page;
					products[product_id].price = res->getDouble("price");
					products[product_id].revenue = 0;

					ss << "SELECT * FROM orderproduct WHERE product_id = " << product_id;
					auto res2 = executeStmt(conn, ss.str());
					ss.str("");

					while (res2->next())
					{
						products[product_id].sold += res2->getInt("amount");
						products[product_id].percent = (products[product_id].sold / (double)totalAmount) * 100.0;
						products[product_id].totalRevenue = (products[product_id].sold * products[product_id].revenue);
					}

					ss << "SELECT name from products WHERE product_id = '" << deployable << "'";
					res2 = executeStmt(conn, ss.str());
					ss.str("");

					std::string menu = "";
					if (res2->next())
					{
						menu = res2->getString("name");
					}

					products[product_id].menu = menu;
				}
			}
		}

		return products;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get orderedProducts by page. Error message: " << e.what();

		return products;
	}
}

std::unordered_map<int, OrderedProduct> Database::getOrderedProductsByMenu(Conn& conn, 
																		   const std::string& menu)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	using ProductId = int;
	std::unordered_map<ProductId, OrderedProduct> products;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT SUM(amount) AS totalAmount FROM orderproduct";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		int totalAmount = 0;
		if (res->next())
		{
			totalAmount = res->getInt("totalAmount");

			ss << "SELECT product_id FROM products WHERE name = '" << menu << "'";
			res = executeStmt(conn, ss.str());
			ss.str("");

			if (res->next())
			{
				const int menuId = res->getInt("product_id");

				ss << "SELECT * FROM products WHERE deployable = '" << menuId << "'";
				res = executeStmt(conn, ss.str());
				ss.str("");

				while (res->next())
				{
					const int product_id = res->getInt("product_id");

					products[product_id].id = product_id;
					products[product_id].name = res->getString("name");
					products[product_id].page = res->getInt("page");
					products[product_id].menu = menu;
					products[product_id].price = res->getDouble("price");
					products[product_id].revenue = 0;

					ss << "SELECT * FROM orderproduct WHERE product_id = " << product_id;
					auto res2 = executeStmt(conn, ss.str());
					ss.str("");

					while (res2->next())
					{
						products[product_id].sold += res2->getInt("amount");
						products[product_id].percent = (products[product_id].sold / (double)totalAmount) * 100.0;
						products[product_id].totalRevenue = (products[product_id].sold * products[product_id].revenue);
					}
				}
			}
		}

		return products;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get orderedProducts by menu. Error message: " << e.what();

		return products;
	}
}

std::unordered_map<int, OrderedProduct> Database::getOrderedProductsByPrice(Conn& conn, 
																			const double& price)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	using ProductId = int;
	std::unordered_map<ProductId, OrderedProduct> products;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT SUM(amount) AS totalAmount FROM orderproduct";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		int totalAmount = 0;
		if (res->next())
		{
			totalAmount = res->getInt("totalAmount");

			ss << "SELECT * FROM products WHERE price = '" << price << "'";
			res = executeStmt(conn, ss.str());
			ss.str("");

			const int deployable = res->getInt("deployable");
			while (res->next() && deployable != 0)
			{
				const int product_id = res->getInt("product_id");

				products[product_id].id = product_id;
				products[product_id].name = res->getString("name");
				products[product_id].page = res->getInt("page");
				products[product_id].price = price;
				products[product_id].revenue = 0;

				ss << "SELECT * FROM orderproduct WHERE product_id = " << product_id;
				auto res2 = executeStmt(conn, ss.str());
				ss.str("");

				while (res2->next())
				{
					products[product_id].sold += res2->getInt("amount");
					products[product_id].percent = (products[product_id].sold / (double)totalAmount) * 100.0;
					products[product_id].totalRevenue = (products[product_id].sold * products[product_id].revenue);
				}

				ss << "SELECT name from products WHERE product_id = '" << deployable << "'";
				res2 = executeStmt(conn, ss.str());
				ss.str("");

				std::string menu = "";
				if (res2->next())
				{
					menu = res2->getString("name");
				}

				products[product_id].menu = menu;
			}
		}

		return products;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get orderedProducts by price. Error message: " << e.what();

		return products;
	}
}

std::unordered_map<int, OrderedProduct> Database::getOrderedProductsByDate(Conn& conn, 
																		   const std::string& date, const std::string& mode)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	std::unordered_map<int, OrderedProduct> products;

	try
	{
		CROW_LOG_DEBUG << "[DB] " << __func__ << date << ".";
		//////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT * FROM orders WHERE " << mode << "(date) = '" << date << "'";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		int totalAmount = 0;
		while (res->next())
		{
			const int order_id = res->getInt("order_id");

			ss << "SELECT * from orderproduct WHERE order_id = '" << order_id << "'";
			auto res2 = executeStmt(conn, ss.str());
			ss.str("");

			while (res2->next())
			{
				ss << "SELECT amount from orderproduct WHERE order_id = '" << order_id << "'";
				auto res3 = executeStmt(conn, ss.str());
				ss.str("");

				while (res3->next())
				{
					totalAmount += res3->getInt("amount");
				}
			}

			ss << "SELECT * from orderproduct WHERE order_id = '" << order_id << "'";
			res2 = executeStmt(conn, ss.str());
			ss.str("");

			while (res2->next())
			{
				const int product_id = res2->getInt("product_id");
				const int amount = res2->getInt("amount");

				products[product_id].id = product_id;
				products[product_id].sold += amount;
				products[product_id].percent = (products[product_id].sold / totalAmount) * 100.0;

				ss << "SELECT * from products WHERE product_id = '" << product_id << "'";
				auto res3 = executeStmt(conn, ss.str());
				ss.str("");

				if (res3->next() && res3->getInt("deployable"))
				{
					const int deployable = res3->getInt("deployable");

					products[product_id].name = res3->getString("name");
					products[product_id].page = res3->getInt("page");
					products[product_id].price = res3->getDouble("price");
					products[product_id].revenue = 0;
					products[product_id].totalRevenue = products[product_id].sold * products[product_id].revenue;

					ss << "SELECT name from products WHERE product_id = '" << deployable << "'";
					res3 = executeStmt(conn, ss.str());
					ss.str("");

					std::string menu = "";
					if (res3->next())
					{
						menu = res3->getString("name");
					}
						
					products[product_id].menu = menu;
				}
			}
		}

		return products;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get orderedProducts by date. Error message: " << e.what();

		return products;
	}
}

std::vector<Ingredient> Database::getIngredients(Conn& conn)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	std::vector<Ingredient> ingredients;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		auto res = executeStmt(conn, "SELECT * FROM ingredients");

		while (res->next())
		{
			const std::string name = res->getString("name");
			Ingredient ingredient = { name };
			ingredients.push_back(ingredient);
		}

		return ingredients;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get ingredients. Error message: " << e.what();
		return ingredients;
	}
}

Ingredient Database::getIngredientByName(Conn& conn, 
										 const std::string& name)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	Ingredient ingredient;

	try
	{
		//////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT * FROM ingredients WHERE name = '" << name << "'";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		if (res->next())
		{
			ingredient.name = res->getString("name");
		}

		return ingredient;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get ingredient by name. Error message: " << e.what();
		return ingredient; // Return an empty Ingredient on error
	}
}

std::vector<Ingredient> Database::getIngredientsFromProduct(Conn& conn, 
															const Product& product)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	std::vector<Ingredient> ingredients;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT product_id FROM products WHERE name = '" << product.name << "' AND price = '" << product.price << "' AND page = '" << product.page << "'";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		if (res->next())
		{
			int product_id = res->getInt("product_id");

			ss << "SELECT ingredient_id FROM productingredient WHERE product_id = " << product_id;
			res = executeStmt(conn, ss.str());
			ss.str("");

			while (res->next())
			{
				const int ingredient_id = res->getInt("ingredient_id");

				ss << "SELECT * FROM ingredients WHERE ingredient_id = " << ingredient_id;
				auto res2 = executeStmt(conn, ss.str());
				ss.str("");

				if (res2->next())
				{
					ingredients.push_back({ res2->getString("name") });
				}
			}
		}

		return ingredients;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get ingredients from product. Error message: " << e.what();
		return ingredients; // Return an empty vector on error
	}
}

std::vector<Allergen> Database::getAllergens(Conn& conn)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	std::vector<Allergen> allergens;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		auto res = executeStmt(conn, "SELECT * FROM allergens");

		while (res->next())
		{
			const std::string name = res->getString("name");
			Allergen allergen = { name };
			allergens.push_back(allergen);
		}

		return allergens;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get allergens. Error message: " << e.what();
		return std::vector<Allergen>();
	}
}

Allergen Database::getAllergenByName(Conn& conn, 
									 const std::string name)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	Allergen allergen;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT * FROM allergens WHERE name = '" << name << "'";
		auto res = executeStmt(conn, ss.str());

		if (res->next())
		{
			const std::string name = res->getString("name");
			allergen.name = name;
		}

		return allergen;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get allergen by name. Error message: " << e.what();
		return allergen; // Return an empty Allergen on error
	}
}

std::vector<Allergen> Database::getAllergensFromProduct(Conn& conn, 
														const Product& product)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	std::vector<Allergen> allergens;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "SELECT product_id FROM products WHERE name = '" << product.name << "' AND price = '" << product.price << "' AND page = '" << product.page << "'";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		if (res->next())
		{
			const int product_id = res->getInt("product_id");

			ss << "SELECT allergen_id FROM productallergen WHERE product_id = " << product_id;
			res = executeStmt(conn, ss.str());
			ss.str("");

			while (res->next())
			{
				const int allergen_id = res->getInt("allergen_id");

				ss << "SELECT * FROM allergens WHERE allergen_id = " << allergen_id;
				auto res2 = executeStmt(conn, ss.str());
				ss.str("");

				if (res2->next())
				{
					allergens.push_back({ res2->getString("name") });
				}
			}
		}

		return allergens;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get allergens from product. Error message: " << e.what();
		return allergens;
	}
}

std::vector<page_t> Database::getDataFromPages(Conn& conn)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	std::vector<page_t> pages(cts::N_FOURTH_ROW_BUTTONS);
	int i = 0;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		for (auto& p : pages)
		{
			std::stringstream ss;
			ss << "SELECT * FROM products WHERE page = '" << i + 1 << "'";
			auto res = executeStmt(conn, ss.str());
			ss.str("");

			while (res->next())
			{
				const std::string deployableName = res->getString("name");
				const double deployablePrice = res->getDouble("price");
				const std::string deployableColor = res->getString("color");
				const int deployablePage = res->getInt("page");
				const bool deployableDeployable = res->getBoolean("deployable");

				Product p1(deployableName, deployablePrice, deployableColor, deployablePage, deployableDeployable);
				std::vector<Product> deployable_vector;

				if (deployableDeployable == 0)
				{
					if (deployablePrice == 0)
					{
						ss << "SELECT product_id FROM products WHERE name = '" << deployableName << "' AND price = 0 AND color = '" << deployableColor << "' AND page = '" << deployablePage << "' AND deployable = 0";
						auto res2 = executeStmt(conn, ss.str());
						ss.str("");

						if (res2->next())
						{
							int deployable_id = res->getInt("product_id");

							ss << "SELECT * FROM products WHERE deployable = '" << deployable_id << "'";
							res2 = executeStmt(conn, ss.str());
							ss.str("");

							deployable_vector = { Product("", 0.0, "#FFFFFF", 0, 0) };
							while (res2->next())
							{
								const std::string name = res2->getString("name");
								const double price = res2->getDouble("price");
								const std::string color = res2->getString("color");
								const int page = res2->getInt("page");
								const bool deployable = res2->getBoolean("deployable");

								Product p2(name, price, color, page, deployable);
								deployable_vector.push_back(p2);
							}
						}
					}
				}

				p.push_back({ p1, deployable_vector });
			}

			i++;
		}

		return pages;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get data from pages. Error message: " << e.what();
		return pages;
	}
}

std::string Database::generateSessionToken()
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	std::random_device rd;
	std::mt19937 generator(rd());

	std::uniform_int_distribution<int> distribution(0, cts::ASCII_CHARACTERS.size() - 1);

	std::stringstream ss;
	for (int i = 0; i < cts::SESSION_TOKEN_LENGTH; ++i)
	{
		ss << cts::ASCII_CHARACTERS[distribution(generator)];
	}

	std::string new_session_token = ss.str();

	return new_session_token;
}

void Database::payTable(Conn& conn, const Order& order)
{
	//std::unique_lock<std::shared_mutex> lock(mutex, std::adopt_lock);

	Table t_aux;
	{
		Table t = getTableByNumber(conn, order.n_table);
		t_aux = t;
		std::cout << "B2" << std::endl;
	}

	saveOrder(conn, order);
	std::cout << "B3" << std::endl;

	removeTable(conn, t_aux);
	std::cout << "B4" << std::endl;
}


// Print
void Database::printTables(Conn& conn)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	const std::vector<Table> tables = getTables(conn);

	CROW_LOG_INFO << "[LIST] Tables: ";
	for (const auto table : tables)
	{
		std::cout << "\t- Table Number: " << table.n_table << ", Bill: " << table.bill << ", Discount: " << table.discount << std::endl;
	}
}

void Database::printEmployees(Conn& conn)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	const std::vector<Employee> employees = getEmployees(conn);

	CROW_LOG_INFO << "[LIST] Employees: ";
	for (const auto employee : employees)
	{
		std::cout << "\t- Name: " << employee.firstName << " " << employee.lastName << ", Level: " << employee.level;
	}
}

void Database::printProducts(Conn& conn)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	const std::vector<Product> products = getProducts(conn);

	CROW_LOG_INFO << "[LIST] Products: ";
	for (const auto product : products)
	{
		std::cout << "\t- Name: " << product.name << ", Price: " << product.price << std::endl;
	}
}

void Database::printOrders(Conn& conn)
{
	/*
	CROW_LOG_DEBUG << "[DB] " << __func__;

	const std::vector<Order> orders = getOrders();

	CROW_LOG_INFO << "[LIST] Orders: ";
	for (const auto order : orders) {
		std::cout << "\t- Time: " << order.time << ", Message: " << order.message << std::endl;
	}*/
}

void Database::printIngredients(Conn& conn)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	const std::vector<Ingredient> ingredients = getIngredients(conn);

	CROW_LOG_INFO << "[LIST] Ingredients: ";
	for (const auto ingredient : ingredients)
	{
		std::cout << "\t- Name: " << ingredient.name << std::endl;
	}
}

void Database::printAllergens(Conn& conn)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	const std::vector<Allergen> allergens = getAllergens(conn);

	CROW_LOG_INFO << "[LIST] Allergens: ";
	for (const auto allergen : allergens)
	{
		std::cout << "\t- Name: " << allergen.name << std::endl;
	}
}


// Change
void Database::moveTable(Conn& conn, 
						 const int& current_n_table,
						 const int& new_n_table)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	try
	{
		std::stringstream ss;

		ss << "SELECT * FROM tables WHERE n_table = " << current_n_table;
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		res->next();
		const int current_table_id = res->getInt("table_id");

		ss << "SELECT table_id, bill FROM tables WHERE n_table = " << new_n_table;
		res = executeStmt(conn, ss.str());
		ss.str("");

		const Table curr_table = getTableByNumber(conn, current_n_table);
		const Table new_table = getTableByNumber(conn, new_n_table);

		// If theres an existing table
		if (res->next())
		{
			 const int new_table_id = res->getInt("table_id");
			 int new_table_bill = res->getInt("bill");

			ss << "SELECT product_id, amount FROM tableproduct WHERE table_id =" << current_table_id;
			res = executeStmt(conn, ss.str());
			ss.str("");

			// For each product of current table
			while (res->next())
			{
				const int current_table_product_amount = res->getInt("amount");
				const int current_table_product_id = res->getInt("product_id");

				ss << "SELECT * FROM tableproduct WHERE table_id =" << new_table_id << " AND product_id = " << current_table_product_id;
				auto res2 = executeStmt(conn, ss.str());
				ss.str("");

				// If product_id matches between current and new table. Update amount and new table bill
				if (res2->next())
				{
					int new_table_product_id = res2->getInt("product_id");
					int new_table_amount = res2->getInt("amount");

					ss << "SELECT * FROM tableproduct WHERE table_id =" << current_table_id << " AND product_id = " << current_table_product_id;
					auto res3 = executeStmt(conn, ss.str());
					ss.str("");

					if (res3->next())
					{
						int current_table_amount = res3->getInt("amount");
						new_table_amount += current_table_amount;

						// Update new table product amount
						auto pstmt1 = prepareStatement(conn, "UPDATE tableproduct SET amount = ? WHERE table_id = ? AND product_id = ?");
						pstmt1->setInt(1, new_table_amount);
						pstmt1->setInt(2, new_table_id);
						pstmt1->setInt(3, current_table_product_id);
						pstmt1->execute();

						ss << "SELECT price FROM products WHERE product_id =" << current_table_product_id;
						res3 = executeStmt(conn, ss.str());
						ss.str("");

						if (res3->next())
						{
							double current_table_product_price = res3->getDouble("price");
							new_table_bill += current_table_product_price * current_table_product_amount;

							// Update new table bill
							auto pstmt2 = prepareStatement(conn, "UPDATE tables SET bill = ? WHERE table_id = ?");
							pstmt2->setInt(1, new_table_bill);
							pstmt2->setInt(2, new_table_id);
							pstmt2->execute();

							// Delete the current table tableproduct row
							auto pstmt3 = prepareStatement(conn, "DELETE FROM tableproduct WHERE table_id = ? AND product_id = ?");
							pstmt3->setInt(1, current_table_id);
							pstmt3->setInt(2, current_table_product_id);
							pstmt3->execute();
						}
					}
				}
				else
				{ // If product doesnt match, change its table_id and update table bill
					auto pstmt1 = prepareStatement(conn, "UPDATE tableproduct SET table_id = ? WHERE table_id = ? AND product_id = ?");
					pstmt1->setInt(1, new_table_id);
					pstmt1->setInt(2, current_table_id);
					pstmt1->setInt(3, current_table_product_id);
					pstmt1->execute();

					ss << "SELECT price FROM products WHERE product_id =" << current_table_product_id;
					auto res3 = executeStmt(conn, ss.str());
					ss.str("");

					res3->next();
					int current_table_product_price = res3->getDouble("price");
					new_table_bill += (current_table_product_price * current_table_product_amount);

					auto pstmt2 = prepareStatement(conn, "UPDATE tables SET bill = ? WHERE table_id = ?");
					pstmt2->setInt(1, new_table_bill);
					pstmt2->setInt(2, new_table_id);
					pstmt2->execute();
				}
			}

			removeTable(conn, curr_table);
		}
		else
		{ // If there is no existing table just change n_table
			auto pstmt = prepareStatement(conn, "UPDATE tables SET n_table = ? WHERE table_id = ?");
			pstmt->setInt(1, new_n_table);
			pstmt->setInt(2, current_table_id);
			pstmt->execute();
		}
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not change table number. Error message: " << e.what();
	}
}

void Database::changeTableProductAmount(Conn& conn, 
										const Table& table,
										const Product& product,
										const int& new_amount)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;

		ss << "SELECT table_id, bill, discount FROM tables WHERE n_table = " << table.n_table;
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		if (res->next())
		{
			const int table_id = res->getInt("table_id");
			const double bill = res->getDouble("bill");
			const double discount = res->getDouble("discount");

			ss << "SELECT product_id FROM products WHERE name = '" << product.name << "' AND price = '" << product.price << "'";
			res = executeStmt(conn, ss.str());
			ss.str("");

			if (res->next())
			{
				const int product_id = res->getInt("product_id");

				ss << "SELECT amount FROM tableproduct WHERE table_id = '" << table_id << "' AND product_id = '" << product_id << "'";
				res = executeStmt(conn, ss.str());
				ss.str("");

				if (res->next())
				{
					const int amount = res->getInt("amount");

					if (new_amount > 0)
					{
						const double new_bill = bill + product.price * (new_amount - amount) * (1.0 - discount);

						auto pstmt1 = prepareStatement(conn, "UPDATE tables SET bill = ? WHERE n_table = ?");
						pstmt1->setDouble(1, new_bill);
						pstmt1->setInt(2, table.n_table);
						pstmt1->execute();

						auto pstmt2 = prepareStatement(conn, "UPDATE tableproduct SET amount = ? WHERE table_id = ? AND product_id = ?");
						pstmt2->setInt(1, new_amount);
						pstmt2->setInt(2, table_id);
						pstmt2->setInt(3, product_id);
						pstmt2->execute();
					}
					else
					{
						removeTableProduct(conn, table.n_table, product, amount);
					}
				}
			}
		}
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not change tableproduct amount. Error message: " << e.what();
	}
}


// Remove
void Database::removeTable(Conn& conn, 
						   const Table& table)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		const double bill = table.bill;
		const double discount = table.discount;
		const int n_table = table.n_table;

		std::stringstream ss;
		ss << "SELECT * FROM tables WHERE n_table = " << table.n_table;
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		if (res->next())
		{
			const int table_id = res->getInt("table_id");

			auto pstmt1 = prepareStatement(conn, "DELETE FROM tableproduct WHERE table_id = ?");
			pstmt1->setInt(1, table_id);
			pstmt1->execute();

			auto pstmt2 = prepareStatement(conn, "DELETE FROM tables WHERE n_table = ? AND bill = ? AND discount = ?");
			pstmt2->setInt(1, n_table);
			pstmt2->setDouble(2, bill);
			pstmt2->setDouble(3, discount);
			pstmt2->execute();

			CROW_LOG_INFO << "[REMOVED] Table with n_table " << n_table <<
				", bill " << bill <<
				" and discount " << discount;
		}
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not remove table. Error message: " << e.what();
	}
}

void Database::removeEmployee(Conn& conn, 
							  const Employee& employee)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		auto pstmt = prepareStatement(conn, "DELETE FROM employees WHERE firstName = ? AND lastName = ? AND level = ?");
		pstmt->setString(1, employee.firstName);
		pstmt->setString(2, employee.lastName);
		pstmt->setInt(3, employee.level);
		pstmt->execute();

		CROW_LOG_INFO << "[REMOVED] Employee with name " << employee.firstName << " " << employee.lastName <<
			"and level " << employee.level;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not remove employee. Error message: " << e.what();
	}
}

void Database::removeProduct(Conn& conn, 
							 const Product& product)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::string name = product.name;
		const double price = product.price;
		const int page = product.page;

		auto pstmt = prepareStatement(conn, "DELETE FROM products WHERE name = ? AND price = ? AND page = ?");
		pstmt->setString(1, name);
		pstmt->setDouble(2, price);
		pstmt->setDouble(3, page);
		pstmt->execute();

		CROW_LOG_INFO << "[REMOVED] Product with name " << name <<
			" price " << price <<
			" on page " << page;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not remove product. Error message: " << e.what();
	}
}

void Database::removeTableProduct(Conn& conn, 
								  const int& n_table,
								  const Product& product,
								  const int& times)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	try
	{
		std::stringstream ss;

		ss << "SELECT * FROM tables WHERE n_table = " << n_table;
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		if (res->next())
		{
			const int table_id = res->getInt("table_id");
			const double bill = res->getDouble("bill");
			const double discount = res->getDouble("discount");

			ss << "SELECT product_id FROM products WHERE name = '" << product.name << "' AND price = " << product.price;
			res = executeStmt(conn, ss.str());
			ss.str("");

			if (res->next())
			{
				const int product_id = res->getInt("product_id");

				auto pstmt1 = prepareStatement(conn, "DELETE FROM tableproduct WHERE table_id = ? AND product_id = ? AND details = ?");
				pstmt1->setInt(1, table_id);
				pstmt1->setInt(2, product_id);
				pstmt1->setString(3, product.details);
				pstmt1->execute();

				double new_bill = bill - (product.price * times * (1.0 - discount));
				if (new_bill < 0) new_bill = 0;

				auto pstmt2 = prepareStatement(conn, "UPDATE tables SET bill = ? WHERE table_id = ?");
				pstmt2->setDouble(1, new_bill);
				pstmt2->setInt(2, table_id);
				pstmt2->execute();
			}
		}

		CROW_LOG_INFO << "[REMOVED] Product(s) from table " << n_table <<
			" with name " << product.name <<
			" and price " << product.price;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not remove tableproduct. Error message: " << e.what();
	}
}

void Database::removeProductIngredient(Conn& conn, 
									   const Product& product,
									   const Ingredient& ingredient)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	try
	{
		std::stringstream ss;

		ss << "SELECT product_id FROM products WHERE name = '" << product.name << "' AND price = '" << product.price << "'";
		auto res = executeStmt(conn, ss.str());
		ss.str("");

		if (res->next())
		{
			const int product_id = res->getInt("product_id");

			ss << "SELECT ingredient_id FROM ingredients WHERE name = '" << ingredient.name << "'";
			res = executeStmt(conn, ss.str());
			ss.str("");

			if (res->next())
			{
				const int ingredient_id = res->getInt("ingredient_id");

				auto pstmt = prepareStatement(conn, "DELETE FROM productingredient WHERE product_id = ? AND ingredient_id = ?");
				pstmt->setInt(1, product_id);
				pstmt->setInt(2, ingredient_id);
				pstmt->execute();
			}
		}

		CROW_LOG_INFO << "[REMOVED] Ingredient(s) from product " << product.name <<
			" with price " << product.price;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not remove productingredient. Error message: " << e.what();
	}
}

void Database::removeProductIngredients(Conn& conn, 
										const Product& product)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	////std::unique_lock<std::shared_mutex> lock(mutex);

	std::stringstream ss;

	ss << "SELECT product_id FROM products WHERE name = '" << product.name << "' AND price = '" << product.price << "'";
	auto res = executeStmt(conn, ss.str());
	ss.str("");

	if (res->next())
	{
		const int product_id = res->getInt("product_id");

		auto pstmt = prepareStatement(conn, "DELETE FROM productingredient WHERE product_id = ?");
		pstmt->setInt(1, product_id);
		pstmt->execute();
	}
}

void Database::removeProductAllergens(Conn& conn, 
									  const Product& product)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	////std::unique_lock<std::shared_mutex> lock(mutex);

	std::stringstream ss;

	ss << "SELECT product_id FROM products WHERE name = '" << product.name << "' AND price = '" << product.price << "'";
	auto res = executeStmt(conn, ss.str());
	ss.str("");

	if (res->next())
	{
		const int product_id = res->getInt("product_id");

		auto pstmt = prepareStatement(conn, "DELETE FROM productallergen WHERE product_id = ?");
		pstmt->setInt(1, product_id);
		pstmt->execute();
	}
}

void Database::removeOrder(Conn& conn, 
						   const Order& order)
{
	/*
	CROW_LOG_DEBUG << "[DB] " << __func__;

	try {
		std::string time = order.time;
		std::string message = order.message;

		auto pstmt = prepareStatement("DELETE FROM orders WHERE time = ? AND message = ?");
		pstmt->setString(1, time);
		pstmt->setString(2, message);
		pstmt->execute();

		CROW_LOG_INFO << "[REMOVED] Order with time " << time <<
			" and message " << message <<
			" has been deleted.";
	}
	catch (const sql::SQLException& e) {
		CROW_LOG_WARNING << "[EXCEPTION] Could not remove order. Error message: " << e.what();
	}*/
}

void Database::removeIngredient(Conn& conn, 
								const Ingredient& ingredient)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::string name = ingredient.name;

		auto pstmt = prepareStatement(conn, "DELETE FROM ingredients WHERE name = ?");
		pstmt->setString(1, name);
		pstmt->execute();

		CROW_LOG_INFO << "[REMOVED] Ingredient with name " << name;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not remove ingredient. Error message: " << e.what();
	}
}

void Database::removeAllergen(Conn& conn, 
							  const Allergen& allergen)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::string name = allergen.name;

		auto pstmt = prepareStatement(conn, "DELETE FROM allergens WHERE name = ?");
		pstmt->setString(1, name);
		pstmt->execute();

		CROW_LOG_INFO << "[REMOVED] Allergen with name " << name;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not remove allergen. Error message: " << e.what();
	}
}


// Modify
void Database::modifyProduct(Conn& conn, 
							 const Product& oldProduct,
							 const Product& newProduct)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		const std::string selectedElementName = oldProduct.name;
		const double selectedElementPrice = oldProduct.price;
		const std::string selectedElementColor = oldProduct.color;
		const int selectedElementPage = oldProduct.page;
		const bool selectedElementDeployable = oldProduct.deployable;

		const std::string newName = newProduct.name;
		const double newPrice = newProduct.price;
		const std::string newColor = newProduct.color;

		std::stringstream ss;
		ss << "UPDATE products SET name = ?, price = ?, color = ? WHERE name = '" << selectedElementName << "' AND price = '" << selectedElementPrice << "' AND page = '" << selectedElementPage << "'";
		auto pstmt = prepareStatement(conn, ss.str());
		ss.str("");
		pstmt->setString(1, newName);
		pstmt->setDouble(2, newPrice);
		pstmt->setString(3, newColor);
		pstmt->execute();
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not modify product. Error message: " << e.what();
	}
}

void Database::changeNumClients(Conn& conn, 
								const Table& table,
								const int& n_clients)
{
	CROW_LOG_DEBUG << "[DB] " << __func__;

	try
	{
		////std::unique_lock<std::shared_mutex> lock(mutex);

		std::stringstream ss;
		ss << "UPDATE tables SET n_clients = ? WHERE n_table = ?";
		auto pstmt = prepareStatement(conn, ss.str());
		ss.str("");
		pstmt->setInt(1, n_clients);
		pstmt->setInt(2, table.n_table);
		pstmt->execute();
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not change number of clients. Error message: " << e.what();
	}
}