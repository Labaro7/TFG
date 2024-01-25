#include "..\headers\server.h"
#include "..\headers\domain.h"
#include "..\headers\constants.h"
#include "..\headers\placeholders.h"
#include "..\headers\authMiddleware.h"
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

int main() {
    App_T app;
    Server server;

    // Set the log level (DEBUG, INFO, WARNING, ERROR, CRITICAL
    crow::logger::setLogLevel(crow::LogLevel::Info);

    std::cout << "Server running on: " << SERVER_IP << ":" << SERVER_PORT << std::endl;

    //server.dropAllTables();
    //server.initialize();

    CROW_ROUTE(app, "/")
        ([&server](const crow::request& req, crow::response& res) {
            std::ifstream file(INDEX_HTML_FILE_PATH);
            std::string modifiedHTML = insertDataInPlaceHolders(&file, TABLES_PRICES_PLACEHOLDER, server);

            if (modifiedHTML == "") {
                res.code = 500; // Internal Server Error
                res.body = "Error reading HTML template", "text/plain";
            }

            res.set_header("Content-Type", "text/html");
            res.write(modifiedHTML);
            res.end();
        });

    CROW_ROUTE(app, "/login")
        ([&server](const crow::request& req, crow::response& res) {
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
        ([&server](const crow::request& req, crow::response& res) {
            const auto& json_data = crow::json::load(req.body);
            std::string username = json_data["username"].s();
            std::string password = server.hash(json_data["password"].s());

            std::cout << "login" << std::endl;

            CROW_LOG_INFO << "Employee logged in with username: " << username << " and password hash:" << password;

            Employee e = server.getEmployeeByAccount(username, password);
            if (!e.isEmpty()) {
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
                res.add_header("Set-Cookie", "employee_name=" + e.name + "; Path=/");
                res.redirect("/");

                std::cout << "redir " << res.body << std::endl;
            }
            else {
                res.redirect("/login");
                CROW_LOG_ERROR << "Employee not found";
            }
            
            res.end();
        });

    CROW_ROUTE(app, "/table")
        ([&server](const crow::request& req, crow::response& res) {
            auto page = crow::mustache::load_text("table.html");
            std::string n_table = req.url_params.get("tableInput"); // This has to match the name of the input that is being sent to get its value correctly.
            if (n_table == "") n_table = "0";
            Table t(stoi(n_table));
            std::vector<Product> products = server.getProducts(); // TODO: Move inside placeHolder function

            std::ifstream file(TABLE_HTML_FILE_PATH);
            std:: string modifiedHTML = insertDataInPlaceHolders(&file, TABLE_NUMBER_PLACEHOLDER, stoi(n_table), products, server);

            if (modifiedHTML == "") {
                res.code = 500; // Internal Server Error
                res.body = "Error reading HTML template", "text/plain";
            }

            res.set_header("Content-Type", "text/html");
            res.write(modifiedHTML);
            res.end();
        });

    CROW_ROUTE(app, "/order")
        .methods("POST"_method)
        ([&server](const crow::request& req, crow::response& res) {
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
            Table t = server.getTableByNumber(n_table);

            // If a table with that number doesnt exist
            if (t.isEmpty()) {
                t = { n_table, 3, product_unordered_map(), 0.0 };
                server.saveTable(t);
            }

            for (const auto& object : added) {
                int times = object["times"].i();
                Product p(object["name"].s(), std::stod(object["price"].s()), "#FFFFFF", 0, false);

                server.saveTableProduct(t, p, times);
            }
        });

    CROW_ROUTE(app, "/modifyTable")
        .methods("POST"_method)
        ([&server](const crow::request& req, crow::response& res) {
            std::cout << "modify " << std::endl;
        });

    CROW_ROUTE(app, "/payTable")
        .methods("POST"_method)
        ([&server](const crow::request& req, crow::response& res) {
            auto json_data = crow::json::load(req.body);
            const int n_table = json_data["n_table"].i();
            const std::string date = json_data["date"].s();
            std::string employee = "";

            if (MIDDLEWARE_ACTIVATED && AUTH_NEEDED) {
                employee = json_data["employee"].s();
            }
            
            server.payTable(n_table, employee, date);
        });

    CROW_ROUTE(app, "/deleteTable")
        .methods("POST"_method)
        ([&server](const crow::request& req, crow::response& res) {
            auto json_data = crow::json::load(req.body);
            const int n_table = json_data["n_table"].i();

            Table t = server.getTableByNumber(n_table);
            server.removeTable(t);
        });

    CROW_ROUTE(app, "/currentTables")
        ([&server]() {
            std::vector<Table> tables = server.getTables();
            std::vector<int> n_tables;
            crow::json::wvalue response;

            for (const auto& t : tables) {
                n_tables.push_back(t.n_table);
            }

            response["tables"] = n_tables;

            return crow::response(response);
        });

    CROW_ROUTE(app, "/moveTable")
        .methods("POST"_method)
        ([&server](const crow::request& req, crow::response& res) {
            const auto& json_data = crow::json::load(req.body);
            
            int current_n_table = json_data["current_n_table"].i();
            int new_n_table = json_data["new_n_table"].i();

            server.moveTable(current_n_table, new_n_table);
        });


    CROW_ROUTE(app, "/api/stats")
        ([&server]() {
            std::vector<Order> orders = server.getOrders();

            return server.prepareOrdersJSON(orders);
        });

    CROW_CATCHALL_ROUTE(app)
        ([]() {
        return "Wrong Route";
            });

    CROW_ROUTE(app, "/edit")
        ([&server](const crow::request& req, crow::response& res) {
            std::ifstream file(EDIT_HTML_FILE_PATH);
            std::string modifiedHTML = insertDataInPlaceHolders2(&file, PRODUCT_LIST_PLACEHOLDER, server);

            if (modifiedHTML == "") {
                res.code = 500;
                res.body = "Error reading HTML template", "text/plain";
            }

            res.set_header("Content-Type", "text/html");
            res.write(modifiedHTML);
            res.end();
        });

    CROW_ROUTE(app, "/edit/add/product")
        .methods("POST"_method)
        ([&server](const crow::request& req, crow::response& res) {
            const auto& json_data = crow::json::load(req.body);

            std::string name = json_data["name"].s();
            double price = std::stod(json_data["price"].s());
            std::string color = json_data["color"].s();
            int page = json_data["page"].i();
            int deployable = json_data["deployable"].i();
            std::vector<Product> empty_vector;

            if (deployable == 0 && price == 0) {
                empty_vector = { Product("", 0.0, "#FFFFFF", 0, 0) };
                Product p(name, price, color, page, deployable);
                server.restaurant->pages[page-1].push_back({ p, empty_vector });

                server.saveProduct(p);
            }
            else if (deployable == 0 && price != 0) {
                Product p(name, price, color, page, deployable);
                server.restaurant->pages[page-1].push_back({ p, empty_vector });

                server.saveProduct(p);
            }
            else {
                Product p(name, price, color, page, deployable);
                for (auto& q : server.restaurant->pages[page-1]) {
                    if (server.getProductIdByName(q.first.name) == deployable) {
                        q.second.push_back(p);
                    }
                }

                server.saveProduct(p);
            }
        });

    CROW_ROUTE(app, "/edit/modify/product")
        .methods("POST"_method)
        ([&server](const crow::request& req, crow::response& res) {
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
        ([&server](const crow::request& req, crow::response& res) {
            const auto& json_data = crow::json::load(req.body);

            std::string selectedElementName = json_data["selectedElementName"].s();
            double selectedElementPrice = std::stod(json_data["selectedElementPrice"].s());
            int selectedElementPage = json_data["selectedElementPage"].i();

            Product p(selectedElementName, selectedElementPrice, "#FFFFFF", selectedElementPage, 0);

            server.removeProduct(p);
        });

    CROW_ROUTE(app, "/edit/add/employee")
        .methods("POST"_method)
        ([&server](const crow::request& req, crow::response& res) {


        });

    CROW_ROUTE(app, "/edit/add/ingredient")
        .methods("POST"_method)
        ([&server](const crow::request& req, crow::response& res) {


        });

    CROW_ROUTE(app, "/edit/add/allergen")
        .methods("POST"_method)
        ([&server](const crow::request& req, crow::response& res) {


        });


    // App methods chain
    app.bindaddr(SERVER_IP)
        .port(SERVER_PORT)
        .server_name(SERVER_NAME)
        .multithreaded()
        .ssl_file(CRT_FILE_PATH, KEY_FILE_PATH)
        .run();
}