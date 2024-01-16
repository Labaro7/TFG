#include "..\headers\placeholders.h"

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
        ss << "<li class='table'><a class='tableNumber' href='" << TABLE_NUMBER_HREF << t.n_table << "'>Table: " << t.n_table << "</a><div class='tablePrice'>$" << t.bill * (1 - (t.discount / 100)) << "</div></li>" << std::endl;
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
std::string insertDataInPlaceHolders(std::ifstream* file, const std::string tableNumberPlaceholder, const int n_table, const std::vector<Product> products, Server& server) {
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
        ss << "<div class='productsPagesButton' id='productsPagesButton" << i << "' data-number='" << i << "' onclick='displayMenu(this)'>" << i << "</div>" << std::endl;
    }

    std::string pagesButtonsHTML = ss.str();
    ss.str(""); // Important to clear here

    // 2. Insert previous HTML piece with the fourth row buttons into HTML
    size_t pagesButtonsPlaceholderPos = contentHTML.find(PAGES_BUTTONS_PLACEHOLDER);
    if (pagesButtonsPlaceholderPos != std::string::npos) {
        contentHTML.replace(pagesButtonsPlaceholderPos, strlen(PAGES_BUTTONS_PLACEHOLDER), pagesButtonsHTML);
    }

    // 3. Get the data of products currently added to the database
    std::vector<std::pair<std::string, float>> productsInfo;

    for (const auto p : products) {
        productsInfo.push_back({ p.name, p.price });
    }

    // 3. Generate HTML piece with the products of the restaurant
    std::string productListHTML;

    std::vector<page_t> pages = server.getDataFromPages();
    int i = 0;
    for (const auto& page : pages) {
        for (const auto& p : page) {
            std::string product_name = p.first.name;
            double product_price = p.first.price;
            std::string product_color = p.first.color;
            int product_deployable = p.first.deployable;
            int product_id = server.getProductIdByName(product_name);
            auto list = p.second;

            // Is product
            if (p.first.deployable == 0 && p.second.empty()) {
                if (product_name.size() <= 15)
                    ss << std::fixed << std::setprecision(2) << "<li class ='grid-product' onclick='addProductToTicket(this)' style='background-color:" << product_color << ";'><div class='products-names'>" << product_name << "</div>" << "<div class='products-prices'>" << product_price << "</div></li>" << std::endl; // We use this because std::to_string() eliminates the precision set
                else
                    ss << std::fixed << std::setprecision(2) << "<li class ='grid-product-small' onclick='addProductToTicket(this)' style='background-color:" << product_color << ";'><div class='products-names'>" << product_name << "</div>" << "<div class='products-prices'>" << product_price << "</div></li>" << std::endl;
            }

            // Is desployable
            else {
                if (p.first.deployable == 0 && p.first.price == 0) {
                    ss << "<li class='grid-deployable' onclick='openDeployable(this)' style='background-color:" << product_color << ";' data-id='" << product_id << "'>" << product_name << "</li>";
                }
                
                for (const auto& q : p.second) {
                    if (q.price) {
                        product_color = q.color;
                        product_deployable = q.deployable;
                        ss << std::fixed << std::setprecision(2) << "<li class='deployable-product' data-deployable='" << product_id << "' onclick='addProductToTicket(this)' style = 'background-color:" << product_color << ";'><div class = 'products-names'>" << q.name << "</div><div class = 'products-prices'>" << q.price << "</div></li>";
                    }
                }

                ss << "</li>" << std::endl;
            }

            productListHTML = ss.str();
            ss.str("");

            // 3. Insert HTML piece with the products of the restaurant into HTML
            std::string changedPlaceholder = PRODUCT_LIST_PLACEHOLDER;
            changedPlaceholder.replace(35, 1, std::to_string(i+1));

            size_t productListPlaceholderPos = contentHTML.find(changedPlaceholder);
            if (productListPlaceholderPos != std::string::npos) {
                contentHTML.insert(productListPlaceholderPos, productListHTML);
            }
        }

        i++;
    }

    // 4. Get ticket products and ticket bill
    const product_unordered_map ticketProducts = server.getTableByNumber(n_table).products;
    double bill = server.getTableByNumber(n_table).bill;
    double discount = server.getTableByNumber(n_table).discount;

    for (const auto& p : ticketProducts) {
        ss << std::fixed << std::setprecision(2) <<
            "<li class='ticketProduct'>" << std::endl << "<div class='productTimes'>x" << p.second << " </div><div class='productNames'>" << p.first.name << "</div><div class='productPrices'>" << p.first.price << "</div><div class='productTotalPrices'>" << p.second * p.first.price << "</div></li>" << std::endl;
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

// add.html
std::string insertDataInPlaceHolders2(std::ifstream* file, const std::string& productListPlaceHolder, Server& server) {
    // Data to insert into HTML
    // 1. Current products
    // 1.1 Pages buttons
    // 1.2 Current products
    // 2. 
    // 3. 
    // 4. 
    // 5. 

    // Read the content of the HTML into contentHTML
    if (!file->is_open()) {
        return "";
    }

    std::stringstream ssHTML;
    ssHTML << file->rdbuf();
    file->close();
    std::string contentHTML = ssHTML.str();

    std::stringstream ss;

    // 1.1. HTML with fourth row buttons
    for (int i = 1; i <= N_FOURTH_ROW_BUTTONS; i++) {
        ss << "<div class='productsPagesButton' id='productsPagesButton" << i << "' data-number='" << i << "' onclick='displayMenu(this)'>" << i << "</div>" << std::endl;
    }

    std::string pagesButtonsHTML = ss.str();
    ss.str(""); // Important to clear here

    // 1.1. Insert previous HTML piece with the fourth row buttons into HTML
    size_t fourthRowButtonsPlaceholderPos = contentHTML.find(PAGES_BUTTONS_PLACEHOLDER);
    if (fourthRowButtonsPlaceholderPos != std::string::npos) {
        contentHTML.replace(fourthRowButtonsPlaceholderPos, strlen(PAGES_BUTTONS_PLACEHOLDER), pagesButtonsHTML);
    }

    // 1.2. Get the data of products currently added to the database
    std::vector<Product> products = server.getProducts();
    std::vector<std::pair<std::string, float>> productsInfo;

    for (const auto p : products) {
        productsInfo.push_back({ p.name, p.price });
    }

    // 1.2. Generate HTML piece with the products of the restaurant
    std::string productListHTML;

    std::vector<page_t> pages = server.getDataFromPages();
    int i = 0;
    for(const auto& page : pages) {
        for(const auto& p : page) {
            std::string product_name = p.first.name;
            double product_price = p.first.price;
            int product_deployable = p.first.deployable;

            // Is product
            if (p.first.deployable == 0 && p.second.empty()) {
                if (product_name.size() <= 15)
                    ss << std::fixed << std::setprecision(2) << "<li class ='grid-product'><div class='products-names'>" << product_name << "</div>" << "<div class='products-prices'>" << product_price << "</div></li>" << std::endl; // We use this because std::to_string() eliminates the precision set
                else
                    ss << std::fixed << std::setprecision(2) << "<li class ='grid-product-small''><div class='products-names'>" << product_name << "</div>" << "<div class='products-prices'>" << product_price << "</div></li>" << std::endl;
            }

            // Is desployable
            else {
                if (p.first.deployable == 0 && p.first.price == 0) {
                    int product_id = server.getProductIdByName(product_name);

                    ss << "<li class='grid-deployable' data-id='" << product_id << "' onclick = 'openDeployable(this)'>" << product_name << "</li>";

                    for (const auto& q : p.second) {
                        if (q.price) {
                            ss << std::fixed << std::setprecision(2) << "<li class='deployable-product' data-name='" << product_name << "'><div class='products-names'>" << q.name << "</div><div class='products-prices'>" << q.price << "</div></li>";
                        }
                    }

                    ss << "</li>" << std::endl;
                }
            }

            productListHTML = ss.str();
            ss.str("");

            // 1.2. Insert HTML piece with the products of the restaurant into HTML
            std::string changedPlaceholder = PRODUCT_LIST_PLACEHOLDER;
            changedPlaceholder.replace(35, 1, std::to_string(i + 1));

            size_t productListPlaceholderPos = contentHTML.find(changedPlaceholder);
            if (productListPlaceholderPos != std::string::npos) {
                contentHTML.insert(productListPlaceholderPos, productListHTML);
            }
        }

        i++;
    }

    return contentHTML;
}