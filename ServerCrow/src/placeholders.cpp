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
                    if (q.price) {
                        ss << std::fixed << std::setprecision(2) << "<li class='deployable-product' data-name='" << product_name << "' onclick='addProductToTicket(this)'><div class='products-names'>" << q.name << "</div><div class='products-prices'>" << q.price << "</div></li>";
                    }
                }

                ss << "</li>" << std::endl;
            }

            productListHTML = ss.str();
            ss.str("");

            // 3. Insert HTML piece with the products of the restaurant into HTML
            std::stringstream ss2;
            ss2 << "<!-- PLACEHOLDER: LIST OF PRODUCTS " << i + 1 << " -->" << std::endl; // i+1 because the fourth row starts counting in 1
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