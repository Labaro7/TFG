#include "..\headers\server.h"
#include "..\headers\domain.h"
#include <tuple>

const char* TABLES_PRICES_PLACEHOLDER = "<!-- PLACEHOLDER: TABLES PRICES -->";
const char* TABLE_NUMBER_PLACEHOLDER = "<!-- PLACEHOLDER: TABLE NUMBER -->"; // It is char* because of strlen()
const char* PRODUCT_LIST_1_PLACEHOLDER = "<!-- PLACEHOLDER: LIST OF PRODUCTS -->"; // Same
const char* TICKET_PRODUCTS_PLACEHOLDER = "<!-- PLACEHOLDER: TICKET PRODUCTS -->";
const char* TICKET_BILL_PLACEHOLDER = "<!-- PLACEHOLDER: TABLE BILL -->";


// index.html
std::string insertDataInPlaceHolders(std::ifstream* file, const std::string tablesPricesPlaceholder, Server& server) {
    std::stringstream ssHTML;
    ssHTML << file->rdbuf();
    file->close();
    std::string contentHTML = ssHTML.str();

    std::vector<Table> tables = server.database()->getTables();

    // Make a HTML with the tables prices
    std::ostringstream ss;

    for (const auto& t : tables) {
        std::cout << t.bill << std::endl;
        ss << "<li class='table'><a class='tableNumber' href='https://192.168.1.66:18080/table?tableInput=" << t.n_table << "'>Table: " << t.n_table << "</a><div class='tablePrice'>$" << t.bill * (1 - (t.discount / 100)) << "</div></li>" << std::endl;
    }   

    std::string tablesPricesHTML = ss.str();
    ss.str(""); // Important to clear here

    // Insert previous HTML piece with the table number into HTML
    size_t tablesPricesPlaceholderPos = contentHTML.find(TABLES_PRICES_PLACEHOLDER);
    if (tablesPricesPlaceholderPos != std::string::npos) {
        contentHTML.replace(tablesPricesPlaceholderPos, strlen(TABLES_PRICES_PLACEHOLDER), tablesPricesHTML);
    }

    return contentHTML;
}

// table.html
std::string insertDataInPlaceHolders(std::ifstream* file, const std::string tableNumberPlaceholder, const int n_table, const std::string productListPlaceholder, const std::vector<Product> products, Server& server){
    // Data to insert into HTML
    // 1. Table number
    // 2. Restaurant products
    // 3. Ticket products
    // 4. Ticket bill

    // Read the content of the HTML into contentHTML
    if (!file->is_open()) {
        return "";
    }
    
    std::stringstream ssHTML;
    ssHTML << file->rdbuf();
    file->close();
    std::string contentHTML = ssHTML.str();

    std::ostringstream ss;

    // 1. HTML with the table number
    ss << "<div id='numTable'>Table: " << n_table << "</div>" << std::endl;
    std::string tableNumberHTML = ss.str();
    ss.str(""); // Important to clear here

    // 1. Insert previous HTML piece with the table number into HTML
    size_t tableNumberPlaceholderPos = contentHTML.find(TABLE_NUMBER_PLACEHOLDER);
    if (tableNumberPlaceholderPos != std::string::npos) {
        contentHTML.replace(tableNumberPlaceholderPos, strlen(TABLE_NUMBER_PLACEHOLDER), tableNumberHTML);
    }

    // 2. Get the data of products currently added to the database
    std::vector<std::pair<std::string, float>> productsInfo;

    for (const auto p : products) {
        productsInfo.push_back({ p.name, p.price });
    }

    // 2. Generate HTML piece with the products of the restaurant
    std::string productListHTML;

    using product = std::tuple<Product, std::vector<std::pair<std::string, int>>>;

    const productsMenus_t data = server.getDataFromPage(0);
    for (const auto& p : data) {
        std::string product_name = std::get<0>(p).name;
        int product_price = round(std::get<0>(p).price * 100.0) / 100.0;;
        auto list = std::get<1>(p);

        // Is product
        if (std::get<1>(p).empty()) {
            ss << std::fixed << std::setprecision(2) << "<li class ='grid-product' onclick='addProductToTicket(this)'>" << product_name << "<br>" << "<div class='prices'>" << product_price << "</div></li>" << std::endl; // We use this because std::to_string() eliminates the precision set
        }

        // Is desployable
        else {

        }
    }

    productListHTML = ss.str();
    ss.str("");

    // 2. Insert HTML piece with the products of the restaurant into HTML
    size_t productListPlaceholderPos = contentHTML.find(PRODUCT_LIST_1_PLACEHOLDER);
    if (productListPlaceholderPos != std::string::npos) {
        contentHTML.replace(productListPlaceholderPos, strlen(PRODUCT_LIST_1_PLACEHOLDER), productListHTML);
    }

    // 3. Get ticket products and ticket bill
    const product_unordered_map ticketProducts = server.getTableByNumber(n_table).products;
    double bill = server.getTableByNumber(n_table).bill;
    double discount = server.getTableByNumber(n_table).discount;

    for (const auto& p : ticketProducts) {
        ss << "<li class='ticketProduct'>" << "x" << p.second << " " << p.first.name << " | " << p.first.price << "</li>" << std::endl;
    }
    
    std::string ticketProductsHTML = ss.str();
    ss.str("");

    // 3. Insert previous HTML piece with the ticket of the table into HTML
    size_t ticketProductsPlaceholderPos = contentHTML.find(TICKET_PRODUCTS_PLACEHOLDER);
    if (ticketProductsPlaceholderPos != std::string::npos) {
        contentHTML.replace(ticketProductsPlaceholderPos, strlen(TICKET_PRODUCTS_PLACEHOLDER), ticketProductsHTML);
    }

    // 4. Generate HTML piece with the ticket bill
    ss << "<div id='bill'><div id='currency'>$</div><div id='price'>" << bill * (1 - (discount/100)) << "</div></div>" << std::endl;
    std::string ticketBillHTML = ss.str();
    ss.str("");
    size_t ticketBillPlaceholderPos = contentHTML.find(TICKET_BILL_PLACEHOLDER);
    contentHTML.replace(ticketBillPlaceholderPos, strlen(TICKET_BILL_PLACEHOLDER), ticketBillHTML);


    return contentHTML;
}

int main() {
    crow::SimpleApp app;
    Server server;
    
    server.dropAllTables();
    server.initialize();

    CROW_ROUTE(app, "/")([&server](const crow::request& req, crow::response& res) {
        auto page = crow::mustache::load_text("index.html");

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
        std:: string modifiedHTML = insertDataInPlaceHolders(&file, TABLE_NUMBER_PLACEHOLDER, stoi(n_table), PRODUCT_LIST_1_PLACEHOLDER, products, server);

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