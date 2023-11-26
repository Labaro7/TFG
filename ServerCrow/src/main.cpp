#include "..\headers\server.h"
#include "..\headers\domain.h"
#include <tuple>

const char* TABLES_PRICES_PLACEHOLDER = "<!-- PLACEHOLDER: TABLES PRICES -->";
const char* TABLE_NUMBER_PLACEHOLDER = "<!-- PLACEHOLDER: TABLE NUMBER -->"; // It is char* because of strlen()
const char* TICKET_PRODUCTS_PLACEHOLDER = "<!-- PLACEHOLDER: TICKET PRODUCTS -->";
const char* TICKET_BILL_PLACEHOLDER = "<!-- PLACEHOLDER: TABLE BILL -->";
const char* FOURTH_ROW_BUTTONS_PLACEHOLDER = "<!-- PLACEHOLDER: FOURTH ROW BUTTONS -->";
int N_FOURTH_ROW_BUTTONS = 5;

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
std::string insertDataInPlaceHolders(std::ifstream* file, const std::string tableNumberPlaceholder, const int n_table, const std::vector<Product> products, Server& server){
    // Data to insert into HTML
    // 1. Table number
    // 2. Fourth row buttons
    // 3. Restaurant products
    // 4. Ticket products
    // 5. Ticket bill

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

    // 2. HTML with fourth row buttons
    for (int i = 1; i <= N_FOURTH_ROW_BUTTONS; i++) {
        ss << "<div class='fourthRowButton' id='fourthRowButton" << i << "' data-number='" << i << "' onclick='displayMenu(this)'>" << i << "</div>" << std::endl;
    }

    std::string fourthRowButtonsHTML = ss.str();
    ss.str(""); // Important to clear here

    // 2. Insert previous HTML piece with the fourth row buttons into HTML
    size_t fourthRowButtonsPlaceholderPos = contentHTML.find(FOURTH_ROW_BUTTONS_PLACEHOLDER);
    if (fourthRowButtonsPlaceholderPos != std::string::npos) {
        contentHTML.replace(fourthRowButtonsPlaceholderPos, strlen(FOURTH_ROW_BUTTONS_PLACEHOLDER), fourthRowButtonsHTML);
    }

    // 3. Get the data of products currently added to the database
    std::vector<std::pair<std::string, float>> productsInfo;

    for (const auto p : products) {
        productsInfo.push_back({ p.name, p.price });
    }

    // 3. Generate HTML piece with the products of the restaurant
    std::string productListHTML;

    using product = std::tuple<Product, std::vector<std::pair<std::string, int>>>;

    for (int i = 0; i < server.getRestaurantPagesSize(); i++) {
        const productsMenus_t data = server.getDataFromPage(i);
        for (const auto& p : data) {
            std::string product_name = std::get<0>(p).name;
            double product_price = std::get<0>(p).price;
            auto list = std::get<1>(p);

            // Is product
            if (std::get<1>(p).empty()) {
                if (product_name.size() <= 15)
                    ss << std::fixed << std::setprecision(2) << "<li class ='grid-product' onclick='addProductToTicket(this)'><div class='products-names'>" << product_name << "</div>" << "<div class='products-prices'>" << product_price << "</div></li>" << std::endl; // We use this because std::to_string() eliminates the precision set
                else
                    ss << std::fixed << std::setprecision(2) << "<li class ='grid-product-small' onclick='addProductToTicket(this)'><div class='products-names'>" << product_name << "</div>" << "<div class='products-prices'>" << product_price << "</div></li>" << std::endl;
            }

            // Is desployable
            else {
                ss << "<li class='grid-deployable' onclick='openDeployable(this)'>" << product_name << "</li>";

                for (const auto& q : std::get<1>(p)) {
                    ss << std::fixed << std::setprecision(2) << "<li class='deployable-product' data-name='" << product_name << "' onclick='addProductToTicket(this)'><div class='products-names'>" << q.name << "</div><div class='products-prices'>" << q.price << "</div></li>";
                }

                ss << "</li>" << std::endl;
            }

            productListHTML = ss.str();
            ss.str("");

            // 3. Insert HTML piece with the products of the restaurant into HTML
            std::stringstream ss2;
            ss2 << "<!-- PLACEHOLDER: LIST OF PRODUCTS " << i+1 << " -->" << std::endl; // i+1 because the fourth row starts counting in 1
            std::string PRODUCT_LIST_PLACEHOLDER = ss2.str();
            ss2.str("");

            size_t productListPlaceholderPos = contentHTML.find(PRODUCT_LIST_PLACEHOLDER.c_str());
            if (productListPlaceholderPos != std::string::npos) {
                contentHTML.insert(productListPlaceholderPos, productListHTML);
            }
        }
    }


    // 4. Get ticket products and ticket bill
    const product_unordered_map ticketProducts = server.getTableByNumber(n_table).products;
    double bill = server.getTableByNumber(n_table).bill;
    double discount = server.getTableByNumber(n_table).discount;

    for (const auto& p : ticketProducts) {
        ss << std::fixed << std::setprecision(2) << 
            "<li class='ticketProduct'>" << std::endl << "<div class='productTimes'>x" << p.second << "</div><div class='productNames'>" << p.first.name << "</div><div class='productPrices'>" << p.first.price << "</div></li>" << std::endl;
    }
    
    std::string ticketProductsHTML = ss.str();
    ss.str("");

    // 4. Insert previous HTML piece with the ticket of the table into HTML
    size_t ticketProductsPlaceholderPos = contentHTML.find(TICKET_PRODUCTS_PLACEHOLDER);
    if (ticketProductsPlaceholderPos != std::string::npos) {
        contentHTML.replace(ticketProductsPlaceholderPos, strlen(TICKET_PRODUCTS_PLACEHOLDER), ticketProductsHTML);
    }

    // 5. Generate HTML piece with the ticket bill
    ss << "<div id='bill'><div id='currency'>$</div><div id='price'>" << bill * (1 - (discount / 100));
    ss << "</div><div id='discount'><div id='minus'>-</div><div id='discountValue'>" << discount << "</div><div id='percentage'>%</div></div></div>" << std::endl;

    std::string ticketBillHTML = ss.str();
    ss.str("");
    size_t ticketBillPlaceholderPos = contentHTML.find(TICKET_BILL_PLACEHOLDER);
    contentHTML.replace(ticketBillPlaceholderPos, strlen(TICKET_BILL_PLACEHOLDER), ticketBillHTML);


    return contentHTML;
}

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
        file.close();
        std::string index = ss.str();

        //std::cout << "RECEIVED" << std::endl;
        //std::cout << req.body << std::endl;

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
        
        //std::cout << n_table << std::endl;

        for (const auto& object : added) {
            int times = object["times"].i();
            Product p(object["name"].s(), object["price"].d());

            server.saveTableProduct(t, p);

            //std::cout << times << std::endl;
            //std::cout << p.name << " " << p.price << std::endl << std::endl;
        }


        // TODO: Change the response to the client
        res.set_header("Content-Type", "text/html");
        res.write(index);
        res.end();
            });

    CROW_CATCHALL_ROUTE(app)
        ([]() {
        return "Wrong Route";
            });

    // App methods chain
    app.bindaddr("192.168.1.66")
        .port(18080)
        .server_name("CrowCpp")
        .multithreaded()
        .ssl_file("C:\\Users\\User\\Desktop\\TFG\\ServerCrow\\ServerCrow\\ssl\\server.crt", "C:\\Users\\User\\Desktop\\TFG\\ServerCrow\\ServerCrow\\ssl\\server.key") // TODO: Put relative path
        .run();
}