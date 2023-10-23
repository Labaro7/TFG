#include "..\headers\server.h"

const char* TABLE_NUMBER_PLACEHOLDER = "<!-- PLACEHOLDER: TABLE NUMBER -->"; // It is char* because of strlen()
const char* PRODUCT_LIST_1_PLACEHOLDER = "<!-- PLACEHOLDER: LIST OF PRODUCTS -->"; // Same

std::string insertDataInPlaceHolders(std::ifstream* file, const std::string tableNumberPlaceholder, const int n_table, const std::string productListPlaceholder, const std::vector<Product> products){
    if (!file->is_open()) {
        return "";
    }
    
    // Read the content of the HTML into contentHTML
    std::stringstream ssHTML;
    ssHTML << file->rdbuf();
    file->close();
    std::string contentHTML = ssHTML.str();

    // Make a HTML with the table number
    std::ostringstream ss;
    ss << "<div class='numTable'>Table: " << n_table << "</div>" << std::endl;
    std::string tableNumberHTML = ss.str();

    // Important to clear here or the table number will be shown in the next html piece
    ss.str("");

    // Insert previous HTML piece with table number into HTML
    size_t tableNumberPlaceholderPos = contentHTML.find(TABLE_NUMBER_PLACEHOLDER);
    if (tableNumberPlaceholderPos != std::string::npos) {
        contentHTML.replace(tableNumberPlaceholderPos, strlen(TABLE_NUMBER_PLACEHOLDER), tableNumberHTML);
    }

    // Get the list of data of products currently added to the database
    std::vector<std::pair<std::string, float>> productsInfo;

    for (const auto p : products) {
        productsInfo.push_back({ p.getName(), p.getPrice() });
    }


    // Generate HTML piece with current tables
    std::string tableListHTML;

    for (const auto p : productsInfo) {
        std::string name_ = p.first;
        double price_ = round(p.second * 100.0) / 100.0; // Round the price to two decimals
        ss << std::fixed << std::setprecision(2) << "<li class = 'grid-product'>" << p.first << "<br>" << "<div class='prices'>" << price_ << "</div></li > " << std::endl; // We use this because std::to_string() eliminates the precision set
    }

    tableListHTML = ss.str();

    // Insert HTML piece with current tables into webpage HTML
    size_t productListPlaceholderPos = contentHTML.find(PRODUCT_LIST_1_PLACEHOLDER);
    if (productListPlaceholderPos != std::string::npos) {
        contentHTML.replace(productListPlaceholderPos, strlen(PRODUCT_LIST_1_PLACEHOLDER), tableListHTML);
    }

    return contentHTML;
}

int main() {
    crow::SimpleApp app;
    Server server;

    CROW_ROUTE(app, "/")([&server] {
        auto page = crow::mustache::load_text("index.html");

        server.dropAllTables();
        server.initialize();

        return page;
        });

    // TODO: Fix the 505 error when not inputting table and accepting
    CROW_ROUTE(app, "/table")([&server](const crow::request& req, crow::response& res) {
        auto page = crow::mustache::load_text("table.html");
        std::string n_table = req.url_params.get("tableInput"); // This has to match the name of the input that is being sent to get its value correctly.
        Table t(stoi(n_table));

        server.saveTable(&t);

        // TODO: Put relative path
        std::ifstream file("C:\\Users\\User\\Desktop\\TFG\\ServerCrow\\ServerCrow\\templates\\table.html");
        std::vector<Product> products = server.getProducts();
        std:: string modifiedHTML = insertDataInPlaceHolders(&file, TABLE_NUMBER_PLACEHOLDER, stoi(n_table), PRODUCT_LIST_1_PLACEHOLDER, products);

        if (modifiedHTML == "") {
            res.code = 500; // Internal Server Error
            res.body = "Error reading HTML template", "text/plain";
        }

        res.set_header("Content-Type", "text/html");
        res.write(modifiedHTML);
        res.end();
        });


    CROW_CATCHALL_ROUTE(app)
        ([]() {
        return "Wrong Route";
            });

    // Here we can set the log level (DEBUG, INFO, WARNING, ERROR, CRITICAL
    crow::logger::setLogLevel(crow::LogLevel::Debug);

    // App methods chain
    app.bindaddr("192.168.1.66")
        .port(18080)
        .server_name("CrowCpp")
        .multithreaded()
        .ssl_file("C:\\Users\\User\\Desktop\\TFG\\ServerCrow\\ServerCrow\\ssl\\server.crt", "C:\\Users\\User\\Desktop\\TFG\\ServerCrow\\ServerCrow\\ssl\\server.key") // TODO: Put relative path
        .run();

}