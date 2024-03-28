#include "..\headers\database.hpp"
#include "..\headers\crow_all.hpp"
#include "..\headers\domain.hpp"

Database::Database() : pstmt()
{
	try
	{
		connection_properties = {
			{"hostName", HOST},
			{"port", PORT},
			{"userName", USERNAME},
			{"password", PASSWORD},
			{"schema", DATABASE_NAME},
			{"OPT_RECONNECT", true}
		};

		driver = get_driver_instance();
		con = static_cast<std::shared_ptr<sql::Connection>>(driver->connect(connection_properties));
		stmt = con->createStatement();
	}
	catch (const sql::SQLException e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not construct database." << e.what();
		exit(1);
	}
}

Database::Database(const Database& database)
{
	connection_properties = database.connection_properties;

	driver = database.driver;

	con = static_cast<std::shared_ptr<sql::Connection>>(driver->connect(connection_properties));

	stmt = database.stmt;

	pstmt = database.pstmt;

}

Database::Database(const std::shared_ptr<Database> database)
{
	connection_properties = database->connection_properties;

	driver = database->driver;

	con = static_cast<std::shared_ptr<sql::Connection>>(driver->connect(connection_properties));

	stmt = database->stmt;

	pstmt = database->pstmt;

}

Database::~Database()
{
	con->close();

	// These have to be explicitly deleted. con is shared_ptr so it doesnt need to
	delete stmt;
	delete pstmt;
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
		Ingredient ingredient = { name };
		saveIngredient(ingredient);
	}
}

void Database::initializeAllergensTable()
{
	std::vector<std::string> allergenNames = { "Gluten", "Peanuts", "Dairy" };

	for (const auto& name : allergenNames)
	{
		Allergen allergen = { name };
		saveAllergen(allergen);
	}
}

void Database::initialize()
{
	CROW_LOG_INFO << "[DB] initialize";

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
	CROW_LOG_INFO << "[DB] dropAllTables";

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		// We must drop first the tables that have a reference to another
		stmt->execute("DROP TABLE IF EXISTS tableproduct;");
		stmt->execute("DROP TABLE IF EXISTS orderproduct;");
		stmt->execute("DROP TABLE IF EXISTS productingredient;");
		stmt->execute("DROP TABLE IF EXISTS productorder;");

		stmt->execute("DROP TABLE IF EXISTS orders;");
		stmt->execute("DROP TABLE IF EXISTS ingredients;");
		stmt->execute("DROP TABLE IF EXISTS allergens;");
		stmt->execute("DROP TABLE IF EXISTS employees;");
		stmt->execute("DROP TABLE IF EXISTS products;");
		stmt->execute("DROP TABLE IF EXISTS tables;");
		stmt->execute("DROP TABLE IF EXISTS restaurants;");

		CROW_LOG_INFO << "[REMOVED] All tables dropped.";
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not drop tables. Error message: " << e.what();
	}
}


// Save
void Database::saveTable(const Table& table)
{
	CROW_LOG_INFO << "[DB] saveTable";

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		pstmt = con->prepareStatement("INSERT INTO tables(n_table, n_clients, bill, discount) VALUES(?,?,?,?)");
		pstmt->setInt(1, table.n_table);
		pstmt->setInt(2, table.n_clients);
		pstmt->setDouble(3, table.bill);
		pstmt->setDouble(4, table.discount);
		pstmt->execute();

		CROW_LOG_INFO << "[ADDED] Table " << table.n_table <<
			" with " << table.n_clients <<
			" clients inserted into tables.";
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not save table. Error message: " << e.what();
	}
}

void Database::saveEmployee(const Employee& oldEmployee, const Employee& newEmployee)
{
	CROW_LOG_INFO << "[DB] saveEmployee";

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		const std::string oldEmployeeFullName = oldEmployee.firstName + " " + oldEmployee.lastName;
		const std::string newEmployeeFullName = oldEmployee.firstName + " " + oldEmployee.lastName;

		if (getEmployeeByName(oldEmployeeFullName).isEmpty() && getEmployeeByName(newEmployeeFullName).isEmpty())
		{
			pstmt = con->prepareStatement("INSERT INTO employees(firstName, lastName, email, id, mobileNumber, level, username, password, session_token) VALUES(?,?,?,?,?,?,?,?,?)");
			pstmt->setString(1, newEmployee.firstName);
			pstmt->setString(2, newEmployee.lastName);
			pstmt->setString(3, newEmployee.email);
			pstmt->setString(4, newEmployee.id);
			pstmt->setString(5, newEmployee.mobileNumber);
			pstmt->setInt(6, newEmployee.level);
			pstmt->setString(7, newEmployee.username);
			pstmt->setString(8, newEmployee.password_hash);
			pstmt->setString(9, generateSessionToken());
			pstmt->execute();

			CROW_LOG_INFO << "[ADDED] Employee " << newEmployee.firstName << " " << newEmployee.lastName <<
				" with level " << newEmployee.level;
		}
		else if (!getEmployeeByName(oldEmployeeFullName).isEmpty())
		{
			pstmt = con->prepareStatement("UPDATE employees SET firstName = ?, lastName = ?, email = ?, id = ?, mobileNumber = ?, level = ?, username = ?, password = ?, session_token = ? WHERE firstName = ? AND lastName = ? AND level = ?");
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
			pstmt->execute();

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

void Database::saveProduct(const Product& product)
{
	CROW_LOG_INFO << "[DB] saveProduct";

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		std::string name = product.name;
		double price = product.price;
		std::string color = product.color;
		int page = product.page;
		int deployable = product.deployable;

		if (getProductByName(name).isEmpty())
		{
			pstmt = con->prepareStatement("INSERT INTO products(name, price, color, page, deployable) VALUES(?,?,?,?,?)");
			pstmt->setString(1, name);
			pstmt->setDouble(2, price);
			pstmt->setString(3, color);
			pstmt->setInt(4, page);
			pstmt->setInt(5, deployable);
			pstmt->execute();

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

void Database::saveOrder(const Order& order)
{
	CROW_LOG_INFO << "[DB] saveOrder";

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		pstmt = con->prepareStatement("INSERT INTO orders(n_table, n_clients, bill, paid, discount, method, employee, date) VALUES(?,?,?,?,?,?,?,?)");
		pstmt->setInt(1, order.n_table);
		pstmt->setInt(2, order.n_clients);
		pstmt->setDouble(3, order.bill);
		pstmt->setDouble(4, order.paid);
		pstmt->setDouble(5, order.discount);
		pstmt->setString(6, order.method);
		pstmt->setString(7, order.employee);
		pstmt->setString(8, order.date);
		pstmt->execute();

		std::stringstream query;
		query << "SELECT table_id FROM tables WHERE n_table = '" << order.n_table << "'";
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		if (res->next())
		{
			int table_id = res->getInt("table_id");

			query << "SELECT * FROM tableproduct WHERE table_id = '" << table_id << "'";
			res = stmt->executeQuery(query.str());
			query.str("");

			while (res->next())
			{
				int product_id = res->getInt("product_id");
				int amount = res->getInt("amount");

				saveOrderProduct(order, product_id, amount);
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
void Database::saveOrderProduct(const Order& order, const int& product_id, const int& amount)
{
	CROW_LOG_INFO << "[DB] saveOrderProduct";

	try
	{
		//std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;
		query << "SELECT order_id FROM orders WHERE n_table = '" << order.n_table << "' AND n_clients = '" << order.n_clients << "' AND bill = '" << order.bill << "' AND discount = '" << order.discount << "' AND employee = '" << order.employee << "' AND date = '" << order.date << "'";
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		if (res->next())
		{
			int order_id = res->getInt("order_id");

			pstmt = con->prepareStatement("INSERT INTO orderproduct(order_id, product_id, amount) VALUES(?,?,?)");
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

void Database::saveIngredient(const Ingredient& ingredient)
{
	CROW_LOG_INFO << "[DB] saveIngredient";

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		if (getIngredientByName(ingredient.name).isEmpty())
		{
			pstmt = con->prepareStatement("INSERT INTO ingredients(name) VALUES(?)");
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

void Database::saveAllergen(const Allergen& allergen)
{
	CROW_LOG_INFO << "[DB] saveAllergen";

	try
	{
		//std::unique_lock<std::mutex> lock(mutex);

		if (getAllergenByName(allergen.name).isEmpty())
		{
			pstmt = con->prepareStatement("INSERT INTO allergens(name) VALUES(?)");
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
void Database::saveTableProduct(Table& table,
								const Product& product,
								const int& amount,
								const std::string& details,
								const Employee& employee)
{
	CROW_LOG_INFO << "[DB] saveTableProduct";

	try
	{
		//std::unique_lock<std::mutex> lock(mutex);

		int table_id = 0;
		int n_table = table.n_table;
		//double discount = table.discount;

		int product_id = 0;
		std::string name = product.name;

		std::stringstream query;

		query << "SELECT * FROM tables WHERE n_table = " << n_table;
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		if (res->next())
		{
			table_id = res->getInt("table_id");
			double new_bill = res->getDouble("bill") + (product.price * amount);
			const std::string employeeName = employee.firstName + " " + getLastNameInitials(employee.lastName);

			pstmt = con->prepareStatement("UPDATE tables SET bill = ?, last_modified = ? WHERE table_id = ?");
			pstmt->setDouble(1, new_bill);
			pstmt->setString(2, employeeName);
			pstmt->setInt(3, table_id);
			pstmt->execute();

			CROW_LOG_INFO << "[UPDATED] Table " << table.n_table <<
				" updated bill is " << new_bill;

			query << "SELECT product_id FROM products WHERE name = '" << name << "'";
			res = stmt->executeQuery(query.str());
			query.str("");

			if (res->next())
			{
				product_id = res->getInt("product_id");

				query << "SELECT amount FROM tableproduct WHERE table_id = '" << table_id << "' AND product_id = '" << product_id << "' AND details = '" << details << "'";
				res = stmt->executeQuery(query.str());
				query.str("");

				if (res->next())
				{
					int new_amount = res->getInt("amount") + amount;

					pstmt = con->prepareStatement("UPDATE tableproduct SET amount = ? WHERE table_id = ? AND product_id = ? AND details = ?");
					pstmt->setInt(1, new_amount);
					pstmt->setInt(2, table_id);
					pstmt->setInt(3, product_id);
					pstmt->setString(4, details);

					pstmt->execute();

					CROW_LOG_INFO << "[UPDATED] Tableproduct with table_id " << table_id <<
						" and product_id " << product_id <<
						"and details " << details <<
						" to amount " << new_amount;
				}
				else
				{
					pstmt = con->prepareStatement("INSERT INTO tableproduct(table_id, product_id, amount, details) VALUES(?,?,?,?)");
					pstmt->setInt(1, table_id);
					pstmt->setInt(2, product_id);
					pstmt->setInt(3, amount);
					pstmt->setString(4, details);
					pstmt->execute();

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
void Database::saveProductIngredient(const Product& product,
									 const Ingredient& ingredient)
{
	CROW_LOG_INFO << "[DB] saveProductIngredient";

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;

		query << "SELECT product_id FROM products WHERE name = '" << product.name << "' AND price = '" << product.price << "'";
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		if (res->next())
		{
			int product_id = res->getInt("product_id");

			query << "SELECT ingredient_id FROM ingredients WHERE name = '" << ingredient.name << "'";
			res = stmt->executeQuery(query.str());
			query.str("");

			if (res->next())
			{
				int ingredient_id = res->getInt("ingredient_id");

				pstmt = con->prepareStatement("INSERT INTO productingredient(product_id, ingredient_id) VALUES(?,?)");
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

void Database::saveProductAllergen(const Product& product, const Allergen& allergen)
{
	CROW_LOG_INFO << "[DB] saveProductAllergen";

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;

		query << "SELECT product_id FROM products WHERE name = '" << product.name << "' AND price = '" << product.price << "'";
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		if (res->next())
		{
			int product_id = res->getInt("product_id");

			query << "SELECT allergen_id FROM allergens WHERE name = '" << allergen.name << "'";
			res = stmt->executeQuery(query.str());
			query.str("");

			if (res->next())
			{
				int allergen_id = res->getInt("allergen_id");

				pstmt = con->prepareStatement("INSERT INTO productallergen(product_id, allergen_id) VALUES(?,?)");
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


//Get
std::vector<Table> Database::getTables()
{
	CROW_LOG_INFO << "[DB] getTables";

	std::vector<Table> tables;

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		sql::ResultSet* res = stmt->executeQuery("SELECT * FROM tables"); // TODO: change tables for a varible that corresponds to the table name

		while (res->next())
		{
			Table table;

			int id = res->getInt("table_id");
			table.n_table = res->getInt("n_table");
			// TODO: Add std::unordered_map<std::string, int> products
			table.bill = res->getDouble("bill");
			table.discount = res->getDouble("discount");

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

Table Database::getTableByNumber(const int n_table)
{
	CROW_LOG_INFO << "[DB] getTableByNumber";

	Table table;

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;

		query << "SELECT * FROM tables WHERE n_table = " << n_table; // TODO: change tables for a varible that corresponds to the table name
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		if (res->next())
		{
			int table_id = res->getInt("table_id");
			int n = res->getInt("n_table");
			int n_clients = res->getInt("n_clients");
			double bill = res->getDouble("bill");
			double discount = res->getDouble("discount");

			query << "SELECT * FROM tableproduct WHERE table_id = " << table_id;
			res = stmt->executeQuery(query.str());
			query.str("");

			product_unordered_map products;
			while (res->next())
			{
				int product_id = res->getInt("product_id");
				int amount = res->getInt("amount");
				std::string details = res->getString("details");

				query << "SELECT * FROM products WHERE product_id = " << product_id;
				sql::ResultSet* res2 = stmt->executeQuery(query.str());
				query.str("");

				if (res2->next())
				{
					Product p(res2->getString("name"), res2->getDouble("price"), res2->getString("color"), res2->getInt("page"), res2->getBoolean("deployable"), details);

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

std::string Database::getLastModifiedFromTable(const Table& table)
{
	CROW_LOG_INFO << "[DB] getlastModifiedFromTable";

	std::string last_modified;

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;

		query << "SELECT last_modified FROM tables WHERE n_table = " << table.n_table; // TODO: change tables for a varible that corresponds to the table name
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

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


std::vector<Employee> Database::getEmployees()
{
	CROW_LOG_INFO << "[DB] getEmployees";

	std::vector<Employee> employees;

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		sql::ResultSet* res = stmt->executeQuery("SELECT * FROM employees"); // TODO: change employees for a varible that corresponds to the table name

		while (res->next())
		{
			std::string firstName = res->getString("firstName");
			std::string lastName = res->getString("lastName");
			std::string email = res->getString("email");
			std::string id = res->getString("id");
			std::string mobileNumber = res->getString("mobileNumber");
			int level = res->getInt("level");
			std::string username = res->getString("username");
			std::string password_hash = res->getString("password");
			std::string session_token = res->getString("session_token");

			Employee employee = { firstName, lastName, email, id, mobileNumber, level, username, password_hash, session_token };

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

Employee Database::getEmployeeByName(const std::string& fullName)
{
	CROW_LOG_INFO << "[DB] getEmployeeByName";

	Employee employee;

	try
	{
		//std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;
		query << "SELECT * FROM employees WHERE CONCAT(firstName, ' ', lastName) = '" << fullName << "'"; // String needs to be inside '' // TODO: change employees for a varible that corresponds to the table name
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

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

Employee Database::getEmployeeByAccount(const std::string& username,
										const std::string& password_hash)
{
	CROW_LOG_INFO << "[DB] getEmployeeByAccount";

	Employee employee;

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;
		query << "SELECT * FROM employees WHERE username = '" << username << "' AND password ='" << password_hash << "'";
		sql::ResultSet* res = stmt->executeQuery(query.str());

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

Employee Database::getEmployeeBySessionToken(const std::string& session_token)
{
	CROW_LOG_INFO << "[DB] getEmployeeBySessionToken";

	Employee employee;

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;
		query << "SELECT * FROM employees WHERE session_token = '" << session_token << "'";
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

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

std::vector<Product> Database::getProducts()
{
	CROW_LOG_INFO << "[DB] getProducts";

	std::vector<Product> products;

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		sql::ResultSet* res = stmt->executeQuery("SELECT * FROM products"); // TODO: change employees for a varible that corresponds to the table name

		while (res->next())
		{
			//int id = res->getInt("product_id");
			std::string name = res->getString("name");
			double price = res->getDouble("price");
			std::string color = res->getString("color");
			int page = res->getInt("page");
			bool deployable = res->getBoolean("deployable");

			Product product = { name, price, color, page, deployable };

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

Product Database::getProductByName(const std::string name)
{
	CROW_LOG_INFO << "[DB] getProductByName";

	Product product;
	try
	{
		//std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;
		query << "SELECT * FROM products WHERE name = '" << name << "'";
		sql::ResultSet* res = stmt->executeQuery(query.str());

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

int Database::getProductIdByName(const std::string name)
{
	CROW_LOG_INFO << "[DB] getProductIdByName";

	int id = 0;

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;
		query << "SELECT product_id FROM products WHERE name = '" << name << "'";
		sql::ResultSet* res = stmt->executeQuery(query.str());

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

std::vector<Product> Database::getProductsByDeployableId(const int& deployable_id)
{
	CROW_LOG_INFO << "[DB] getProductsByDeployableId";

	std::vector<Product> products;

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;
		query << "SELECT * FROM products WHERE deployable = '" << deployable_id << "'";
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		while (res->next())
		{
			std::string name = res->getString("name");
			double price = res->getDouble("price");
			std::string color = res->getString("color");
			int page = res->getInt("page");
			int deployable = res->getInt("deployable");

			Product p(name, price, color, page, deployable);
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

std::pair<int, std::vector<Product>> Database::getProductsAndIds()
{
	CROW_LOG_INFO << "[DB] getProductsAndIds";

	std::unique_lock<std::mutex> lock(mutex);

	std::vector<Product> products = getProducts();

	return std::pair<int, std::vector<Product>>();
}

std::vector<Order> Database::getOrders()
{
	CROW_LOG_INFO << "[DB] getOrders";

	std::vector<Order> orders;

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;
		query << "SELECT * FROM orders";
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		while (res->next())
		{
			int order_id = res->getInt("order_id");
			int n_table = res->getInt("n_table");
			int n_clients = res->getInt("n_clients");
			double bill = res->getDouble("bill");
			double paid = res->getDouble("paid");
			double discount = res->getDouble("discount");
			std::string method = res->getString("method");
			std::string employee = res->getString("employee");
			std::string date = res->getString("date");

			query << "SELECT * from orderproduct WHERE order_id = '" << order_id << "'";
			sql::ResultSet* res2 = stmt->executeQuery(query.str());
			query.str("");

			std::vector<std::pair<Product, int>> products;
			while (res2->next())
			{
				int product_id = res2->getInt("product_id");
				int amount = res2->getInt("amount");

				query << "SELECT * from products WHERE product_id = '" << product_id << "'";
				sql::ResultSet* res3 = stmt->executeQuery(query.str());
				query.str("");

				if (res3->next())
				{
					std::string name = res3->getString("name");
					double price = res3->getDouble("price");
					std::string color = res3->getString("color");
					int page = res3->getInt("page");
					int deployable = res3->getInt("deployable");

					Product p(name, price, color, page, deployable);
					products.push_back({ p, amount });
				}
			}

			Order o = { order_id, n_table, n_clients, bill, paid, discount, method, products, employee, date };
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
		//std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;
		query << "SELECT * FROM orders WHERE time = '" << time << "'";
		sql::ResultSet* res = stmt->executeQuery(query.str());

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

std::vector<Order> Database::getOrdersByDate(const std::string& date, const std::string& mode)
{
	CROW_LOG_INFO << "[DB] getOrdersByDate";

	std::vector<Order> orders;

	try
	{
		CROW_LOG_INFO << "[DB] getOrdersByDate " << date << ".";
		//std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;
		query << "SELECT * FROM orders WHERE " << mode << "(date) = '" << date << "'";
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		while (res->next())
		{
			int order_id = res->getInt("order_id");
			int n_table = res->getInt("n_table");
			int n_clients = res->getInt("n_clients");
			double bill = res->getDouble("bill");
			double paid = res->getDouble("paid");
			double discount = res->getDouble("discount");
			std::string method = res->getString("method");
			std::string employee = res->getString("employee");
			std::string date = res->getString("date");

			query << "SELECT * from orderproduct WHERE order_id = '" << order_id << "'";
			sql::ResultSet* res2 = stmt->executeQuery(query.str());
			query.str("");

			std::vector<std::pair<Product, int>> products;
			while (res2->next())
			{
				int product_id = res2->getInt("product_id");
				int amount = res2->getInt("amount");

				query << "SELECT * from products WHERE product_id = '" << product_id << "'";
				sql::ResultSet* res3 = stmt->executeQuery(query.str());
				query.str("");

				if (res3->next())
				{
					std::string name = res3->getString("name");
					double price = res3->getDouble("price");
					std::string color = res3->getString("color");
					int page = res3->getInt("page");
					int deployable = res3->getInt("deployable");

					Product p(name, price, color, page, deployable);
					products.push_back({ p, amount });
				}
			}

			Order o = { order_id, n_table, n_clients, bill, paid, discount, method, products, employee, date };
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

std::vector<Order> Database::getOrdersByEmployee(const std::string& employeeName)
{
	CROW_LOG_INFO << "[DB] getOrdersByEmployee";

	std::vector<Order> orders;

	try
	{
		//std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;
		query << "SELECT * FROM orders WHERE employee = '" << employeeName << "'";
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		while (res->next())
		{
			int order_id = res->getInt("order_id");
			int n_table = res->getInt("n_table");
			int n_clients = res->getInt("n_clients");
			double bill = res->getDouble("bill");
			double paid = res->getDouble("paid");
			double discount = res->getDouble("discount");
			std::string method = res->getString("method");
			std::string employee = res->getString("employee");
			std::string date = res->getString("date");

			query << "SELECT * from orderproduct WHERE order_id = '" << order_id << "'";
			sql::ResultSet* res2 = stmt->executeQuery(query.str());
			query.str("");

			std::vector<std::pair<Product, int>> products;
			while (res2->next())
			{
				int product_id = res2->getInt("product_id");
				int amount = res2->getInt("amount");

				query << "SELECT * from products WHERE product_id = '" << product_id << "'";
				sql::ResultSet* res3 = stmt->executeQuery(query.str());
				query.str("");

				if (res3->next())
				{
					std::string name = res3->getString("name");
					double price = res3->getDouble("price");
					std::string color = res3->getString("color");
					int page = res3->getInt("page");
					int deployable = res3->getInt("deployable");

					Product p(name, price, color, page, deployable);
					products.push_back({ p, amount });
				}
			}

			Order o = { order_id, n_table, n_clients, bill, paid, discount, method, products, employee, date };
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

std::vector<Order> Database::getOrdersByMethod(const std::string& method)
{
	CROW_LOG_INFO << "[DB] getOrdersByMethod";

	std::vector<Order> orders;

	try
	{
		//std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;
		query << "SELECT * FROM orders WHERE method = '" << method << "'";
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		while (res->next())
		{
			int order_id = res->getInt("order_id");
			int n_table = res->getInt("n_table");
			int n_clients = res->getInt("n_clients");
			double bill = res->getDouble("bill");
			double paid = res->getDouble("paid");
			double discount = res->getDouble("discount");
			std::string method = res->getString("method");
			std::string employee = res->getString("employee");
			std::string date = res->getString("date");

			query << "SELECT * from orderproduct WHERE order_id = '" << order_id << "'";
			sql::ResultSet* res2 = stmt->executeQuery(query.str());
			query.str("");

			std::vector<std::pair<Product, int>> products;
			while (res2->next())
			{
				int product_id = res2->getInt("product_id");
				int amount = res2->getInt("amount");

				query << "SELECT * from products WHERE product_id = '" << product_id << "'";
				sql::ResultSet* res3 = stmt->executeQuery(query.str());
				query.str("");

				if (res3->next())
				{
					std::string name = res3->getString("name");
					double price = res3->getDouble("price");
					std::string color = res3->getString("color");
					int page = res3->getInt("page");
					int deployable = res3->getInt("deployable");

					Product p(name, price, color, page, deployable);
					products.push_back({ p, amount });
				}
			}

			Order o = { order_id, n_table, n_clients, bill, paid, discount, method, products, employee, date };
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

std::vector<Order> Database::getOrdersByNTable(const std::string& n_table)
{
	CROW_LOG_INFO << "[DB] getOrdersByNTable";

	int nTable = std::stoi(n_table);
	std::vector<Order> orders;

	try
	{
		//std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;
		query << "SELECT * FROM orders WHERE n_table = '" << nTable << "'";
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		while (res->next())
		{
			int order_id = res->getInt("order_id");
			int n_table = res->getInt("n_table");
			int n_clients = res->getInt("n_clients");
			double bill = res->getDouble("bill");
			double paid = res->getDouble("paid");
			double discount = res->getDouble("discount");
			std::string method = res->getString("method");
			std::string employee = res->getString("employee");
			std::string date = res->getString("date");

			query << "SELECT * from orderproduct WHERE order_id = '" << order_id << "'";
			sql::ResultSet* res2 = stmt->executeQuery(query.str());
			query.str("");

			std::vector<std::pair<Product, int>> products;
			while (res2->next())
			{
				int product_id = res2->getInt("product_id");
				int amount = res2->getInt("amount");

				query << "SELECT * from products WHERE product_id = '" << product_id << "'";
				sql::ResultSet* res3 = stmt->executeQuery(query.str());
				query.str("");

				if (res3->next())
				{
					std::string name = res3->getString("name");
					double price = res3->getDouble("price");
					std::string color = res3->getString("color");
					int page = res3->getInt("page");
					int deployable = res3->getInt("deployable");

					Product p(name, price, color, page, deployable);
					products.push_back({ p, amount });
				}
			}

			Order o = { order_id, n_table, n_clients, bill, paid, discount, method, products, employee, date };
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


std::vector<BillAndPaid> Database::getBillsAndPaids()
{
	CROW_LOG_INFO << "[DB] getBillsAndPaids";

	std::vector<BillAndPaid> billsAndPaids;

	try
	{
		//std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;
		query << "SELECT bill, paid FROM orders";
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		while (res->next())
		{
			double bill = res->getDouble("bill");
			double paid = res->getDouble("paid");
			std::string date = res->getString("date");

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

std::vector<BillAndPaid> Database::getBillsAndPaidsByDate(const std::string& date, const std::string& mode)
{
	CROW_LOG_INFO << "[DB] getBillsAndPaidsByDate";

	std::vector<BillAndPaid> billsAndPaids;

	try
	{
		//std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;
		query << "SELECT bill, paid, date FROM orders WHERE " << mode << "(date) = " << date;
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		while (res->next())
		{
			double bill = res->getDouble("bill");
			double paid = res->getDouble("paid");
			std::string date = res->getString("date");

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

std::vector<BillAndPaid> Database::getBillsAndPaidsByEmployee(const std::string& employeeName)
{
	CROW_LOG_INFO << "[DB] getBillsAndPaidsByEmployee";

	std::vector<BillAndPaid> billsAndPaids;

	try
	{
		//std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;
		query << "SELECT bill, paid, date FROM orders WHERE employee = '" << employeeName << "'";
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		while (res->next())
		{
			double bill = res->getDouble("bill");
			double paid = res->getDouble("paid");
			std::string date = res->getString("date");

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

int Database::getNClients()
{
	CROW_LOG_INFO << "[DB] getNClients";

	int totalNClients = 0;

	try
	{
		//std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;
		query << "SELECT n_clients FROM orders";
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		while (res->next())
		{
			int n_clients = res->getInt("n_clients");

			totalNClients += n_clients;
		}

		return totalNClients;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get number of clients. Error message: " << e.what();

		return totalNClients;
	}
}

int Database::getNClientsByDate(const std::string& date, const std::string& mode)
{
	CROW_LOG_INFO << "[DB] getNClientsByDate";

	int totalNClients = 0;

	try
	{
		//std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;
		query << "SELECT n_clients FROM orders WHERE " << mode << "(date) = '" << date << "'";
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		while (res->next())
		{
			int n_clients = res->getInt("n_clients");

			totalNClients += n_clients;
		}

		return totalNClients;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get number of clients by date. Error message: " << e.what();

		return totalNClients;
	}
}

int Database::getNClientsByEmployee(const std::string& employeeName)
{
	CROW_LOG_INFO << "[DB] getNClientsByEmployee";

	int totalNClients = 0;

	try
	{
		//std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;
		query << "SELECT n_clients FROM orders WHERE employee = '" << employeeName << "'";
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		while (res->next())
		{
			int n_clients = res->getInt("n_clients");

			totalNClients += n_clients;
		}

		return totalNClients;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get number of clients by employee. Error message: " << e.what();

		return totalNClients;
	}
}

std::vector<Ingredient> Database::getIngredients()
{
	CROW_LOG_INFO << "[DB] getIngredients";

	std::vector<Ingredient> ingredients;

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		sql::ResultSet* res = stmt->executeQuery("SELECT * FROM ingredients");

		while (res->next())
		{
			std::string name = res->getString("name");
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

Ingredient Database::getIngredientByName(const std::string& name)
{
	CROW_LOG_INFO << "[DB] getIngredientsByName";

	Ingredient ingredient;

	try
	{
		//std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;
		query << "SELECT * FROM ingredients WHERE name = '" << name << "'";
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

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

std::vector<Ingredient> Database::getIngredientsFromProduct(const Product& product)
{
	CROW_LOG_INFO << "[DB] getIngredientsFromProduct";

	std::vector<Ingredient> ingredients;

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;
		query << "SELECT product_id FROM products WHERE name = '" << product.name << "' AND price = '" << product.price << "' AND page = '" << product.page << "'";
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		if (res->next())
		{
			int product_id = res->getInt("product_id");

			query << "SELECT ingredient_id FROM productingredient WHERE product_id = " << product_id;
			res = stmt->executeQuery(query.str());
			query.str("");

			while (res->next())
			{
				int ingredient_id = res->getInt("ingredient_id");

				query << "SELECT * FROM ingredients WHERE ingredient_id = " << ingredient_id;
				sql::ResultSet* res2 = stmt->executeQuery(query.str());
				query.str("");

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

std::vector<Allergen> Database::getAllergens()
{
	CROW_LOG_INFO << "[DB] getAllergens";

	std::vector<Allergen> allergens;

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		sql::ResultSet* res = stmt->executeQuery("SELECT * FROM allergens");

		while (res->next())
		{
			std::string name = res->getString("name");
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

Allergen Database::getAllergenByName(const std::string name)
{
	CROW_LOG_INFO << "[DB] getAllergensByName";

	Allergen allergen;

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;
		query << "SELECT * FROM allergens WHERE name = '" << name << "'";
		sql::ResultSet* res = stmt->executeQuery(query.str());

		if (res->next())
		{
			std::string name = res->getString("name");
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

std::vector<Allergen> Database::getAllergensFromProduct(const Product& product)
{
	CROW_LOG_INFO << "[DB] getAllergensFromProduct";

	std::vector<Allergen> allergens;

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;
		query << "SELECT product_id FROM products WHERE name = '" << product.name << "' AND price = '" << product.price << "' AND page = '" << product.page << "'";
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		if (res->next())
		{
			int product_id = res->getInt("product_id");

			query << "SELECT allergen_id FROM productallergen WHERE product_id = " << product_id;
			res = stmt->executeQuery(query.str());
			query.str("");

			while (res->next())
			{
				int allergen_id = res->getInt("allergen_id");

				query << "SELECT * FROM allergens WHERE allergen_id = " << allergen_id;
				sql::ResultSet* res2 = stmt->executeQuery(query.str());
				query.str("");

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

std::vector<page_t> Database::getDataFromPages()
{
	CROW_LOG_INFO << "[DB] getDataFromPages";

	std::vector<page_t> pages(N_FOURTH_ROW_BUTTONS);
	int i = 0;

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		for (auto& p : pages)
		{
			std::stringstream query;
			query << "SELECT * FROM products WHERE page = '" << i + 1 << "'";
			sql::ResultSet* res = stmt->executeQuery(query.str());
			query.str("");

			while (res->next())
			{
				std::string name = res->getString("name");
				double price = res->getDouble("price");
				std::string color = res->getString("color");
				int page = res->getInt("page");
				bool deployable = res->getBoolean("deployable");

				Product p1(name, price, color, page, deployable);
				std::vector<Product> deployable_vector;

				if (deployable == 0)
				{
					if (price == 0)
					{
						query << "SELECT product_id FROM products WHERE name = '" << name << "' AND price = 0 AND color = '" << color << "' AND page = '" << page << "' AND deployable = 0";
						sql::ResultSet* res2 = stmt->executeQuery(query.str());
						query.str("");

						if (res2->next())
						{
							int deployable_id = res->getInt("product_id");

							query << "SELECT * FROM products WHERE deployable = '" << deployable_id << "'";
							res2 = stmt->executeQuery(query.str());
							query.str("");

							deployable_vector = { Product("", 0.0, "#FFFFFF", 0, 0) };
							while (res2->next())
							{
								name = res2->getString("name");
								price = res2->getDouble("price");
								color = res2->getString("color");
								page = res2->getInt("page");
								deployable = res2->getBoolean("deployable");

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
	CROW_LOG_INFO << "[DB] generateSessionToken";

	std::random_device rd;
	std::mt19937 generator(rd());

	std::uniform_int_distribution<int> distribution(0, ASCII_CHARACTERS.size() - 1);

	std::stringstream ss;
	for (int i = 0; i < SESSION_TOKEN_LENGTH; ++i)
	{
		ss << ASCII_CHARACTERS[distribution(generator)];
	}

	std::string new_session_token = ss.str();

	return new_session_token;
}


// Print
void Database::printTables()
{
	CROW_LOG_INFO << "[DB] printTables";

	std::vector<Table> tables = getTables();

	CROW_LOG_INFO << "[LIST] Tables: ";
	for (const auto table : tables)
	{
		std::cout << "\t- Table Number: " << table.n_table << ", Bill: " << table.bill << ", Discount: " << table.discount << std::endl;
	}
}

void Database::printEmployees()
{
	CROW_LOG_INFO << "[DB] printEmployees";

	std::vector<Employee> employees = getEmployees();

	CROW_LOG_INFO << "[LIST] Employees: ";
	for (const auto employee : employees)
	{
		std::cout << "\t- Name: " << employee.firstName << " " << employee.lastName << ", Level: " << employee.level;
	}
}

void Database::printProducts()
{
	CROW_LOG_INFO << "[DB] printProducts";

	std::vector<Product> products = getProducts();

	CROW_LOG_INFO << "[LIST] Products: ";
	for (const auto product : products)
	{
		std::cout << "\t- Name: " << product.name << ", Price: " << product.price << std::endl;
	}
}

void Database::printOrders()
{
	/*
	CROW_LOG_INFO << "[DB] printOrders";

	std::vector<Order> orders = getOrders();

	CROW_LOG_INFO << "[LIST] Orders: ";
	for (const auto order : orders) {
		std::cout << "\t- Time: " << order.time << ", Message: " << order.message << std::endl;
	}*/
}

void Database::printIngredients()
{
	CROW_LOG_INFO << "[DB] printIngredients";

	std::vector<Ingredient> ingredients = getIngredients();

	CROW_LOG_INFO << "[LIST] Ingredients: ";
	for (const auto ingredient : ingredients)
	{
		std::cout << "\t- Name: " << ingredient.name << std::endl;
	}
}

void Database::printAllergens()
{
	CROW_LOG_INFO << "[DB] printAllergens";

	std::vector<Allergen> allergens = getAllergens();

	CROW_LOG_INFO << "[LIST] Allergens: ";
	for (const auto allergen : allergens)
	{
		std::cout << "\t- Name: " << allergen.name << std::endl;
	}
}


// Change
void Database::moveTable(const int& current_n_table,
						 const int& new_n_table)
{
	CROW_LOG_INFO << "[DB] moveTable";

	try
	{
		std::stringstream query;

		query << "SELECT * FROM tables WHERE n_table = " << current_n_table;
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		res->next();
		int current_table_id = res->getInt("table_id");

		query << "SELECT table_id, bill FROM tables WHERE n_table = " << new_n_table;
		res = stmt->executeQuery(query.str());
		query.str("");

		Table curr_table = getTableByNumber(current_n_table);
		Table new_table = getTableByNumber(new_n_table);

		// If theres an existing table
		if (res->next())
		{
			int new_table_id = res->getInt("table_id");
			int new_table_bill = res->getInt("bill");

			query << "SELECT product_id, amount FROM tableproduct WHERE table_id =" << current_table_id;
			res = stmt->executeQuery(query.str());
			query.str("");

			// For each product of current table
			while (res->next())
			{
				int current_table_product_id = res->getInt("product_id");
				int current_table_product_amount = res->getInt("amount");

				query << "SELECT * FROM tableproduct WHERE table_id =" << new_table_id << " AND product_id = " << current_table_product_id;
				sql::ResultSet* res2 = stmt->executeQuery(query.str());
				query.str("");

				// If product_id matches between current and new table. Update amount and new table bill
				if (res2->next())
				{
					int new_table_product_id = res2->getInt("product_id");
					int new_table_amount = res2->getInt("amount");

					query << "SELECT * FROM tableproduct WHERE table_id =" << current_table_id << " AND product_id = " << current_table_product_id;
					sql::ResultSet* res3 = stmt->executeQuery(query.str());
					query.str("");

					if (res3->next())
					{
						int current_table_amount = res3->getInt("amount");
						new_table_amount += current_table_amount;

						// Update new table product amount
						pstmt = con->prepareStatement("UPDATE tableproduct SET amount = ? WHERE table_id = ? AND product_id = ?");
						pstmt->setInt(1, new_table_amount);
						pstmt->setInt(2, new_table_id);
						pstmt->setInt(3, current_table_product_id);
						pstmt->execute();

						query << "SELECT price FROM products WHERE product_id =" << current_table_product_id;
						res3 = stmt->executeQuery(query.str());
						query.str("");

						if (res3->next())
						{
							double current_table_product_price = res3->getDouble("price");
							new_table_bill += current_table_product_price * current_table_product_amount;

							// Update new table bill
							pstmt = con->prepareStatement("UPDATE tables SET bill = ? WHERE table_id = ?");
							pstmt->setInt(1, new_table_bill);
							pstmt->setInt(2, new_table_id);
							pstmt->execute();

							// Delete the current table tableproduct row
							pstmt = con->prepareStatement("DELETE FROM tableproduct WHERE table_id = ? AND product_id = ?");
							pstmt->setInt(1, current_table_id);
							pstmt->setInt(2, current_table_product_id);
							pstmt->execute();
						}
					}
				}
				else
				{ // If product doesnt match, change its table_id and update table bill
					pstmt = con->prepareStatement("UPDATE tableproduct SET table_id = ? WHERE table_id = ? AND product_id = ?");
					pstmt->setInt(1, new_table_id);
					pstmt->setInt(2, current_table_id);
					pstmt->setInt(3, current_table_product_id);
					pstmt->execute();

					query << "SELECT price FROM products WHERE product_id =" << current_table_product_id;
					sql::ResultSet* res3 = stmt->executeQuery(query.str());
					query.str("");

					res3->next();
					int current_table_product_price = res3->getDouble("price");
					new_table_bill += (current_table_product_price * current_table_product_amount);

					pstmt = con->prepareStatement("UPDATE tables SET bill = ? WHERE table_id = ?");
					pstmt->setInt(1, new_table_bill);
					pstmt->setInt(2, new_table_id);
					pstmt->execute();
				}
			}

			removeTable(curr_table);
		}
		else
		{ // If there is no existing table just change n_table
			pstmt = con->prepareStatement("UPDATE tables SET n_table = ? WHERE table_id = ?");
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

void Database::changeTableProductAmount(const Table& table,
										const Product& product,
										const int& new_amount)
{
	CROW_LOG_INFO << "[DB] changeTableProductAmount";

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		std::stringstream query;

		query << "SELECT table_id, bill, discount FROM tables WHERE n_table = " << table.n_table;
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		if (res->next())
		{
			int table_id = res->getInt("table_id");
			double bill = res->getDouble("bill");
			double discount = res->getDouble("discount");

			query << "SELECT product_id FROM products WHERE name = '" << product.name << "' AND price = '" << product.price << "'";
			res = stmt->executeQuery(query.str());
			query.str("");

			if (res->next())
			{
				int product_id = res->getInt("product_id");

				query << "SELECT amount FROM tableproduct WHERE table_id = '" << table_id << "' AND product_id = '" << product_id << "'";
				res = stmt->executeQuery(query.str());
				query.str("");

				if (res->next())
				{
					int amount = res->getInt("amount");

					if (new_amount > 0)
					{
						double new_bill = bill + product.price * (new_amount - amount) * (1.0 - discount);

						pstmt = con->prepareStatement("UPDATE tables SET bill = ? WHERE n_table = ?");
						pstmt->setDouble(1, new_bill);
						pstmt->setInt(2, table.n_table);
						pstmt->execute();

						pstmt = con->prepareStatement("UPDATE tableproduct SET amount = ? WHERE table_id = ? AND product_id = ?");
						pstmt->setInt(1, new_amount);
						pstmt->setInt(2, table_id);
						pstmt->setInt(3, product_id);
						pstmt->execute();
					}
					else
					{
						removeTableProduct(table.n_table, product, amount);
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
void Database::removeTable(const Table& table)
{
	CROW_LOG_INFO << "[DB] removeTable";

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		int n_table = table.n_table;
		double bill = table.bill;
		double discount = table.discount;

		std::stringstream query;
		query << "SELECT * FROM tables WHERE n_table = " << table.n_table;
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		if (res->next())
		{
			int table_id = res->getInt("table_id");

			pstmt = con->prepareStatement("DELETE FROM tableproduct WHERE table_id = ?");
			pstmt->setInt(1, table_id);
			pstmt->execute();

			pstmt = con->prepareStatement("DELETE FROM tables WHERE n_table = ? AND bill = ? AND discount = ?");
			pstmt->setInt(1, n_table);
			pstmt->setDouble(2, bill);
			pstmt->setDouble(3, discount);
			pstmt->execute();

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

void Database::removeEmployee(const Employee& employee)
{
	CROW_LOG_INFO << "[DB] removeEmployee";

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		pstmt = con->prepareStatement("DELETE FROM employees WHERE firstName = ? AND lastName = ? AND level = ?");
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

void Database::removeProduct(const Product& product)
{
	CROW_LOG_INFO << "[DB] removeProduct";

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		std::string name = product.name;
		double price = product.price;
		int page = product.page;

		pstmt = con->prepareStatement("DELETE FROM products WHERE name = ? AND price = ? AND page = ?");
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

void Database::removeTableProduct(const int& n_table,
								  const Product& product,
								  const int& times)
{
	CROW_LOG_INFO << "[DB] remoTableProduct";

	try
	{
		std::stringstream query;

		query << "SELECT * FROM tables WHERE n_table = " << n_table;
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		if (res->next())
		{
			int table_id = res->getInt("table_id");
			double bill = res->getDouble("bill");
			double discount = res->getDouble("discount");

			query << "SELECT product_id FROM products WHERE name = '" << product.name << "' AND price = " << product.price;
			res = stmt->executeQuery(query.str());
			query.str("");

			if (res->next())
			{
				int product_id = res->getInt("product_id");

				pstmt = con->prepareStatement("DELETE FROM tableproduct WHERE table_id = ? AND product_id = ? AND details = ?");
				pstmt->setInt(1, table_id);
				pstmt->setInt(2, product_id);
				pstmt->setString(3, product.details);
				pstmt->execute();

				double new_bill = bill - (product.price * times * (1.0 - discount));
				if (new_bill < 0) new_bill = 0;

				pstmt = con->prepareStatement("UPDATE tables SET bill = ? WHERE table_id = ?");
				pstmt->setDouble(1, new_bill);
				pstmt->setInt(2, table_id);
				pstmt->execute();
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

void Database::removeProductIngredient(const Product& product,
									   const Ingredient& ingredient)
{
	CROW_LOG_INFO << "[DB] removeProductIngredient";

	try
	{
		std::stringstream query;

		query << "SELECT product_id FROM products WHERE name = '" << product.name << "' AND price = '" << product.price << "'";
		sql::ResultSet* res = stmt->executeQuery(query.str());
		query.str("");

		if (res->next())
		{
			int product_id = res->getInt("product_id");

			query << "SELECT ingredient_id FROM ingredients WHERE name = '" << ingredient.name << "'";
			res = stmt->executeQuery(query.str());
			query.str("");

			if (res->next())
			{
				int ingredient_id = res->getInt("ingredient_id");

				pstmt = con->prepareStatement("DELETE FROM productingredient WHERE product_id = ? AND ingredient_id = ?");
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

void Database::removeProductIngredients(const Product& product)
{
	CROW_LOG_INFO << "[DB] remoProductIngredients";

	std::unique_lock<std::mutex> lock(mutex);

	std::stringstream query;

	query << "SELECT product_id FROM products WHERE name = '" << product.name << "' AND price = '" << product.price << "'";
	sql::ResultSet* res = stmt->executeQuery(query.str());
	query.str("");

	if (res->next())
	{
		int product_id = res->getInt("product_id");

		pstmt = con->prepareStatement("DELETE FROM productingredient WHERE product_id = ?");
		pstmt->setInt(1, product_id);
		pstmt->execute();
	}
}

void Database::removeProductAllergens(const Product& product)
{
	CROW_LOG_INFO << "[DB] removeProductAllergens";

	std::unique_lock<std::mutex> lock(mutex);

	std::stringstream query;

	query << "SELECT product_id FROM products WHERE name = '" << product.name << "' AND price = '" << product.price << "'";
	sql::ResultSet* res = stmt->executeQuery(query.str());
	query.str("");

	if (res->next())
	{
		int product_id = res->getInt("product_id");

		pstmt = con->prepareStatement("DELETE FROM productallergen WHERE product_id = ?");
		pstmt->setInt(1, product_id);
		pstmt->execute();
	}
}

void Database::removeOrder(const Order& order)
{
	/*
	CROW_LOG_INFO << "[DB] removeOrder";

	try {
		std::string time = order.time;
		std::string message = order.message;

		pstmt = con->prepareStatement("DELETE FROM orders WHERE time = ? AND message = ?");
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

void Database::removeIngredient(const Ingredient& ingredient)
{
	CROW_LOG_INFO << "[DB] removeIngredient";

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		std::string name = ingredient.name;

		pstmt = con->prepareStatement("DELETE FROM ingredients WHERE name = ?");
		pstmt->setString(1, name);
		pstmt->execute();

		CROW_LOG_INFO << "[REMOVED] Ingredient with name " << name;
	}
	catch (const sql::SQLException& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not remove ingredient. Error message: " << e.what();
	}
}

void Database::removeAllergen(const Allergen& allergen)
{
	CROW_LOG_INFO << "[DB] removeAllergen";

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		std::string name = allergen.name;

		pstmt = con->prepareStatement("DELETE FROM allergens WHERE name = ?");
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
void Database::modifyProduct(const Product& oldProduct,
							 const Product& newProduct)
{
	CROW_LOG_INFO << "[DB] modifyProduct";

	try
	{
		std::unique_lock<std::mutex> lock(mutex);

		std::string selectedElementName = oldProduct.name;
		double selectedElementPrice = oldProduct.price;
		std::string selectedElementColor = oldProduct.color;
		int selectedElementPage = oldProduct.page;
		bool selectedElementDeployable = oldProduct.deployable;

		std::string newName = newProduct.name;
		double newPrice = newProduct.price;
		std::string newColor = newProduct.color;

		std::stringstream query;
		query << "UPDATE products SET name = ?, price = ?, color = ? WHERE name = '" << selectedElementName << "' AND price = '" << selectedElementPrice << "' AND page = '" << selectedElementPage << "'";
		pstmt = con->prepareStatement(query.str());
		query.str("");
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