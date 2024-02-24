#include "..\headers\server.h"
#include "..\headers\crow_all.h"
#include "..\headers\domain.h"
#include <sstream>

Server::Server()
{
	database = std::make_unique<Database>();
	restaurant = std::make_unique<Restaurant>();
}

Server::~Server()
{
}


Database& Server::db()
{
	return *database;
}

void Server::initialize()
{
	database->initialize();

	// Make the restaurant instances that store different cfigs of products
	using productsMenus_t = std::vector<std::tuple<Product, std::vector<Product>>>;

	Product p1("Drinks", 0.0, "#FEF95D", 1, 0);
	Product p2("Beers", 0.0, "#FEF95D", 1, 0);
	Product p3("Caffeteria", 0.0, "#FEF95D", 1, 0);
	Product p4("White wines", 0.0, "#FEF95D", 1, 0);
	Product p5("Red wines", 0.0, "#FEF95D", 1, 0);
	Product p6("Rosse wines", 0.0, "#FEF95D", 1, 0);
	Product p7("Sparkling wines", 0.0, "#FEF95D", 1, 0);
	saveProduct(p1);
	saveProduct(p2);
	saveProduct(p3);
	saveProduct(p4);
	saveProduct(p5);
	saveProduct(p6);
	saveProduct(p7);

	Product p8("Appetizers", 0.0, "#FEF95D", 2, 0);
	Product p9("Snacks", 0.0, "#FEF95D", 2, 0);
	Product p10("Salads", 0.0, "#FEF95D", 2, 0);
	Product p11("Burguers and Sandwiches", 0.0, "#FEF95D", 2, 0);
	Product p12("Kids Menu", 0.0, "#FEF95D", 2, 0);
	Product p13("Pizzas", 0.0, "#FEF95D", 2, 0);
	Product p14("Desserts", 0.0, "#FEF95D", 2, 0);
	saveProduct(p8);
	saveProduct(p9);
	saveProduct(p10);
	saveProduct(p11);
	saveProduct(p12);
	saveProduct(p13);
	saveProduct(p14);

	Product p15("Cocktails", 0.0, "#FEF95D", 5, 0);
	Product p16("Rum", 0.0, "#FEF95D", 5, 0);
	Product p17("Gin", 0.0, "#FEF95D", 5, 0);
	Product p18("Whisky", 0.0, "#FEF95D", 5, 0);
	Product p19("Vodka", 0.0, "#FEF95D", 5, 0);
	Product p20("Various", 0.0, "#FEF95D", 5, 0);
	Product p21("Juices", 0.0, "#FEF95D", 5, 0);
	saveProduct(p15);
	saveProduct(p16);
	saveProduct(p17);
	saveProduct(p18);
	saveProduct(p19);
	saveProduct(p20);
	saveProduct(p21);

	// Drinks
	Product p22("Water", 1.5, "#FFFFFF", 1, 1);
	Product p23("Cocacola", 2.0, "#FFFFFF", 1, 1);
	Product p24("Fanta", 2.0, "#FFFFFF", 1, 1);
	Product p25("Aquarius", 2.0, "#FFFFFF", 1, 1);
	Product p26("Nestea", 2.0, "#FFFFFF", 1, 1);
	Product p27("Sprite", 2.0, "#FFFFFF", 1, 1);
	saveProduct(p22);
	saveProduct(p23);
	saveProduct(p24);
	saveProduct(p25);
	saveProduct(p26);
	saveProduct(p27);

	// Beers
	Product p28("Guiness", 4.0, "#FFFFFF", 1, 2);
	Product p29("Paulaner", 4.0, "#FFFFFF", 1, 2);
	Product p30("Voll-Damm", 4.5, "#FFFFFF", 1, 2);
	Product p31("Estrella Galicia", 3.0, "#FFFFFF", 1, 2);
	Product p32("Heineken", 3.0, "#FFFFFF", 1, 2);
	saveProduct(p28);
	saveProduct(p29);
	saveProduct(p30);
	saveProduct(p31);
	saveProduct(p32);

	// Caffeteria
	Product p33("Espresso", 1.0, "#FFFFFF", 1, 3);
	Product p34("Latte", 2.0, "#FFFFFF", 1, 3);
	Product p35("Capuccino", 2.0, "#FFFFFF", 1, 3);
	Product p36("American", 2.0, "#FFFFFF", 1, 3);
	Product p37("Irish Coffee", 2.0, "#FFFFFF", 1, 3);
	saveProduct(p33);
	saveProduct(p34);
	saveProduct(p35);
	saveProduct(p36);
	saveProduct(p37);

	std::vector<productsMenus_t> pages(5);
	std::vector<Product> empty_vec;
	std::vector<Product> empty_deployable = { {Product("", 0.0, "#FFFFFF", 1, 0)} };

	// Page 1
	std::vector<Product> drinks = { {p22}, {p23}, {p24}, {p25}, {p26} , {p27} };
	std::vector<Product> beers = { {p28}, {p29}, {p30}, {p31}, {p32} };
	std::vector<Product> coffees = { {p33}, {p34}, {p35}, {p36}, {p37} };
	std::vector<Product> white_wines = empty_deployable;
	std::vector<Product> red_wines = empty_deployable;
	std::vector<Product> rosse_wines = empty_deployable;
	std::vector<Product> sparkling_wines = empty_deployable;

	std::tuple<Product, std::vector<Product>> tup1 = { p1, drinks };
	std::tuple<Product, std::vector<Product>> tup2 = { p2, beers };
	std::tuple<Product, std::vector<Product>> tup3 = { p3, coffees };
	std::tuple<Product, std::vector<Product>> tup4 = { p4, white_wines };
	std::tuple<Product, std::vector<Product>> tup5 = { p5, red_wines };
	std::tuple<Product, std::vector<Product>> tup6 = { p6, rosse_wines };
	std::tuple<Product, std::vector<Product>> tup7 = { p7, sparkling_wines };

	// Page 2
	std::tuple<Product, std::vector<Product>> tup8 = { p8, empty_deployable };
	std::tuple<Product, std::vector<Product>> tup9 = { p9, empty_deployable };
	std::tuple<Product, std::vector<Product>> tup10 = { p10, empty_deployable };
	std::tuple<Product, std::vector<Product>> tup11 = { p11, empty_deployable };
	std::tuple<Product, std::vector<Product>> tup12 = { p12, empty_deployable };
	std::tuple<Product, std::vector<Product>> tup13 = { p13, empty_deployable };
	std::tuple<Product, std::vector<Product>> tup14 = { p14, empty_deployable };

	// Page 3
	std::tuple<Product, std::vector<Product>> tup15 = { p15, empty_deployable };
	std::tuple<Product, std::vector<Product>> tup16 = { p16, empty_deployable };
	std::tuple<Product, std::vector<Product>> tup17 = { p17, empty_deployable };
	std::tuple<Product, std::vector<Product>> tup18 = { p18, empty_deployable };
	std::tuple<Product, std::vector<Product>> tup19 = { p19, empty_deployable };
	std::tuple<Product, std::vector<Product>> tup20 = { p20, empty_deployable };
	std::tuple<Product, std::vector<Product>> tup21 = { p21, empty_deployable };

	product_unordered_map order1 = { {p35, 1}, {p37, 1} };
	Table t1(60, 1, order1, 0.0);
	saveTable(t1);
	for (auto const& p : order1)
	{
		saveTableProduct(t1, p.first, p.second);
	}

	product_unordered_map order2 = { {p31, 3}, {p26, 1} };
	Table t2(12, 5, order2, 10.0);
	saveTable(t2);
	for (auto const& p : order2)
	{
		saveTableProduct(t2, p.first, p.second);
	}

	pages[0] = { tup1, tup2, tup3, tup4, tup5, tup6, tup7 };
	pages[1] = { tup8, tup9, tup10, tup11, tup12, tup13, tup14 };
	pages[2] = { tup15, tup16, tup17, tup18, tup19, tup20, tup21 };
	pages[3] = { };
	pages[4] = { };
	//restaurant->pages = pages;
}

std::vector<Product> Server::getProductsByDeployableId(const int& deployable_id)
{
	return database->getProductsByDeployableId(deployable_id);
}

void Server::getDataFromDatabase()
{
	auto ps = database->getProducts();
	int id = 1;

	for (auto& p : ps)
	{
		if (p.deployable == 0)
		{
			std::vector<Product> deployable_products = getProductsByDeployableId(id);
			if (deployable_products.empty()) deployable_products = { Product("", 0.0, "#FFFFFF", 0, 0) };
			restaurant->pages[p.page - 1].push_back({ p, deployable_products });
		}
		else
		{
			std::vector<Product> empty_vector;
			restaurant->pages[p.page - 1].push_back({ p, empty_vector });
		}

		id++;
	}
}

void Server::dropAllTables()
{
	database->dropAllTables();
}

std::string Server::prepareOrdersJSON(const std::vector<Order>& orders)
{
	std::stringstream ss;

	ss << "{\n  'orders':[ \n";

	for (auto& order : orders)
	{
		ss << "    {\n"
			<< "      'id':" << order.id << ",\n"
			<< "      'n_table':" << order.n_table << ",\n"
			<< "      'bill':" << order.bill << ",\n"
			<< "      'discount':" << order.discount << ",\n"
			<< "      'employee':'" << order.employee << "',\n"
			<< "      'date':'" << order.date << "',\n"
			<< "      'products':[ \n";

		for (const auto& product : order.products)
		{
			ss << "        {\n"
				<< "          'name':'" << product.first.name << "',\n"
				<< "          'price':'" << product.first.price << "',\n"
				<< "          'amount':" << product.second << "\n"
				<< "        }, \n";

			// Remove the comma
			if (product.first.name == order.products.back().first.name)
			{
				ss.seekp(-3, std::ios_base::end);
			}
		}

		ss << "] \n    }, \n";

		// Remove the comma
		if (order.id == orders.back().id)
		{
			ss.seekp(-3, std::ios_base::end);
		}
	}

	// Remove the comma
	if (!orders.empty())
	{
		ss.seekp(-3, std::ios_base::end);
	}

	ss << "]\n}";

	return ss.str();
}


std::string Server::hash(const std::string& s)
{
	std::hash<std::string> hasher;
	size_t hashValue = hasher(s);

	std::mt19937 generator(hashValue);

	std::uniform_int_distribution<int> distribution(0, ASCII_CHARACTERS.size() - 1);

	std::stringstream ss;
	for (int i = 0; i < HASH_LENGTH; ++i)
	{
		ss << ASCII_CHARACTERS[distribution(generator)];
	}

	return ss.str();
}

std::string Server::generateSessionToken(Employee e)
{
	return database->generateSessionToken(e);
}

void Server::modifyProduct(const Product& oldProduct,
						   const Product& newProduct)
{
	database->modifyProduct(oldProduct, newProduct);
}

// Save
void Server::saveTable(const Table& table)
{
	Table t = getTableByNumber(table.n_table);

	if (t.isEmpty())
	{ // If this table is not saved
		restaurant->current_tables[table.n_table] = table; // We first store it in server instance
		database->saveTable(table); // And the in the database
	}
	else
	{
		CROW_LOG_WARNING << "[EXCEPTION] Table is already in the database.";
	}
}

// TODO: The same as in saveTable but for the rest
void Server::saveEmployee(const Employee& employee)
{
	database->saveEmployee(employee);
}

void Server::saveProduct(const Product& product)
{
	database->saveProduct(product);
}

void Server::saveOrder(const Order& order)
{
	database->saveOrder(order);
}

void Server::saveOrderProduct(const Order& order,
							  const int& product_id,
							  const int& amount)
{
	database->saveOrderProduct(order, product_id, amount);
}

void Server::saveProductIngredient(const Product& product,
								   const Ingredient& ingredient)
{
	database->saveProductIngredient(product, ingredient);
}

void Server::saveIngredient(const Ingredient& ingredient)
{
	database->saveIngredient(ingredient);
}

void Server::saveAllergen(const Allergen& allergen)
{
	database->saveAllergen(allergen);
}

void Server::saveTableProduct(Table& table,
							  const Product& product,
							  const int& times)
{
	Table aux(table);
	aux.bill += product.price;
	//restaurant->current_tables[table.n_table] = table;

	database->saveTableProduct(aux, product, times);
}


// Get
std::vector<page_t> Server::getDataFromPages()
{
	return database->getDataFromPages();
}

std::pair<int, std::vector<Product>> Server::getProductsAndIds()
{
	return database->getProductsAndIds();
}

int Server::getRestaurantPagesSize()
{
	return restaurant->pages.size();
}

std::vector<Table> Server::getTables()
{
	return database->getTables();
}

Table Server::getTableByNumber(int n_table)
{
	return database->getTableByNumber(n_table);
}

std::vector<Employee> Server::getEmployees()
{
	return database->getEmployees();
}

Employee Server::getEmployeeByName(std::string name)
{
	return database->getEmployeeByName(name);
}

Employee Server::getEmployeeByAccount(const std::string& username,
									  const std::string& password_hash)
{
	return database->getEmployeeByAccount(username, password_hash);
}

Employee Server::getEmployeeBySessionToken(const std::string& session_token)
{
	return database->getEmployeeBySessionToken(session_token);
}

std::vector<Product> Server::getProducts()
{
	return database->getProducts();
}

Product Server::getProductByName(std::string name)
{
	return database->getProductByName(name);
}

int Server::getProductIdByName(const std::string name)
{
	return database->getProductIdByName(name);
}

std::vector<Order> Server::getOrders()
{
	return database->getOrders();
}

/*Order Server::getOrderByTime(const std::string time)
{
	return database->getOrderByTime(time);
}*/

std::vector<Ingredient> Server::getIngredients()
{
	return database->getIngredients();
}

Ingredient Server::getIngredientByName(std::string name)
{
	return database->getIngredientByName(name);
}

std::vector<Allergen> Server::getAllergens()
{
	return database->getAllergens();
}

Allergen Server::getAllergenByName(std::string name)
{
	return database->getAllergenByName(name);
}


// Change
void Server::moveTable(const int& current_n_table,
					   const int& new_n_table)
{
	Table current_table = restaurant->current_tables[current_n_table];
	Table new_table = restaurant->current_tables[new_n_table];

	restaurant->current_tables.erase(current_n_table);

	if (new_table.isEmpty())
	{
		current_table.n_table = new_n_table;
		restaurant->current_tables[new_n_table] = current_table;
	}
	else
	{
		for (auto curr_table_product : current_table.products)
		{
			auto found = new_table.products.find(curr_table_product.first);
			new_table.bill += (curr_table_product.first.price * curr_table_product.second * new_table.discount);

			if (found != new_table.products.end())
			{
				found->second += curr_table_product.second;
			}
			else
			{
				new_table.products[curr_table_product.first] = curr_table_product.second;
			}
		}
	}

	database->moveTable(current_n_table, new_n_table);
}

void Server::changeTableProductAmount(const Table& table,
									  const Product& product,
									  const int& new_amount)
{
	database->changeTableProductAmount(table, product, new_amount);
}


// Remove
void Server::removeTable(const Table& table)
{
	database->removeTable(table);
}

void Server::removeEmployee(const Employee& employee)
{
	database->removeEmployee(employee);
}

void Server::removeProduct(const Product& product)
{
	database->removeProduct(product);
}

void Server::removeTableProduct(const int& n_table,
								const Product& product,
								const int& times)
{
	database->removeTableProduct(n_table, product, times);
}

void Server::removeOrder(const Order& order)
{
	database->removeOrder(order);
}

void Server::removeIngredient(const Ingredient& ingredient)
{
	database->removeIngredient(ingredient);
}

void Server::removeAllergen(const Allergen& allergen)
{
	database->removeAllergen(allergen);
}


void Server::payTable(const int& n_table,
					  const std::string& employee,
					  const std::string& date)
{
	Order o;
	Table t = getTableByNumber(n_table);

	o.copyDataFromTable(t);
	o.employee = employee;
	o.date = date;
	saveOrder(o);

	removeTable(t);
}


// Print
void Server::printTables()
{
	database->printTables();
}

void Server::printEmployees()
{
	database->printEmployees();
}

void Server::printProducts()
{
	database->printProducts();
}

void Server::printOrders()
{
	database->printOrders();
}

void Server::printIngredients()
{
	database->printIngredients();
}

void Server::printAllergens()
{
	database->printAllergens();
}