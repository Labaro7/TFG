#include "..\include\server.hpp"
#include "..\include\domain.hpp"
#include "..\include\constants.hpp"
#include "..\include\placeholders.hpp"
#include "..\include\authMiddleware.hpp"
#include <tuple>
#include <chrono>
#include <iomanip>
#include <type_traits>

// Made with conditional macros to not restrict the scope of the app variable to the if scope.
#if MIDDLEWARE_ACTIVATED 
	using App_t = crow::App<AuthMiddleware>;
#else 
	using App_t = crow::SimpleApp;
#endif

namespace
{
	template<typename T>
	T validate(const crow::json::rvalue& json_field, const T default_value)
	{
		if constexpr (std::is_same_v<T, int>)
		{
			return json_field.t() == crow::json::type::Number ? json_field.i() : default_value;
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			return json_field.t() == crow::json::type::String ? json_field.s() : default_value;
		}
		else if constexpr (std::is_same_v<T, crow::json::rvalue>)
		{
			return json_field.t() == crow::json::type::List ? json_field : default_value;
		}
		else if constexpr (std::is_same_v<T, bool>)
		{
			return json_field.t() == crow::json::type::True || json_field.t() == crow::json::type::False ? json_field.b() : default_value;
		}
		else if constexpr (std::is_same_v<T, double>)
		{
			return json_field.t() == crow::json::type::Number ? json_field.d() : default_value;
		}
		else
		{
			static_assert("Unsupported type");
		}
	}
}


int main()
{
	Server server = Server::getInstance();
	App_t app;
	
	// Set the log level (DEBUG, INFO, WARNING, ERROR, CRITICAL)
	crow::logger::setLogLevel(crow::LogLevel::Debug);

	CROW_LOG_INFO << "Server running on: " << cts::SERVER_IP << ":" << cts::SERVER_PORT;

	//server.dropAllTables();
	//server.initialize();

	CROW_ROUTE(app, "/")
		([&server](const crow::request& req, crow::response& res)
		 {
			 std::ifstream file(cts::INDEX_HTML_FILE_PATH);
			 const std::string modifiedHTML = insertDataInPlaceHolders(&file, cts::TABLES_PRICES_PLACEHOLDER, server);

			 if (modifiedHTML == "")
			 {
				 res.set_header("Content-Type", "text/plain");
				 res.code = 500;
				 res.body = "Error reading HTML template";
			 }

			 res.set_header("Content-Type", "text/html");
			 res.code = 200;
			 res.write(modifiedHTML);
			 res.end();
		 });

	CROW_ROUTE(app, "/login")
		([&server](const crow::request& req, crow::response& res)
		 {
			 std::ifstream file(cts::LOGIN_HTML_FILE_PATH);
			 std::stringstream ss;

			 ss << file.rdbuf();
			 const std::string loginPage = ss.str();
			 ss.str("");

			 file.close();

			 res.set_header("Content-Type", "text/html");
			 res.code = 200;
			 res.write(loginPage);
			 res.end();
		 });

	CROW_ROUTE(app, "/login")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& jsonData = crow::json::load(req.body);
			 const std::string username = jsonData["username"].s();
			 const std::string password = server.hash(jsonData["password"].s());

			 CROW_LOG_INFO << "Employee logged in with username: " << username << " and password hash:" << password;

			 Employee e = server.getEmployeeByAccount(username, password);
			 if (!e.isEmpty())
			 {
				 CROW_LOG_INFO << "Found employee with session token: " << e.session_token;
				 // TODO: Set starting_date of the employee

				 std::ifstream file(cts::INDEX_HTML_FILE_PATH);
				 std::stringstream ss;
				 ss << file.rdbuf();
				 const std::string indexPage = ss.str();
				 ss.str("");
				 file.close();

				 res.set_header("Content-Type", "text/html");
				 res.set_header("Access-Control-Allow-Origin", "*");
				 res.add_header("Set-Cookie", cts::SESSION_TOKEN_NAME + "=" + e.session_token + "; Path=/");
				 res.add_header("Set-Cookie", "employee_name=" + e.firstName + " " + e.lastName + "; Path=/");

				 res.code = 200;
				 res.end();
			 }
		 });

	CROW_ROUTE(app, "/table")
		([&server](const crow::request& req, crow::response& res)
		 {
			 int n_table = 0; 
			 try
			 {
				 n_table = std::stoi(req.url_params.get("tableInput"));
			 }
			 catch (const std::invalid_argument& e){}

			 const std::vector<Product> products = server.getProducts(); // TODO: Move inside placeHolder function

			 std::ifstream file(cts::TABLE_HTML_FILE_PATH);
			 std::string modifiedHTML = insertDataInPlaceHolders(&file, cts::TABLE_NUMBER_PLACEHOLDER, n_table, products, server);

			 if (modifiedHTML == "")
			 {
				 res.code = 500;
				 res.body = "Error reading HTML template", "text/plain";
			 }

			 res.set_header("Content-Type", "text/html");
			 res.write(modifiedHTML);
			 res.end();
		 });

	CROW_ROUTE(app, "/order")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 std::ifstream file(cts::INDEX_HTML_FILE_PATH);
			 std::stringstream ss;

			 ss << file.rdbuf();
			 const std::string index = ss.str();
			 ss.str("");
			 file.close();

			 // TODO: Get the date from the JSON
			 const auto &json_data = crow::json::load(req.body);

			 const int n_table = validate(json_data["n_table"], 0);
			 const int n_clients = validate(json_data["n_clients"], 0);
			 const auto order = validate(json_data["order"], crow::json::rvalue());
			 const auto added = validate(json_data["added"], crow::json::rvalue());
			 const auto modified = validate(json_data["modified"], crow::json::rvalue());
			 const auto deleted = validate(json_data["deleted"], crow::json::rvalue());
			 const std::string employeeName = validate(json_data["employee"], std::string(""));

			 Table t = server.getTableByNumber(n_table);

			 const Employee employee = server.getEmployeeByName(employeeName);

			 if (t.isEmpty())
			 {
				 t = { n_table, n_clients, domain::product_unordered_map(), 0.0 };
				 server.saveTable(t);
			 }

				server.changeNumClients(t, n_clients);

				for (const auto& object : added)
				{
					const int times = object["times"].i();
					const std::string details = object["details"].s();
					const Product p(object["name"].s(),
							std::stod(object["price"].s()),
							"#FFFFFF",
							0,
							false,
							object["details"].s());

					server.saveTableProduct(t, p, times, details, employee);
				}

				for (const auto& object : modified)
				{
					const int new_times = object["new_amount"].i();
					const Product p(object["name"].s(),
							std::stod(object["price"].s()),
							"#FFFFFF",
							0,
							false,
							object["details"].s());

					server.changeTableProductAmount(t, p, new_times);
				}

				for (const auto& object : deleted)
				{
					const int times = std::stoi(object["times"].s());
					const Product p(object["name"].s(),
							std::stod(object["price"].s()),
							"#FFFFFF",
							0,
							false,
							object["details"].s());

					server.removeTableProduct(n_table, p, times);
				}
			
			 res.code = 200;
			 res.end();
		 });

	CROW_ROUTE(app, "/modifyTable")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {

		 });

	CROW_ROUTE(app, "/payTable")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& json_data = crow::json::load(req.body);

			 const int n_table = validate(json_data["n_table"], 0);
			 const int n_clients = validate(json_data["n_clients"], 0);
			 const double bill = validate(json_data["bill"], 0.0);
			 const double paid = validate(json_data["paid"], 0.0);
			 const double discount = validate(json_data["discount"], 0.0);
			 const std::string method = validate(json_data["method"], std::string(""));
			 const std::string date = validate(json_data["date"], std::string(""));

			 std::string employee = "";
			 if (MIDDLEWARE_ACTIVATED && cts::AUTH_NEEDED)
			 {
				 employee = json_data["employee"].s();
			 }

			 const Order order = {
				 0,
				 n_table,
				 n_clients,
				 bill,
				 paid,
				 discount,
				 method,
				 {},
				 employee,
				 date
			 };

			 server.payTable(order);

			 res.code = 200;
			 res.end();
		 });

	CROW_ROUTE(app, "/deleteTable")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& json_data = crow::json::load(req.body);
			 const int n_table = json_data["n_table"].i();

			 const Table t = server.getTableByNumber(n_table);
			 server.removeTable(t);
		 });

	CROW_ROUTE(app, "/currentTables")
		([&server]()
		 {
			 const std::vector<Table> tables = server.getTables();
			 std::vector<int> n_tables;
			 crow::json::wvalue response;

			 for (const auto& t : tables)
			 {
				 n_tables.push_back(t.n_table);
			 }

			 response["tables"] = n_tables;

			 return crow::response(response);
		 });

	CROW_ROUTE(app, "/moveTable")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& json_data = crow::json::load(req.body);

			 const int current_n_table = json_data["current_n_table"].i();
			 const int new_n_table = json_data["new_n_table"].i();

			 server.moveTable(current_n_table, new_n_table);
		 });

	CROW_ROUTE(app, "/api/<path>")
		([&server](const crow::request& req, std::string path)
		 {
			 const crow::json::wvalue& data = server.retrieveRequest(path);

			 crow::response res(data.dump());
			 res.set_header("Content-Type", "application/json");
			 return res;
		 });

	CROW_CATCHALL_ROUTE(app)
		([&server]()
		 {
			 return "Wrong Route";
		 });

	CROW_ROUTE(app, "/admin")
		([&server](const crow::request& req, crow::response& res)
		 {
			 std::ifstream file(cts::ADMIN_HTML_FILE_PATH);
			 std::string modifiedHTML = insertDataInPlaceHolders2(&file, cts::PRODUCT_LIST_PLACEHOLDER, server);

			 if (modifiedHTML == "")
			 {
				 res.code = 500;
				 res.body = "Error reading HTML template", "text/plain";
			 }

			 res.set_header("Content-Type", "text/html");
			 res.write(modifiedHTML);
			 res.end();
		 });

	CROW_ROUTE(app, "/admin/add/product")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& json_data = crow::json::load(req.body);

			 const std::string name = json_data["name"].s();
			 const double price = std::stod(json_data["price"].s());
			 const std::string color = json_data["color"].s();
			 const int page = json_data["page"].i();
			 const int deployable = json_data["deployable"].i();
			 std::vector<Product> empty_vector;

			 if (deployable == 0 && price == 0)
			 {
				 empty_vector = { Product("", 0.0, "#FFFFFF", 0, 0) };
				 Product p(name, price, color, page, deployable);
				 server.restaurant->pages[page - 1].push_back({ p, empty_vector });

				 server.saveProduct(p);
			 }
			 else if (deployable == 0 && price != 0)
			 {
				 const Product p(name, price, color, page, deployable);
				 server.restaurant->pages[page - 1].push_back({ p, empty_vector });

				 server.saveProduct(p);
			 }
			 else
			 {
				 const Product p(name, price, color, page, deployable);
				 for (auto& q : server.restaurant->pages[page - 1])
				 {
					 if (server.getProductIdByName(q.first.name) == deployable)
					 {
						 q.second.push_back(p);
					 }
				 }

				 server.saveProduct(p);
			 }
		 });

	CROW_ROUTE(app, "/admin/modify/product")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& json_data = crow::json::load(req.body);

			 const std::string selectedElementName = json_data["selectedElementName"].s();
			 const double selectedElementPrice = std::stod(json_data["selectedElementPrice"].s());
			 const int selectedElementPage = json_data["selectedElementPage"].i();

			 const std::string newName = json_data["newName"].s();
			 const double newPrice = std::stod(json_data["newPrice"].s());
			 const std::string newColor = json_data["newColor"].s();

			 const Product oldProduct(selectedElementName, selectedElementPrice, "#FFFFFF", selectedElementPage, 0);
			 const Product newProduct(newName, newPrice, newColor, selectedElementPage, 0);

			 server.modifyProduct(oldProduct, newProduct);
		 });


	CROW_ROUTE(app, "/admin/delete/product")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& json_data = crow::json::load(req.body);

			 const std::string selectedElementName = json_data["selectedElementName"].s();
			 const double selectedElementPrice = std::stod(json_data["selectedElementPrice"].s());
			 const int selectedElementPage = json_data["selectedElementPage"].i();

			 const Product p(selectedElementName, selectedElementPrice, "#FFFFFF", selectedElementPage, 0);

			 server.removeProduct(p);
		 });

	CROW_ROUTE(app, "/admin/add/employee")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& json_data = crow::json::load(req.body);

			 const Employee oldEmployee =
			 {
				 json_data["oldEmployee"]["firstName"].s(),
				 json_data["oldEmployee"]["lastName"].s(),
				 json_data["oldEmployee"]["email"].s(),
				 json_data["oldEmployee"]["id"].s(),
				 json_data["oldEmployee"]["mobileNumber"].s(),
				 static_cast<int>(json_data["oldEmployee"]["level"].i()),
				 json_data["oldEmployee"]["username"].s(),
				 server.hash(json_data["oldEmployee"]["password"].s()),
				 server.generateSessionToken()
			 };

			 const Employee newEmployee =
			 {
				 json_data["newEmployee"]["firstName"].s(),
				 json_data["newEmployee"]["lastName"].s(),
				 json_data["newEmployee"]["email"].s(),
				 json_data["newEmployee"]["id"].s(),
				 json_data["newEmployee"]["mobileNumber"].s(),
				 static_cast<int>(json_data["oldEmployee"]["level"].i()),
				 json_data["newEmployee"]["username"].s(),
				 server.hash(json_data["newEmployee"]["password"].s()),
				 server.generateSessionToken()
			 };

			 server.saveEmployee(oldEmployee, newEmployee);

			 res.code = 200;
			 res.end();
		 });

	CROW_ROUTE(app, "/admin/delete/employee")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& json_data = crow::json::load(req.body);

			 const Employee employee =
			 {
				 json_data["oldEmployee"]["firstName"].s(),
				 json_data["oldEmployee"]["lastName"].s(),
				 json_data["oldEmployee"]["email"].s(),
				 json_data["oldEmployee"]["id"].s(),
				 json_data["oldEmployee"]["mobileNumber"].s(),
				 static_cast<int>(json_data["oldEmployee"]["level"].i()),
				 json_data["oldEmployee"]["username"].s(),
				 server.hash(json_data["oldEmployee"]["password"].s()),
				 server.generateSessionToken()
			 };

			 server.removeEmployee(employee);

			 res.code = 200;
			 res.end();
		 });

	CROW_ROUTE(app, "/admin/add/ingredient")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& json_data = crow::json::load(req.body);
			 const auto& addedIngredients = json_data["added_ingredients"];

			 for (const auto& addedIngredientName : addedIngredients)
			 {
				 const Ingredient addedIngredient(addedIngredientName.s());

				 server.saveIngredient(addedIngredient);
			 }
		 });

	CROW_ROUTE(app, "/admin/delete/ingredient")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& json_data = crow::json::load(req.body);
			 const auto& deletedIngredients = json_data["deleted_ingredients"];

			 for (const auto& deletedIngredientName : deletedIngredients)
			 {
				 const Ingredient deletedIngredient(deletedIngredientName.s());

				 server.removeIngredient(deletedIngredient);
			 }
		 });

	CROW_ROUTE(app, "/admin/add/allergen")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& json_data = crow::json::load(req.body);
			 const auto& addedAllergens = json_data["added_allergens"];

			 for (const auto& addedAllergenName : addedAllergens)
			 {
				 const Allergen addedAllergen(addedAllergenName.s());

				 server.saveAllergen(addedAllergen);
			 }
		 });

	CROW_ROUTE(app, "/admin/delete/allergen")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& json_data = crow::json::load(req.body);
			 const auto& deletedAllergens = json_data["deleted_allergens"];

			 for (const auto& deletedAllergenName : deletedAllergens)
			 {
				 const Allergen deletedAllergen(deletedAllergenName.s());

				 server.removeAllergen(deletedAllergen);
			 }
		 });

	CROW_ROUTE(app, "/admin/add/relationship")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& json_data = crow::json::load(req.body);
			 const auto& selectedProduct = json_data["selected_product"];
			 const auto& selectedIngredients = json_data["selected_ingredients"];
			 const auto& selectedAllergens = json_data["selected_allergens"];

			 const Product product = server.getProductByName(selectedProduct.s());

			 server.removeProductIngredients(product);

			 for (const auto& selectedIngredient : selectedIngredients)
			 {
				 const Ingredient ingredient(selectedIngredient.s());

				 server.saveProductIngredient(product, ingredient);
			 }

			 server.removeProductAllergens(product);

			 for (const auto& selectedAllergen : selectedAllergens)
			 {
				 const Allergen allergen(selectedAllergen.s());

				 server.saveProductAllergen(product, allergen);
			 }
		 });


	// App methods chain
	app.bindaddr(cts::SERVER_IP)
		.port(cts::SERVER_PORT)
		.server_name(cts::SERVER_NAME)
		.multithreaded()
		.ssl_file(cts::CRT_FILE_PATH, cts::KEY_FILE_PATH)
		.run();
}