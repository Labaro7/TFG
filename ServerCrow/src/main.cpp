#include "..\headers\server.h"
#include "..\headers\domain.h"
#include "..\headers\constants.h"
#include "..\headers\placeholders.h"
#include <tuple>

int main() {
    crow::SimpleApp app;
    Server server;

    // Here we can set the log level (DEBUG, INFO, WARNING, ERROR, CRITICAL
    crow::logger::setLogLevel(crow::LogLevel::Error);
    
    //server.dropAllTables();
    //server.initialize();

    CROW_ROUTE(app, "/")
        ([&server](const crow::request& req, crow::response& res) {
            // TODO: Put relative path
            std::ifstream file("C:\\Users\\User\\Desktop\\TFG\\ServerCrow\\ServerCrow\\templates\\index.html");
            std::string modifiedHTML = insertDataInPlaceHolders(&file, TABLES_PRICES_PLACEHOLDER, server);

            // TODO: Change error handling
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
            std::ifstream file("C:\\Users\\User\\Desktop\\TFG\\ServerCrow\\ServerCrow\\templates\\login.html");
            std::stringstream ss;
            ss << file.rdbuf();
            std::string login_page = ss.str();
            ss.str("");
            file.close();

            res.set_header("Content-Type", "text/html");
            res.write(login_page);
            res.end();
        });

    // TODO: Fix the 505 error when not inputting table and accepting
    CROW_ROUTE(app, "/table")
        ([&server](const crow::request& req, crow::response& res) {
            auto page = crow::mustache::load_text("table.html");
            std::string n_table = req.url_params.get("tableInput"); // This has to match the name of the input that is being sent to get its value correctly.
            Table t(stoi(n_table));
            std::vector<Product> products = server.getProducts(); // TODO: Move inside placeHolder function

            // TODO: Put relative path
            std::ifstream file("C:\\Users\\User\\Desktop\\TFG\\ServerCrow\\ServerCrow\\templates\\table.html");
            std:: string modifiedHTML = insertDataInPlaceHolders(&file, TABLE_NUMBER_PLACEHOLDER, stoi(n_table), products, server);

            // TODO: Change error handling
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
            // TODO: Put relative path
            std::ifstream file("C:\\Users\\User\\Desktop\\TFG\\ServerCrow\\ServerCrow\\templates\\index.html");
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

            // TODO: saveOrder();
            // TODO: send signal to arduino to make a sound and print the order

            // TODO: Change the response to the client
            //res.set_header("Content-Type", "text/html");
            //res.write(index);
            //res.end();
        });

    CROW_ROUTE(app, "/payTable")
        .methods("POST"_method)
        ([&server](const crow::request& req, crow::response& res) {
            // Save order
            auto json_data = crow::json::load(req.body);
            const int n_table = json_data["n_table"].i();
            const std::string employee = json_data["employee"].s();
            const std::string date = json_data["date"].s();

            //std::cout << json_data << std::endl;

            server.payTable(n_table, employee, date);

            // Delete tableproduct rows
        });

    CROW_ROUTE(app, "/deleteTable")
        .methods("POST"_method)
        ([&server](const crow::request& req, crow::response& res) {
            auto json_data = crow::json::load(req.body);
            const int n_table = json_data["n_table"].i();

            Table t = server.getTableByNumber(n_table);
            server.removeTable(t);
        });

    CROW_ROUTE(app, "/api/currentTables")
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

    CROW_ROUTE(app, "/api/moveTable")
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
        std::stringstream ss;

        ss << "{\n  'orders':[ \n";

        for (auto& order : orders) {
            ss << "    {\n"
                << "      'id':" << order.id << ",\n"
                << "      'n_table':" << order.n_table << ",\n"
                << "      'bill':" << order.bill << ",\n"
                << "      'discount':" << order.discount << ",\n"
                << "      'employee':'" << order.employee << "',\n"
                << "      'date':'" << order.date << "',\n"
                << "      'products':[ \n";

            for (const auto& product : order.products) {
                ss << "        {\n"
                    << "          'name':'" << product.first.name << "',\n"
                    << "          'price':'" << product.first.price << "',\n"
                    << "          'amount':" << product.second << "\n"
                    << "        }, \n";

                // Remove the trailing comma if products are present
                if (product.first.name == order.products.back().first.name) {
                    ss.seekp(-3, std::ios_base::end);
                }
            }

            ss << "] \n    }, \n";

            // Remove the trailing comma if orders are present
            if (order.id == orders.back().id) {
                ss.seekp(-3, std::ios_base::end);
            }
        }

        // Remove the trailing comma if orders are present
        if (!orders.empty()) {
            ss.seekp(-2, std::ios_base::end);
        }

        ss << "\n  ] \n}";

        return crow::response{ ss.str() };
        });

    CROW_CATCHALL_ROUTE(app)
        ([]() {
        return "Wrong Route";
            });

    CROW_ROUTE(app, "/add")
        ([&server](const crow::request& req, crow::response& res) {
            std::ifstream file("C:\\Users\\User\\Desktop\\TFG\\ServerCrow\\ServerCrow\\templates\\add.html");

            // TODO: Put relative path
            std::string modifiedHTML = insertDataInPlaceHolders2(&file, PRODUCT_LIST_PLACEHOLDER, server);

            // TODO: Change error handling
            if (modifiedHTML == "") {
                res.code = 500; // Internal Server Error
                res.body = "Error reading HTML template", "text/plain";
            }

            res.set_header("Content-Type", "text/html");
            res.write(modifiedHTML);
            res.end();
        });

    CROW_ROUTE(app, "/add/product")
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

    CROW_ROUTE(app, "/add/employee")
        .methods("POST"_method)
        ([&server](const crow::request& req, crow::response& res) {


        });

    CROW_ROUTE(app, "/add/ingredient")
        .methods("POST"_method)
        ([&server](const crow::request& req, crow::response& res) {


        });

    CROW_ROUTE(app, "/add/allergen")
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