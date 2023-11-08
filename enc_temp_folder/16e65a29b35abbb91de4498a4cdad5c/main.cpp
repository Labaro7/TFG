#include "..\headers\server.h"
#include "..\headers\domain.h"
#include <tuple>

const char* TABLE_NUMBER_PLACEHOLDER = "<!-- PLACEHOLDER: TABLE NUMBER -->"; // It is char* because of strlen()
const char* PRODUCT_LIST_1_PLACEHOLDER = "<!-- PLACEHOLDER: LIST OF PRODUCTS -->"; // Same
const char* TICKET_PRODUCTS_PLACEHOLDER = "<!-- PLACEHOLDER: TICKET PRODUCTS -->";

std::string insertDataInPlaceHolders(std::ifstream* file, const std::string tableNumberPlaceholder, const int n_table, const std::string productListPlaceholder, const std::vector<Product> products, Server* server){
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
    ss << "<div id='numTable'>Table: " << n_table << "</div>" << std::endl;
    std::string tableNumberHTML = ss.str();

    // Important to clear here or the table number will be shown in the next html piece
    ss.str("");

    // Insert previous HTML piece with the table number into HTML
    size_t tableNumberPlaceholderPos = contentHTML.find(TABLE_NUMBER_PLACEHOLDER);
    if (tableNumberPlaceholderPos != std::string::npos) {
        contentHTML.replace(tableNumberPlaceholderPos, strlen(TABLE_NUMBER_PLACEHOLDER), tableNumberHTML);
    }

    // Get the data of products currently added to the database
    std::vector<std::pair<std::string, float>> productsInfo;

    for (const auto p : products) {
        productsInfo.push_back({ p.name, p.price });
    }

    // Generate HTML piece with current tables
    std::string tableListHTML;

    using productsMenus_t = std::vector<std::tuple<std::string, int, std::vector<std::pair<std::string, int>>>>;
    using product = std::tuple<std::string, int, std::vector<std::pair<std::string, int>>>;

    const productsMenus_t data = server->getDataFromPage(0);
    for (const auto& p : data) {
        std::string product_name = std::get<0>(p);
        int product_price = round(std::get<1>(p) * 100.0) / 100.0;;
        auto list = std::get<2>(p);

        // Is product
        if (std::get<2>(p).empty()) {
            std::cout << product_name << std::endl;

            ss << std::fixed << std::setprecision(2) << "<li class ='grid-product' onclick='addProductToTicket(this)'>" << product_name << "<br>" << "<div class='prices'>" << product_price << "</div></li > " << std::endl; // We use this because std::to_string() eliminates the precision set
        }

        // Is desployable
        else {

        }
    }

    /*for (const auto p : productsInfo) {
        std::string name_ = p.first;
        double price_ = round(p.second * 100.0) / 100.0; // Round the price to two decimals
        ss << std::fixed << std::setprecision(2) << "<li class ='grid-product' onclick='addProductToTicket(this)'>" << p.first << "<br>" << "<div class='prices'>" << price_ << "</div></li > " << std::endl; // We use this because std::to_string() eliminates the precision set
    }*/

    tableListHTML = ss.str();
    ss.str("");

    // Insert HTML piece with current tables into webpage HTML
    size_t productListPlaceholderPos = contentHTML.find(PRODUCT_LIST_1_PLACEHOLDER);
    if (productListPlaceholderPos != std::string::npos) {
        contentHTML.replace(productListPlaceholderPos, strlen(PRODUCT_LIST_1_PLACEHOLDER), tableListHTML);
    }


    // TICKET
    const std::unordered_map<std::string, int> ticketProducts = server->getTableByNumber(69).products;
    std::cout << "PRODUCTS" << std::endl;
    for (const auto p : ticketProducts) {
        std::cout << p.first << std::endl;
        ss << "<li class='ticketProduct'>" << "x" << p.second << " " << p.first << "</li>" << std::endl;
    }
    
    std::string ticketProductsHTML = ss.str();

    // Insert previous HTML piece with the ticket of the table into HTML
    size_t ticketProductsPlaceholderPos = contentHTML.find(TICKET_PRODUCTS_PLACEHOLDER);
    if (ticketProductsPlaceholderPos != std::string::npos) {
        contentHTML.replace(ticketProductsPlaceholderPos, strlen(TICKET_PRODUCTS_PLACEHOLDER), ticketProductsHTML);
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

        Product p;
        std::unordered_map<std::string, int> ps;
        Table t(69, 1, ps, 10.0, 0.0);
        server.saveTable(&t);

        p.name = "a";
        p.price = 10;
        server.getTableByNumber(69).products[p.name] = 1;
        server.saveProduct(&p);
        server.database()->saveTableProduct(&t, &p);

        p.name = "b";
        p.price = 20;
        server.getTableByNumber(69).products[p.name] = 2;
        server.saveProduct(&p);
        server.database()->saveTableProduct(&t, &p);

        p.name = "c";
        p.price = 30;
        server.getTableByNumber(69).products[p.name] = 3;
        server.saveProduct(&p);
        server.database()->saveTableProduct(&t, &p);

        return page;
        });

    // TODO: Fix the 505 error when not inputting table and accepting
    CROW_ROUTE(app, "/table")([&server](const crow::request& req, crow::response& res) {
        auto page = crow::mustache::load_text("table.html");
        std::string n_table = req.url_params.get("tableInput"); // This has to match the name of the input that is being sent to get its value correctly.
        Table t(stoi(n_table));
        std::vector<Product> products = server.getProducts();

        server.saveTable(&t);

        // TODO: Put relative path
        std::ifstream file("C:\\Users\\User\\Desktop\\TFG\\ServerCrow\\ServerCrow\\templates\\table.html");
        std:: string modifiedHTML = insertDataInPlaceHolders(&file, TABLE_NUMBER_PLACEHOLDER, stoi(n_table), PRODUCT_LIST_1_PLACEHOLDER, products, &server);

        // TODO: Change error handling
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
    crow::logger::setLogLevel(crow::LogLevel::Warning);

    // App methods chain
    app.bindaddr("192.168.1.66")
        .port(18080)
        .server_name("CrowCpp")
        .multithreaded()
        .ssl_file("C:\\Users\\User\\Desktop\\TFG\\ServerCrow\\ServerCrow\\ssl\\server.crt", "C:\\Users\\User\\Desktop\\TFG\\ServerCrow\\ServerCrow\\ssl\\server.key") // TODO: Put relative path
        .run();

}