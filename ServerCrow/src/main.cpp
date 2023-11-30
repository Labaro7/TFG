#include "..\headers\server.h"
#include "..\headers\domain.h"
#include "..\headers\constants.h"
#include "..\headers\placeholders.h"
#include <tuple>

int main() {
    crow::SimpleApp app;
    Server server;

    // Here we can set the log level (DEBUG, INFO, WARNING, ERROR, CRITICAL
    crow::logger::setLogLevel(crow::LogLevel::Warning);
    
    server.dropAllTables();
    server.initialize();

    CROW_ROUTE(app, "/")([&server](const crow::request& req, crow::response& res) {
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

    // TODO: Fix the 505 error when not inputting table and accepting
    CROW_ROUTE(app, "/table")([&server](const crow::request& req, crow::response& res) {
        auto page = crow::mustache::load_text("table.html");
        std::string n_table = req.url_params.get("tableInput"); // This has to match the name of the input that is being sent to get its value correctly.
        Table t(stoi(n_table));
        std::vector<Product> products = server.getProducts();

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

            if (t.isEmpty()) {
                t = { n_table, 3, product_unordered_map(), 0.0 };
                server.saveTable(t);
            }

            for (const auto& object : added) {
                int times = object["times"].i();
                Product p(object["name"].s(), object["price"].d());

                server.saveTableProduct(t, p, times);
            }

            // TODO: saveOrder();

            // TODO: Change the response to the client
            res.set_header("Content-Type", "text/html");
            res.write(index);
            res.end();
        });

    CROW_ROUTE(app, "/api/currentTables")([&server]() {
        std::vector<Table> tables = server.getTables();
        std::vector<int> n_tables;
        crow::json::wvalue response;

        for (const auto& t : tables) {
            n_tables.push_back(t.n_table);
            std::cout << t.n_table;
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

    CROW_CATCHALL_ROUTE(app)
        ([]() {
        return "Wrong Route";
    });

    // App methods chain
    app.bindaddr(SERVER_IP)
        .port(SERVER_PORT)
        .server_name(SERVER_NAME)
        .multithreaded()
        .ssl_file(CRT_FILE_PATH, KEY_FILE_PATH)
        .run();
}