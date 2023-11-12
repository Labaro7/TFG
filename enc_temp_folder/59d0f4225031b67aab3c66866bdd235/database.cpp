#include "../headers/dataBase.h"
#include "../headers/crow_all.h"
#include "../headers/domain.h"

Database::Database() : stmt(0), pstmt(0) {
    try{
        driver = get_driver_instance();
        con = driver->connect(server, username, password);
        stmt = con->createStatement();
        con->setSchema(dataBaseName);
    }
    catch (sql::SQLException e) {
        std::cerr << "Could not connect to server. Error message: " << e.what() << std::endl;
        exit(1);
    }
}

Database::~Database() {
    // These must be deleted when they are not going to be used anymore
    delete con; 
    delete stmt;
    delete pstmt;
}

// Get attributes
sql::Driver* Database::getDriver() { return driver; }
sql::Connection* Database::getCon() { return con; }
sql::Statement* Database::getStmt() { return stmt; }
sql::PreparedStatement* Database::getPstmt() { return pstmt; }


// ------------------------------- MySQL queries ------------------------------- //

// Database
void Database::MySqlCreateDatabase(std::string name) {
    try {
        sql::Statement* stmt = con->createStatement();
        stmt->execute("CREATE DATABASE " + name);
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not create database. Error message: " << e.what();
    }
}

void Database::MySqlDropDatabase(std::string name) {
    try {
        sql::Statement* stmt = con->createStatement();
        stmt->execute("DROP DATABASE " + name);
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not drop database. Error message: " << e.what();
    }
}

void Database::MySqlUseDatabase(std::string name) {
    try {
        sql::Statement* stmt = con->createStatement();
        stmt->execute("USE " + name);
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not use database. Error message: " << e.what();
    }
}

void Database::MySqlSaveChangesToDataBase() {
    try {
        sql::Statement* stmt = con->createStatement();
        stmt->execute("COMMIT");
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not save changes to the database. Error message: " << e.what();
    }
}

// Table
void Database::MySqlCreateTable(std::string name, std::string definition) {
    try {
        sql::Statement* stmt = con->createStatement();
        stmt->execute("CREATE TABLE " + name + " (" + definition + ")");
        CROW_LOG_INFO << "MySQL table created: " << name;
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not create table. Error message: " << e.what();
    }
}

void Database::MySqlDropTable(std::string name) {
    try {
        sql::Statement* stmt = con->createStatement();
        stmt->execute("DROP TABLE IF EXISTS " + name);
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not drop table. Error message: " << e.what();
    }
}

void Database::MySqlModifyTable(std::string name, std::string modification) {
    try {
        sql::Statement* stmt = con->createStatement();
        stmt->execute("ALTER TABLE " + name + " " + modification);
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not modify table. Error message: " << e.what();
    }
}

void Database::MySqlEmptyTable(std::string name) {
    try {
        sql::Statement* stmt = con->createStatement();
        stmt->execute("DELETE FROM " + name);
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not empty table. Error message: " << e.what();
    }
}


// ------------------------------- /MySQL queries ------------------------------- //


void Database::initializeEmployeesTable() {
    std::vector<std::string> names = {"Adrian", "Pepe", "Ana", "Nati", "Fermin", "Pablo", "Cristian"};
    std::vector<int> levels = {3, 2, 2, 4, 4, 2, 1};
    std::vector<std::string> starts = { "2023 - 10 - 04 15:30 : 45" };
    std::vector<std::string> finishes = { "2023-10-04 15:30:45" };

    for (int i = 0; i < names.size(); i++) {
        Employee e = { names[i], levels[i], starts[0], finishes[0] };
        saveEmployee(e);
    }
}

void Database::initializeProductsTable() {
    std::vector<std::string> productNames = { "DRINKS", "BEERS", "CAFETERIA",
    "WHITE WINES", "RED WINES", "ROSSE WINES",
    "SPARKLING WINES", "COCKTAILS", "RUM",
    "GIN", "WHISKY", "VODKA" };

    std::vector<double> prices = { 10.99, 15.99, 8.50,
    10.99, 15.99, 8.50,
    10.99, 15.99, 8.50,
    10.99, 15.99, 8.50 };

    for (size_t i = 0; i < productNames.size(); i++) {
        Product product = { productNames[i], prices[i] };
        saveProduct(product);
    }
}

void Database::initializeOrdersTable() {
    std::vector<std::string> times = { "2023-10-04 15:45:00", "2023-10-05 12:30:00", "2023-10-06 18:15:00" };
    std::vector<std::string> messages = { "Order 1", "Order 2", "Order 3" };

    for (size_t i = 0; i < times.size(); i++) {
        Order order = { times[i], messages[0] };

        saveOrder(order);
    }
}

void Database::initializeIngredientsTable() {
    std::vector<std::string> ingredientNames = { "Salt", "Pepper", "Sugar" };

    for (const auto& name : ingredientNames) {
        Ingredient ingredient = { name };
        saveIngredient(ingredient);
    }
}

void Database::initializeAllergensTable() {
    std::vector<std::string> allergenNames = { "Gluten", "Peanuts", "Dairy" };

    for (const auto& name : allergenNames) {
        Allergen allergen = { name };
        saveAllergen(allergen);
    }
}

void Database::initialize() {
    // Create the tables to define the domain
    // TODO: Make n_table primary key so there are no duplicate tables
    MySqlCreateTable("tables", "table_id INT AUTO_INCREMENT PRIMARY KEY, n_table INT, n_clients INT, bill DOUBLE, discount DOUBLE");
    MySqlCreateTable("employees", "employee_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45), level INT, start VARCHAR(45), finish VARCHAR(45)");
    MySqlCreateTable("products", "product_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45), price DOUBLE");
    MySqlCreateTable("orders", "order_id INT AUTO_INCREMENT PRIMARY KEY, time VARCHAR(45), message VARCHAR(45)");
    MySqlCreateTable("ingredients", "ingredient_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45)");
    MySqlCreateTable("allergens", "allergen_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45)");
    MySqlCreateTable("restaurants", "restaurant_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45)");

    // TODO: Create Junction tables
    // Create the junction tables to the ManyToMany relationships
    MySqlCreateTable("tableproduct", "table_id INT, product_id INT, PRIMARY KEY(table_id, product_id), FOREIGN KEY(table_id) REFERENCES tables(table_id), FOREIGN KEY(product_id) REFERENCES products(product_id)");
    MySqlCreateTable("productorder", "product_id INT, order_id INT, PRIMARY KEY(product_id, order_id), FOREIGN KEY(product_id) REFERENCES products(product_id), FOREIGN KEY(order_id) REFERENCES orders(order_id)");
    MySqlCreateTable("productingredient", "product_id INT, ingredient_id INT, PRIMARY KEY(product_id, ingredient_id), FOREIGN KEY(product_id) REFERENCES products(product_id), FOREIGN KEY(ingredient_id) REFERENCES ingredients(ingredient_id)");
    //MySqlCreateDatabase("ingredientallergen, product_id INT, ingredient_id INT, PRIMARY KEY(product_id, ingredient_id), FOREIGN KEY(product_id) REFERENCES products(product_id), FOREIGN KEY(ingredient_id) REFERENCES ingredients(ingredient_id)");
    
    // Populate the tables
    initializeEmployeesTable();
    initializeProductsTable();
    initializeIngredientsTable();
    initializeAllergensTable();
    initializeOrdersTable();

}

void Database::dropAllTables() {
    try {
        // We must drop first the tables that have a reference to another
        stmt->execute("DROP TABLE IF EXISTS tableproduct;");
        stmt->execute("DROP TABLE IF EXISTS productingredient;");
        stmt->execute("DROP TABLE IF EXISTS productorder;");

        stmt->execute("DROP TABLE IF EXISTS orders;");
        stmt->execute("DROP TABLE IF EXISTS ingredients;");
        stmt->execute("DROP TABLE IF EXISTS allergens;");
        stmt->execute("DROP TABLE IF EXISTS employees;");
        stmt->execute("DROP TABLE IF EXISTS products;");
        stmt->execute("DROP TABLE IF EXISTS tables;");
        stmt->execute("DROP TABLE IF EXISTS restaurants;");

        CROW_LOG_INFO << "[REMOVED] All tables dropped.";
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not drop tables. Error message: " << e.what();
    }
}


// Save
void Database::saveTable(const Table& table) {
    try {
        int n_table = table.n_table;
        int n_clients = table.n_clients;
        double bill = 0; // The bill is updated by saveTableProduct
        double discount = table.discount;

        Table t = getTableByNumber(n_table);
        if (t.isEmpty()) {
            std::ostringstream oss;
            oss << n_table << "," << n_clients << "," << bill << "," << discount;
            std::string values = oss.str();

            pstmt = con->prepareStatement("INSERT INTO tables(n_table, n_clients, bill, discount) VALUES(?,?,?,?)");
            pstmt->setInt(1, n_table);
            pstmt->setInt(2, n_clients);
            pstmt->setDouble(3, bill);
            pstmt->setDouble(4, discount);
            pstmt->execute();

            CROW_LOG_INFO << "[ADDED] Table " << n_table <<
                " with " << n_clients <<
                " clients inserted into tables.";
        }
        else {
            CROW_LOG_WARNING << "[EXCEPTION] Table is already in the database.";
        }
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not save table. Error message: " << e.what();
    }
}

void Database::saveEmployee(const Employee& employee) {
    try {
        std::string name = employee.name;
        int level = employee.level;
        std::string start = employee.start;
        std::string finish = employee.finish;

        if (getEmployeeByName(name).isEmpty()) {
            std::ostringstream oss;
            oss << name << "," << level << "," << start << "," << finish;
            std::string values = oss.str();

            pstmt = con->prepareStatement("INSERT INTO employees(name, level, start, finish) VALUES(?,?,?,?)");
            pstmt->setString(1, name);
            pstmt->setInt(2, level);
            pstmt->setString(3, start);
            pstmt->setString(4, finish);
            pstmt->execute();

            CROW_LOG_INFO << "[ADDED] Employee " << name <<
                " with level " << level <<
                " starting at " << start <<
                " inserted into employees.";
        }
        else {
            CROW_LOG_WARNING << "[EXCEPTION] Employee is already in the database.";
        }
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not save employee. Error message: " << e.what();
    }
}

void Database::saveProduct(const Product& product) {
    try {
        std::string name = product.name;
        double price = product.price;

        if (getProductByName(name).isEmpty()) {
            std::ostringstream oss;
            oss << name << "," << name << "," << price;
            std::string values = oss.str();

            pstmt = con->prepareStatement("INSERT INTO products(name, price) VALUES(?,?)");

            pstmt->setString(1, name);
            pstmt->setDouble(2, price);
            pstmt->execute();

            CROW_LOG_INFO << "[ADDED] Product " << name <<
                " with price " << price <<
                " inserted into products.";
        }
        else {
            CROW_LOG_WARNING << "[EXCEPTION] Product is already in the database.";
        }
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not save product. Error message: " << e.what();
    }
}

void Database::saveOrder(const Order& order) {
    try {
        // TODO: Add products of the order to Table class and Table table
        std::string time = order.time;
        std::string message = order.message;

        if (getOrderByTime(time).isEmpty()) {
            std::ostringstream oss;
            oss << time << "," << message;
            std::string values = oss.str();

            pstmt = con->prepareStatement("INSERT INTO orders(time, message) VALUES(?,?)");

            pstmt->setString(1, time);
            pstmt->setString(2, message);
            pstmt->execute();

            CROW_LOG_INFO << "[ADDED] Order with time " << time <<
                " and message " << message <<
                " inserted into orders.";
        }
        else {
            CROW_LOG_WARNING << "[EXCEPTION] Order is already in the database.";
        }
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not save order. Error message: " << e.what();
    }
}

void Database::saveIngredient(const Ingredient& ingredient) {
    try {
        std::string name = ingredient.name;

        if (getIngredientByName(name).isEmpty()) {
            std::ostringstream oss;
            oss << name;
            std::string values = oss.str();

            pstmt = con->prepareStatement("INSERT INTO ingredients(name) VALUES(?)");
            pstmt->setString(1, name);
            pstmt->execute();

            CROW_LOG_INFO << "[ADDED] Ingredient with name " << name <<
                " inserted into ingredients.";
        }
        else {
            CROW_LOG_WARNING << "[EXCEPTION] Ingredient is already in the database.";
        }
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not save ingredient. Error message: " << e.what();
    }
}

void Database::saveAllergen(const Allergen& allergen) {
    try {
        std::string name = allergen.name;

        if (getAllergenByName(name).isEmpty()) {
            std::ostringstream oss;
            oss << name;
            std::string values = oss.str();

            pstmt = con->prepareStatement("INSERT INTO allergens(name) VALUES(?)");
            pstmt->setString(1, name);
            pstmt->execute();

            CROW_LOG_INFO << "[ADDED] Allergen with name " << name <<
                " inserted into allergens.";
        }
        else {
            CROW_LOG_WARNING << "[EXCEPTION] Allergen is already in the database.";
        }
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not save allergen. Error message: " << e.what();
    }
}

void Database::saveTableProduct(const Table& table, const Product& product) {
    try {
        int n_table = table.n_table;
        double discount = table.discount;
        std::string name = product.name;
        int table_id = 0;
        int product_id = 0;

        std::stringstream query;

        query << "SELECT * FROM tables WHERE n_table = " << n_table;
        sql::ResultSet* res = stmt->executeQuery(query.str());
        query.str("");
    
        if (res->next()) {
            table_id = res->getInt("table_id");
            double new_bill = res->getDouble("bill") + product.price;

            pstmt = con->prepareStatement("UPDATE tables SET bill = ? WHERE table_id = ?");
            pstmt->setInt(1, new_bill);
            pstmt->setInt(2, table_id);
            pstmt->execute();
        }

        query << "SELECT product_id FROM products WHERE name = '" << name << "'";
        res = stmt->executeQuery(query.str());
        query.str("");

        if (res->next()) {
            product_id = res->getInt("product_id");
        }

        pstmt = con->prepareStatement("INSERT INTO tableproduct(table_id, product_id) VALUES(?,?)");
        pstmt->setInt(1, table_id);
        pstmt->setInt(2, product_id);
        pstmt->execute();
    }
    catch(sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not save table/product. Error message: " << e.what();
    }
}

void Database::saveProductIngredient(const Product& product, const Ingredient& ingredient) {
    try {

    }
    catch (sql::SQLException& e) {

    }
}

void Database::saveProductOrder(const Product& product, const Order& order) {
    try {

    }
    catch (sql::SQLException& e) {

    }
}


//Get
std::vector<Table> Database::getTables() const {
    std::vector<Table> tables;

    try {
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM tables"); // TODO: change tables for a varible that corresponds to the table name

        while (res->next()) {
            Table table;

            int id = res->getInt("table_id");
            table.n_table = res->getInt("n_table");
            // TODO: Add std::unordered_map<std::string, int> products
            table.bill = res->getDouble("bill");
            table.discount = res->getDouble("discount");

            //std::cout << "Id: " << id << ". Table Number: " << n_table << std::endl;
            tables.push_back(table);
        }

        std::sort(tables.begin(), tables.end(), [](Table a, Table b) { return a.n_table < b.n_table;  });

        return tables;
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get tables. Error message: " << e.what();
        return std::vector<Table>();
    }
}

Table Database::getTableByNumber(const int n_table) const {
    Table table;

    try {
        std::stringstream query;

        query << "SELECT * FROM tables WHERE n_table = " << n_table; // TODO: change tables for a varible that corresponds to the table name
        sql::ResultSet* res = stmt->executeQuery(query.str());
        query.str("");

        if (res->next()) {
            int table_id = res->getInt("table_id");
            int n = res->getInt("n_table");
            int n_clients = res->getInt("n_clients");
            double bill = res->getDouble("bill");
            double discount = res->getDouble("discount");

            query << "SELECT * FROM tableproduct WHERE table_id = " << table_id; // TODO: change tables for a varible that corresponds to the table name
            res = stmt->executeQuery(query.str());
            query.str("");

            std::unordered_map<std::string, int> products;
            while (res->next()) {
                int product_id = res->getInt("product_id");

                query << "SELECT * FROM products WHERE product_id = " << product_id; // TODO: change tables for a varible that corresponds to the table name
                sql::ResultSet* res2 = stmt->executeQuery(query.str());
                query.str("");

                if (res2->next()) {
                    std::string product_name = res2->getString("name");
                    double product_price = res2->getDouble("price");

                    products[product_name] = product_price;
                }
            }

            table.n_table = n;
            table.n_clients = n_clients;
            table.products = products;
            //for (auto i : products) {
                //std::cout << i.first << std::endl;
            //}
            table.bill = bill;
            table.discount = discount;
        }

        return table;
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get table by number. Error message: " << e.what();
        return table;
    }
}

std::vector<Employee> Database::getEmployees() const {
    std::vector<Employee> employees;

    try {
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM employees"); // TODO: change employees for a varible that corresponds to the table name

        while (res->next()) {
            int id = res->getInt("employee_id");
            std::string name = res->getString("name");
            int level = res->getInt("level");
            std::string start = res->getString("start");
            std::string finish = res->getString("finish");

            Employee employee = { name, level, start, finish };

            //std::cout << "Id: " << id << ". Employee name: " << name << " with level " << level << " and start time " << start << std::endl;
            employees.push_back(employee);
        }

        return employees;
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get employees. Error message: " << e.what();
        return employees;
    }
}

Employee Database::getEmployeeByName(const std::string name) const {
    Employee employee;

    try {
        std::stringstream query;
        query << "SELECT * FROM employees WHERE name = '" << name << "'"; // String needs to be inside '' // TODO: change employees for a varible that corresponds to the table name
        sql::ResultSet* res = stmt->executeQuery(query.str());

        if (res->next()) {
            std::string name = res->getString("name");
            int level = res->getInt("level");
            std::string start = res->getString("start");
            std::string finish = res->getString("finish");

            employee.name = name;
            employee.level = level;
            employee.start = start;
            employee.finish = finish;
        }

        return employee;
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get employee by name. Error message: " << e.what();
        return employee;
    }
}

std::vector<Product> Database::getProducts() const {
    std::vector<Product> products;

    try {
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM products"); // TODO: change employees for a varible that corresponds to the table name
        

        while (res->next()) {
            //int id = res->getInt("product_id");
            std::string name = res->getString("name");
            double price = res->getDouble("price");

            Product product = { name, price };

            //std::cout << "Id: " << id << ". Employee name: " << name << " with level " << level << " and start time " << start << std::endl;
            products.push_back(product);
        }

        return products;
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get products. Error message: " << e.what();
        return products;
    }
}

Product Database::getProductByName(const std::string name) const {
    Product product;
    try {
        
        std::stringstream query;
        query << "SELECT * FROM products WHERE name = '" << name << "'";
        sql::ResultSet* res = stmt->executeQuery(query.str());

        if (res->next()) {
            std::string name = res->getString("name");
            double price = res->getDouble("price");
            product.name = name;
            product.price = price;
        }

        return product;
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get product by name. Error message: " << e.what();
        return product; // Return an empty Product on error
    }
}

std::vector<Order> Database::getOrders() const {
    std::vector<Order> orders;

    try {
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM orders");
        

        while (res->next()) {
            std::string time = res->getString("time");
            std::string message = res->getString("message");

            Order order = { time, message };

            orders.push_back(order);
        }
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get orders. Error message: " << e.what();
    }

    return orders;
}

Order Database::getOrderByTime(const std::string time) const {
    Order order;

    try {
        std::stringstream query;
        query << "SELECT * FROM orders WHERE time = '" << time << "'";
        sql::ResultSet* res = stmt->executeQuery(query.str());

        if (res->next()) {
            std::string time = res->getString("time");
            std::string message = res->getString("message");

            order.time = time;
            order.message = message;
        }
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get order by time. Error message: " << e.what();
    }

    return order;
}

std::vector<Ingredient> Database::getIngredients() const {
    std::vector<Ingredient> ingredients;

    try {
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM ingredients");

        while (res->next()) {
            std::string name = res->getString("name");
            Ingredient ingredient = { name };
            ingredients.push_back(ingredient);
        }

        return ingredients;
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get ingredients. Error message: " << e.what();
        return ingredients;
    }
}

Ingredient Database::getIngredientByName(const std::string name) const {
    Ingredient ingredient;

    try {
        std::stringstream query;
        query << "SELECT * FROM ingredients WHERE name = '" << name << "'";
        sql::ResultSet* res = stmt->executeQuery(query.str());

        if (res->next()) {
            std::string name = res->getString("name");
            ingredient.name = name;
        }

        return ingredient;
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get ingredient by name. Error message: " << e.what();
        return ingredient; // Return an empty Ingredient on error
    }
}

std::vector<Allergen> Database::getAllergens() const {
    std::vector<Allergen> allergens;

    try {
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM allergens");

        while (res->next()) {
            std::string name = res->getString("name");
            Allergen allergen = { name };
            allergens.push_back(allergen);
        }

        return allergens;
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get allergens. Error message: " << e.what();
        return std::vector<Allergen>();
    }
}

Allergen Database::getAllergenByName(const std::string name) const {
    Allergen allergen;

    try {
        std::stringstream query;
        query << "SELECT * FROM allergens WHERE name = '" << name << "'";
        sql::ResultSet* res = stmt->executeQuery(query.str());

        if (res->next()) {
            std::string name = res->getString("name");
            allergen.name = name;
        }

        return allergen;
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get allergen by name. Error message: " << e.what();
        return allergen; // Return an empty Allergen on error
    }
}


// Print
void Database::printTables() {
    std::vector<Table> tables = getTables();

    CROW_LOG_INFO << "[LIST] Tables: ";
    for (const auto table : tables) {
        std::cout << "\t- Table Number: " << table.n_table << ", Bill: " << table.bill << ", Discount: " << table.discount << std::endl;
    }
}

void Database::printEmployees() {
    std::vector<Employee> employees = getEmployees();

    CROW_LOG_INFO << "[LIST] Employees: ";
    for (const auto employee : employees) {
        std::cout << "\t- Name: " << employee.name << ", Level: " << employee.level << ", Start: " << employee.start << ", Finish: " << employee.finish << std::endl;
    }
}

void Database::printProducts() {
    std::vector<Product> products = getProducts();

    CROW_LOG_INFO << "[LIST] Products: ";
    for (const auto product : products) {
        std::cout << "\t- Name: " << product.name << ", Price: " << product.price << std::endl;
    }
}

void Database::printOrders() {
    std::vector<Order> orders = getOrders();

    CROW_LOG_INFO << "[LIST] Orders: ";
    for (const auto order : orders) {
        std::cout << "\t- Time: " << order.time << ", Message: " << order.message << std::endl;
    }
}

void Database::printIngredients() {
    std::vector<Ingredient> ingredients = getIngredients();

    CROW_LOG_INFO << "[LIST] Ingredients: ";
    for (const auto ingredient : ingredients) {
        std::cout << "\t- Name: " << ingredient.name << std::endl;
    }
}

void Database::printAllergens() {
    std::vector<Allergen> allergens = getAllergens();

    CROW_LOG_INFO << "[LIST] Allergens: ";
    for (const auto allergen : allergens) {
        std::cout << "\t- Name: " << allergen.name << std::endl;
    }
}


// Set
void Database::setTable_NTable() {
}

void Database::setTable_NClients() {
}

void Database::setTable_Products(const Table table, const std::unordered_map<std::string, int> products) {
    int n_table = table.n_table;
    std::stringstream query;
    query << "SELECT * FROM tables WHERE n_table = '" << n_table << "'";
    sql::ResultSet* res = stmt->executeQuery(query.str());
}

void Database::setTable_Bill() {
}

void Database::setTable_Discount() {
}

void Database::setEmployee_Name() {
}

void Database::setEmployee_Level() {
}

void Database::setEmployee_Start() {
}

void Database::setEmployee_Finish() {
}



// Remove
void Database::removeTable(const Table& table) {
    try {
        int n_table = table.n_table;
        double bill = table.bill;
        double discount = table.discount;

        pstmt = con->prepareStatement("DELETE FROM tables WHERE n_table = ? AND bill = ? AND discount = ?");
        pstmt->setInt(1, n_table);
        pstmt->setDouble(2, bill);
        pstmt->setDouble(3, discount);
        pstmt->execute();

        CROW_LOG_INFO << "[REMOVED] Table with n_table " << n_table <<
            ", bill " << bill <<
            " and discount " << discount <<
            " has been deleted.";
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not remove table. Error message: " << e.what();
    }
}

void Database::removeEmployee(const Employee& employee) {
    try {
        std::string name = employee.name;
        int level = employee.level;
        std::string start = employee.start;
        std::string finish = employee.finish;

        pstmt = con->prepareStatement("DELETE FROM employees WHERE name = ? AND level = ? AND start = ? AND finish = ?");
        pstmt->setString(1, name);
        pstmt->setInt(2, level);
        pstmt->setString(3, start);
        pstmt->setString(4, finish);
        pstmt->execute();

        CROW_LOG_INFO << "[REMOVED] Employee with name " << name <<
            ", level " << level <<
            ", start " << start <<
            ", and finish " << finish <<
            " has been deleted.";
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not remove employee. Error message: " << e.what();
    }
}

void Database::removeProduct(const Product& product) {
    try {
        std::string name = product.name;
        double price = product.price;

        pstmt = con->prepareStatement("DELETE FROM products WHERE name = ? AND price = ?");
        pstmt->setString(1, name);
        pstmt->setDouble(2, price);
        pstmt->execute();

        CROW_LOG_INFO << "[REMOVED] Product with name " << name <<
            " and price " << price <<
            " has been deleted.";
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not remove product. Error message: " << e.what();
    }
}

void Database::removeOrder(const Order& order) {
    try {
        std::string time = order.time;
        std::string message = order.message;

        pstmt = con->prepareStatement("DELETE FROM orders WHERE time = ? AND message = ?");
        pstmt->setString(1, time);
        pstmt->setString(2, message);
        pstmt->execute();

        CROW_LOG_INFO << "[REMOVED] Order with time " << time <<
            " and message " << message <<
            " has been deleted.";
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not remove order. Error message: " << e.what();
    }
}

void Database::removeIngredient(const Ingredient& ingredient) {
    try {
        std::string name = ingredient.name;

        pstmt = con->prepareStatement("DELETE FROM ingredients WHERE name = ?");
        pstmt->setString(1, name);
        pstmt->execute();

        CROW_LOG_INFO << "[REMOVED] Ingredient with name " << name <<
            " has been deleted.";
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not remove ingredient. Error message: " << e.what();
    }
}

void Database::removeAllergen(const Allergen& allergen) {
    try {
        std::string name = allergen.name;

        pstmt = con->prepareStatement("DELETE FROM allergens WHERE name = ?");
        pstmt->setString(1, name);
        pstmt->execute();

        CROW_LOG_INFO << "[REMOVED] Allergen with name " << name <<
            " has been deleted.";
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not remove allergen. Error message: " << e.what();
    }
}