#include "..\headers\server.h"

int main() {
    crow::SimpleApp app;
    Server server;

    CROW_ROUTE(app, "/")([&server] {
        auto page = crow::mustache::load_text("index.html");

        server.dropAllTables();
        server.initialize();

        return page;
        });

    CROW_ROUTE(app, "/table")([&server](const crow::request& req, crow::response& res) {
        auto page = crow::mustache::load_text("table.html");

        std::string n_table = req.url_params.get("tableInput"); // This has to match the name of the input that is being sent to get its value correctly.
        server.saveTable((new Table(stoi(n_table))));

        std::cout << "Hey" << std::endl;
        std::ifstream html_file("C:\\Users\\User\\Desktop\\TFG\\ServerCrow\\ServerCrow\\templates\\table.html");
        std::cout << "Hey2" << std::endl;
        if (!html_file.is_open()) {
            std::cout << "HeyError" << std::endl;
            res = crow::response(500, "Error reading HTML");
            return;
        }   
        std::cout << "Hey22" << std::endl;

        // Read the content of the HTML template into a string
        std::stringstream html_content;
        html_content << html_file.rdbuf();
        html_file.close();
        std::cout << "Hey33" << std::endl;

        // Here, you would replace this with actual logic to retrieve current tables
        std::vector<Product> productsList = server.getProducts();
        std::vector<std::pair<std::string, float>> products;

        for (const auto p : productsList) {
            products.push_back({ p.getName(), p.getPrice() });
        }
        std::cout << "Hey44" << std::endl;

        // Generate HTML with current tables
        std::ostringstream ss;
        std::string table_list_html;
        for (const auto p : products) {
            std::string name_ = p.first;
            double price_ = round(p.second * 100.0) / 100.0; // Round the price to two decimals
            ss << std::fixed << std::setprecision(2) << "<li class = 'grid-product'>" << p.first << "<br>" << "<div class='prices'>" << price_ << "</div></li > " << std::endl; // We use this because std::to_string() eliminates the precision set
            //std::cout << p.first << " " << price_ << std::endl;
        }
        table_list_html = ss.str();

        // Inject the table list into the HTML template
        std::string final_html = html_content.str();
        size_t placeholder_pos = final_html.find("<!-- LIST OF PRODUCTS -->");
        if (placeholder_pos != std::string::npos) {
            final_html.replace(placeholder_pos, strlen("<!-- LIST OF PRODUCTS -->"), table_list_html);
        }

        res.set_header("Content-Type", "text/html");
        res.write(final_html);
        res.end();
        });


    CROW_CATCHALL_ROUTE(app)
        ([]() {
        return "Wrong Route";
            });

    // ignore all log
    crow::logger::setLogLevel(crow::LogLevel::Debug);
    //crow::logger::setHandler(std::make_shared<ExampleLogHandler>());

    app.bindaddr("192.168.1.66")
        .port(18080)
        .server_name("CrowCpp")
        .multithreaded()
        .run();
}