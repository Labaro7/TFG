#include "..\headers\server.hpp"
#include "..\headers\domain.hpp"
#include "..\headers\constants.hpp"
#include "..\headers\placeholders.hpp"
#include "..\headers\authMiddleware.hpp"
#include <tuple>
#include <chrono>
#include <iomanip>
#include <type_traits>

// Made with conditional macros to not restrict the scope of the app variable to the if scope.
#if MIDDLEWARE_ACTIVATED 
using App_T = crow::App<AuthMiddleware>;
#else 
using App_T = crow::SimpleApp;
#endif

int main()
{
	App_T app;
	Server server;

	// Set the log level (DEBUG, INFO, WARNING, ERROR, CRITICAL
	crow::logger::setLogLevel(crow::LogLevel::Info);

	std::cout << "Server running on: " << SERVER_IP << ":" << SERVER_PORT << std::endl;

	//server.dropAllTables();
	//server.initialize();

	CROW_ROUTE(app, "/")
		([&server](const crow::request& req, crow::response& res)
		 {
			 std::ifstream file(INDEX_HTML_FILE_PATH);
			 std::string modifiedHTML = insertDataInPlaceHolders(&file, TABLES_PRICES_PLACEHOLDER, server);

			 if (modifiedHTML == "")
			 {
				 res.code = 500; // Internal Server Error
				 res.body = "Error reading HTML template", "text/plain";
			 }

			 res.set_header("Content-Type", "text/html");
			 res.write(modifiedHTML);
			 res.end();
		 });

	CROW_ROUTE(app, "/login")
		([&server](const crow::request& req, crow::response& res)
		 {
			 std::ifstream file(LOGIN_HTML_FILE_PATH);
			 std::stringstream ss;
			 ss << file.rdbuf();
			 std::string login_page = ss.str();
			 ss.str("");
			 file.close();

			 res.set_header("Content-Type", "text/html");
			 res.write(login_page);
			 res.end();
		 });

	CROW_ROUTE(app, "/login")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& json_data = crow::json::load(req.body);
			 std::string username = json_data["username"].s();
			 std::string password = server.hash(json_data["password"].s());

			 std::cout << "login" << std::endl;

			 CROW_LOG_INFO << "Employee logged in with username: " << username << " and password hash:" << password;

			 Employee e = server.getEmployeeByAccount(username, password);
			 if (!e.isEmpty())
			 {
				 CROW_LOG_INFO << "Found employee with session token: " << e.session_token;
				 // TODO: Set starting_date of the employee

				 std::ifstream file(INDEX_HTML_FILE_PATH);
				 std::stringstream ss;
				 ss << file.rdbuf();
				 std::string index_page = ss.str();
				 ss.str("");
				 file.close();

				 res.set_header("Content-Type", "text/html");
				 res.add_header("Set-Cookie", SESSION_TOKEN_NAME + "=" + e.session_token + "; Path=/");
				 res.add_header("Set-Cookie", "employee_name=" + e.firstName + " " + e.lastName + "; Path=/");
				 res.redirect("/");

				 std::cout << "redir " << res.body << std::endl;
			 }
			 else
			 {
				 res.redirect("/login");
				 CROW_LOG_ERROR << "Employee not found";
			 }

			 res.end();
		 });

	CROW_ROUTE(app, "/table")
		([&server](const crow::request& req, crow::response& res)
		 {
			 auto page = crow::mustache::load_text("table.html");
			 std::string n_table = req.url_params.get("tableInput"); // This has to match the name of the input that is being sent to get its value correctly.
			 if (n_table == "") n_table = "0";
			 Table t(stoi(n_table));
			 std::vector<Product> products = server.getProducts(); // TODO: Move inside placeHolder function

			 std::ifstream file(TABLE_HTML_FILE_PATH);
			 std::string modifiedHTML = insertDataInPlaceHolders(&file, TABLE_NUMBER_PLACEHOLDER, stoi(n_table), products, server);

			 if (modifiedHTML == "")
			 {
				 res.code = 500; // Internal Server Error
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
			 std::ifstream file(INDEX_HTML_FILE_PATH);
			 std::stringstream ss;
			 ss << file.rdbuf();
			 std::string index = ss.str();
			 ss.str("");
			 file.close();

			 // TODO: Get the date from the JSON
			 auto json_data = crow::json::load(req.body);

			 int n_table = json_data["n_table"].i();
			 auto order = json_data["order"];
			 auto added = json_data["added"];
			 auto modified = json_data["modified"];
			 auto deleted = json_data["deleted"];
			 auto employeeName = json_data["employee"].s();

			 Table t = server.getTableByNumber(n_table);
			 Employee employee = server.getEmployeeByName(employeeName);

			 if (t.isEmpty())
			 {
				 t = { n_table, 3, product_unordered_map(), 0.0 };
				 server.saveTable(t);
			 }

			 for (const auto& object : added)
			 {
				 int times = object["times"].i();
				 std::string details = object["details"].s();
				 Product p(object["name"].s(),
						   std::stod(object["price"].s()),
						   "#FFFFFF",
						   0,
						   false,
						   object["details"].s());

				 server.saveTableProduct(t, p, times, details, employee);
			 }

			 for (const auto& object : modified)
			 {
				 int new_times = object["new_amount"].i();
				 Product p(object["name"].s(),
						   std::stod(object["price"].s()),
						   "#FFFFFF",
						   0,
						   false,
						   object["details"].s());

				 server.changeTableProductAmount(t, p, new_times);
			 }

			 for (const auto& object : deleted)
			 {
				 int times = std::stoi(object["times"].s());
				 Product p(object["name"].s(),
						   std::stod(object["price"].s()),
						   "#FFFFFF",
						   0,
						   false,
						   object["details"].s());

				 server.removeTableProduct(n_table, p, times);
			 }
		 });

	CROW_ROUTE(app, "/modifyTable")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 std::cout << "modify " << std::endl;
		 });

	CROW_ROUTE(app, "/payTable")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 auto json_data = crow::json::load(req.body);
			 const int n_table = json_data["n_table"].i();
			 const int n_clients = json_data["n_clients"].i();
			 const double bill = std::stod(json_data["bill"].s());
			 const double paid = std::stod(json_data["paid"].s());
			 const double discount = json_data["discount"].d();
			 const std::string method = json_data["method"].s();
			 const std::string date = json_data["date"].s();

			 std::string employee = "";
			 if (MIDDLEWARE_ACTIVATED && AUTH_NEEDED)
			 {
				 employee = json_data["employee"].s();
			 }

			 Order order = {
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
		 });

	CROW_ROUTE(app, "/deleteTable")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 auto json_data = crow::json::load(req.body);
			 const int n_table = json_data["n_table"].i();

			 Table t = server.getTableByNumber(n_table);
			 server.removeTable(t);
		 });

	CROW_ROUTE(app, "/currentTables")
		([&server]()
		 {
			 std::vector<Table> tables = server.getTables();
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

			 int current_n_table = json_data["current_n_table"].i();
			 int new_n_table = json_data["new_n_table"].i();

			 server.moveTable(current_n_table, new_n_table);
		 });


	CROW_ROUTE(app, "/api/<path>")
		([&server](const crow::request& req, std::string path)
		 {
			 const crow::json::wvalue data = server.retrieveRequest(path);

			 return data.dump();
		 });

	CROW_CATCHALL_ROUTE(app)
		([]()
		 {
			 return "Wrong Route";
		 });

	CROW_ROUTE(app, "/stats")
		([&server](const crow::request& req, crow::response& res)
		 {
			 std::ifstream file(STATS_HTML_FILE_PATH);
			 std::stringstream ssHTML;
			 ssHTML << file.rdbuf();
			 file.close();
			 std::string contentHTML = ssHTML.str();
			 //std::string modifiedHTML = insertDataInPlaceHolders2(&file, PRODUCT_LIST_PLACEHOLDER, server);

			 /*if (modifiedHTML == "")
			 {
				 res.code = 500;
				 res.body = "Error reading HTML template", "text/plain";
			 }*/

			 res.set_header("Content-Type", "text/html");
			 res.write(contentHTML);
			 res.end();
		 });

	CROW_ROUTE(app, "/edit")
		([&server](const crow::request& req, crow::response& res)
		 {
			 std::ifstream file(EDIT_HTML_FILE_PATH);
			 std::string modifiedHTML = insertDataInPlaceHolders2(&file, PRODUCT_LIST_PLACEHOLDER, server);

			 if (modifiedHTML == "")
			 {
				 res.code = 500;
				 res.body = "Error reading HTML template", "text/plain";
			 }

			 res.set_header("Content-Type", "text/html");
			 res.write(modifiedHTML);
			 res.end();
		 });

	CROW_ROUTE(app, "/edit/add/product")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& json_data = crow::json::load(req.body);

			 std::string name = json_data["name"].s();
			 double price = std::stod(json_data["price"].s());
			 std::string color = json_data["color"].s();
			 int page = json_data["page"].i();
			 int deployable = json_data["deployable"].i();
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
				 Product p(name, price, color, page, deployable);
				 server.restaurant->pages[page - 1].push_back({ p, empty_vector });

				 server.saveProduct(p);
			 }
			 else
			 {
				 Product p(name, price, color, page, deployable);
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

	CROW_ROUTE(app, "/edit/modify/product")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& json_data = crow::json::load(req.body);

			 std::string selectedElementName = json_data["selectedElementName"].s();
			 double selectedElementPrice = std::stod(json_data["selectedElementPrice"].s());
			 int selectedElementPage = json_data["selectedElementPage"].i();

			 std::string newName = json_data["newName"].s();
			 double newPrice = std::stod(json_data["newPrice"].s());
			 std::string newColor = json_data["newColor"].s();

			 Product oldProduct(selectedElementName, selectedElementPrice, "#FFFFFF", selectedElementPage, 0);
			 Product newProduct(newName, newPrice, newColor, selectedElementPage, 0);

			 server.modifyProduct(oldProduct, newProduct);
		 });


	CROW_ROUTE(app, "/edit/delete/product")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& json_data = crow::json::load(req.body);

			 std::string selectedElementName = json_data["selectedElementName"].s();
			 double selectedElementPrice = std::stod(json_data["selectedElementPrice"].s());
			 int selectedElementPage = json_data["selectedElementPage"].i();

			 Product p(selectedElementName, selectedElementPrice, "#FFFFFF", selectedElementPage, 0);

			 server.removeProduct(p);
		 });

	CROW_ROUTE(app, "/edit/add/employee")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& json_data = crow::json::load(req.body);

			 Employee oldEmployee =
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

			 Employee newEmployee =
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

	CROW_ROUTE(app, "/edit/delete/employee")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& json_data = crow::json::load(req.body);

			 Employee employee =
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

	CROW_ROUTE(app, "/edit/add/ingredient")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& json_data = crow::json::load(req.body);
			 const auto& addedIngredients = json_data["added_ingredients"];

			 for (const auto& addedIngredientName : addedIngredients)
			 {
				 Ingredient addedIngredient(addedIngredientName.s());

				 server.saveIngredient(addedIngredient);
			 }
		 });

	CROW_ROUTE(app, "/edit/delete/ingredient")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& json_data = crow::json::load(req.body);
			 const auto& deletedIngredients = json_data["deleted_ingredients"];

			 for (const auto& deletedIngredientName : deletedIngredients)
			 {
				 Ingredient deletedIngredient(deletedIngredientName.s());

				 server.removeIngredient(deletedIngredient);
			 }
		 });

	CROW_ROUTE(app, "/edit/add/allergen")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& json_data = crow::json::load(req.body);
			 const auto& addedAllergens = json_data["added_allergens"];

			 for (const auto& addedAllergenName : addedAllergens)
			 {
				 Allergen addedAllergen(addedAllergenName.s());

				 server.saveAllergen(addedAllergen);
			 }
		 });

	CROW_ROUTE(app, "/edit/delete/allergen")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& json_data = crow::json::load(req.body);
			 const auto& deletedAllergens = json_data["deleted_allergens"];

			 for (const auto& deletedAllergenName : deletedAllergens)
			 {
				 Allergen deletedAllergen(deletedAllergenName.s());

				 server.removeAllergen(deletedAllergen);
			 }
		 });

	CROW_ROUTE(app, "/edit/add/relationship")
		.methods("POST"_method)
		([&server](const crow::request& req, crow::response& res)
		 {
			 const auto& json_data = crow::json::load(req.body);
			 const auto& selectedProduct = json_data["selected_product"];
			 const auto& selectedIngredients = json_data["selected_ingredients"];
			 const auto& selectedAllergens = json_data["selected_allergens"];

			 const Product product = server.getProductByName(selectedProduct.s());

			 server.removeProductIngredients(product);

			 std::cout << selectedProduct << std::endl;
			 std::cout << selectedIngredients << std::endl;
			 std::cout << selectedAllergens << std::endl;

			 for (const auto& selectedIngredient : selectedIngredients)
			 {
				 Ingredient ingredient(selectedIngredient.s());

				 server.saveProductIngredient(product, ingredient);
			 }

			 server.removeProductAllergens(product);

			 for (const auto& selectedAllergen : selectedAllergens)
			 {
				 Allergen allergen(selectedAllergen.s());

				 server.saveProductAllergen(product, allergen);
			 }
		 });


	// App methods chain
	app.bindaddr(SERVER_IP)
		.port(SERVER_PORT)
		.server_name(SERVER_NAME)
		.multithreaded()
		.ssl_file(CRT_FILE_PATH, KEY_FILE_PATH)
		.run();
}