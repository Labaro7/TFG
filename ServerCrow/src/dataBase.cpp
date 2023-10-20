#include "../headers/dataBase.h"
#include "../headers/crow_all.h"

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

    Employee e;

    for (int i = 0; i < names.size(); i++) {
        e.set(names[i], levels[i], starts[0], finishes[0]);
        saveEmployee(&e);
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

    Product product;

    for (size_t i = 0; i < productNames.size(); i++) {
        product.set(productNames[i], prices[i]);
        saveProduct(&product);
    }
}

void Database::initializeOrdersTable() {
    std::vector<std::string> times = { "2023-10-04 15:45:00", "2023-10-05 12:30:00", "2023-10-06 18:15:00" };
    std::vector<std::string> messages = { "Order 1", "Order 2", "Order 3" };

    Order order;

    for (size_t i = 0; i < times.size(); i++) {
        order.setTime(times[i]);
        order.setMessage(messages[i]);
        saveOrder(&order);
    }
}

void Database::initializeIngredientsTable() {
    std::vector<std::string> ingredientNames = { "Salt", "Pepper", "Sugar" };

    Ingredient ingredient;

    for (const auto& name : ingredientNames) {
        ingredient.setName(name);
        saveIngredient(&ingredient);
    }
}

void Database::initializeAllergensTable() {
    std::vector<std::string> allergenNames = { "Gluten", "Peanuts", "Dairy" };

    Allergen allergen;

    for (const auto& name : allergenNames) {
        allergen.setName(name);
        saveAllergen(&allergen);
    }
}

void Database::initialize() {
    // Create the tables to define the domain
    MySqlCreateTable("tables", "table_id INT AUTO_INCREMENT PRIMARY KEY, n_table INT, n_clients INT, bill DOUBLE, discount DOUBLE");
    MySqlCreateTable("employees", "employee_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45), level INT, start VARCHAR(45), finish VARCHAR(45)");
    MySqlCreateTable("products", "product_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45), price DOUBLE");
    MySqlCreateTable("orders", "order_id INT AUTO_INCREMENT PRIMARY KEY, time VARCHAR(45), message VARCHAR(45)");
    MySqlCreateTable("ingredients", "ingredient_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45)");
    MySqlCreateTable("allergens", "allergen_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45)");
    MySqlCreateTable("restaurants", "restaurant_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45)");

    // TODO: Create Junction tables
    // Create the junction tables to the ManyToMany relationships
    MySqlCreateTable("tableProduct", "table_id INT, product_id INT, PRIMARY KEY(table_id, product_id), FOREIGN KEY(table_id) REFERENCES tables(table_id), FOREIGN KEY(product_id) REFERENCES products(product_id)");
    MySqlCreateTable("productOrder", "product_id INT, order_id INT, PRIMARY KEY(product_id, order_id), FOREIGN KEY(product_id) REFERENCES products(product_id), FOREIGN KEY(order_id) REFERENCES orders(order_id)");
    MySqlCreateTable("productIngredient", "product_id INT, ingredient_id INT, PRIMARY KEY(product_id, ingredient_id), FOREIGN KEY(product_id) REFERENCES products(product_id), FOREIGN KEY(ingredient_id) REFERENCES ingredients(ingredient_id)");
    MySqlCreateTable("productOrder", "product_id INT, order_id INT, PRIMARY KEY(product_id, order_id), FOREIGN KEY(product_id) REFERENCES products(product_id), FOREIGN KEY(order_id) REFERENCES orders(order_id)");
    MySqlCreateDatabase("ingredientAllergen, product_id INT, ingredient_id INT, PRIMARY KEY(product_id, ingredient_id), FOREIGN KEY(product_id) REFERENCES products(product_id), FOREIGN KEY(ingredient_id) REFERENCES ingredients(ingredient_id)");
    
    // Populate the tables
    initializeEmployeesTable();
    initializeProductsTable();
    initializeIngredientsTable();
    initializeAllergensTable();
    initializeOrdersTable();
}

void Database::dropAllTables() {
    try {
        stmt->execute("DROP TABLE IF EXISTS orders;");
        stmt->execute("DROP TABLE IF EXISTS ingredients;");
        stmt->execute("DROP TABLE IF EXISTS allergens;");
        stmt->execute("DROP TABLE IF EXISTS employees;");
        stmt->execute("DROP TABLE IF EXISTS products;");
        stmt->execute("DROP TABLE IF EXISTS tables;");
        stmt->execute("DROP TABLE IF EXISTS restaurants;");

        std::cout << "All tables dropped." << std::endl;
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not drop tables. Error message: " << e.what();
    }
}


// Save
void Database::saveTable(Table* table) {
    try {
        int n_table = table->getNTable();
        int n_clients = table->getNClients();
        double bill = table->getBill();
        double discount = table->getDiscount();

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
            std::cout << "[EXCEPTION] Table is already in the database." << std::endl;
        }
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not save table. Error message: " << e.what();
    }
}

void Database::saveEmployee(Employee* employee) {
    try {
        std::string name = employee->getName();
        int level = employee->getLevel();
        std::string start = employee->getStart();
        std::string finish = employee->getFinish();

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

void Database::saveProduct(Product* product) {
    try {
        std::string name = product->getName();
        double price = product->getPrice();

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

void Database::saveOrder(Order* order) {
    try {
        std::string time = order->getTime();
        std::string message = order->getMessage();

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

void Database::saveIngredient(Ingredient* ingredient) {
    try {
        std::string name = ingredient->getName();

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

void Database::saveAllergen(Allergen* allergen) {
    try {
        std::string name = allergen->getName();

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


//Get
std::vector<Table> Database::getTables() const {
    std::vector<Table> tables;

    try {
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM tables"); // TODO: change tables for a varible that corresponds to the table name

        while (res->next()) {
            Table table;

            int id = res->getInt("table_id");
            int n_table = res->getInt("n_table");

            table.setNTable(n_table);

            //std::cout << "Id: " << id << ". Table Number: " << n_table << std::endl;
            tables.push_back(table);
        }

        return tables;
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get tables. Error message: " << e.what();
        return std::vector<Table>();
    }
}

Table Database::getTableByNumber(int n_table) const {
    Table table;

    try {
        std::stringstream query;

        query << "SELECT * FROM tables WHERE n_table = " << n_table; // TODO: change tables for a varible that corresponds to the table name
        sql::ResultSet* res = stmt->executeQuery(query.str());

        if (res->next()) {
            int n = res->getInt("n_table");
            int n_clients = res->getInt("n_clients");
            double bill = res->getDouble("bill");
            double discount = res->getDouble("discount");

            table.setNTable(n);
            table.setNClients(n_clients);
            table.setBill(bill);
            table.setDiscount(discount);
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
        Employee employee;

        while (res->next()) {
            int id = res->getInt("employee_id");
            std::string name = res->getString("name");
            int level = res->getInt("level");
            std::string start = res->getString("start");
            std::string finish = res->getString("finish");

            employee.set(name, level, start, finish);

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

Employee Database::getEmployeeByName(std::string name) const {
    Employee employee;

    try {
        std::stringstream query;
        query << "SELECT * FROM employees WHERE name = '" << name << "'"; // String needs to be inside '' // TODO: change employees for a varible that corresponds to the table name
        sql::ResultSet* res = stmt->executeQuery(query.str());

        if (res->next()) {
            std::string name_ = res->getString("name");
            int level = res->getInt("level");
            std::string start = res->getString("start");
            std::string finish = res->getString("finish");

            employee.set(name_, level, start, finish);
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
        Product product;

        while (res->next()) {
            //int id = res->getInt("product_id");
            std::string name = res->getString("name");
            double price = res->getDouble("price");

            product.set(name, price);

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

Product Database::getProductByName(std::string name) const {
    try {
        Product product;
        std::stringstream query;
        query << "SELECT * FROM products WHERE name = '" << name << "'";
        sql::ResultSet* res = stmt->executeQuery(query.str());

        if (res->next()) {
            std::string product_name = res->getString("name");
            double price = res->getDouble("price");
            product.set(product_name, price);
        }

        return product;
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get product by name. Error message: " << e.what();
        return Product(); // Return an empty Product on error
    }
}

std::vector<Order> Database::getOrders() const {
    std::vector<Order> orders;

    try {
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM orders");
        Order order;

        while (res->next()) {
            std::string time = res->getString("time");
            std::string message = res->getString("message");

            order.setTime(time);
            order.setMessage(message);

            orders.push_back(order);
        }
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get orders. Error message: " << e.what();
    }

    return orders;
}

Order Database::getOrderByTime(std::string time) const {
    Order order;

    try {
        std::stringstream query;
        query << "SELECT * FROM orders WHERE time = '" << time << "'";
        sql::ResultSet* res = stmt->executeQuery(query.str());

        if (res->next()) {
            std::string time = res->getString("time");
            std::string message = res->getString("message");

            order.setTime(time);
            order.setMessage(message);
        }
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get order by time. Error message: " << e.what();
    }

    return order;
}

std::vector<Ingredient> Database::getIngredients() const {
    try {
        std::vector<Ingredient> ingredients;
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM ingredients");
        Ingredient ingredient;

        while (res->next()) {
            std::string name = res->getString("name");
            ingredient.setName(name);
            ingredients.push_back(ingredient);
        }

        return ingredients;
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get ingredients. Error message: " << e.what();
        return std::vector<Ingredient>();
    }
}

Ingredient Database::getIngredientByName(std::string name) const {
    try {
        Ingredient ingredient;
        std::stringstream query;
        query << "SELECT * FROM ingredients WHERE name = '" << name << "'";
        sql::ResultSet* res = stmt->executeQuery(query.str());

        if (res->next()) {
            std::string ingredient_name = res->getString("name");
            ingredient.setName(ingredient_name);
        }

        return ingredient;
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get ingredient by name. Error message: " << e.what();
        return Ingredient(); // Return an empty Ingredient on error
    }
}

std::vector<Allergen> Database::getAllergens() const {
    try {
        std::vector<Allergen> allergens;
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM allergens");
        Allergen allergen;

        while (res->next()) {
            std::string name = res->getString("name");
            allergen.setName(name);
            allergens.push_back(allergen);
        }

        return allergens;
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get allergens. Error message: " << e.what();
        return std::vector<Allergen>();
    }
}

Allergen Database::getAllergenByName(std::string name) const {
    try {
        Allergen allergen;
        std::stringstream query;
        query << "SELECT * FROM allergens WHERE name = '" << name << "'";
        sql::ResultSet* res = stmt->executeQuery(query.str());

        if (res->next()) {
            std::string allergen_name = res->getString("name");
            allergen.setName(allergen_name);
        }

        return allergen;
    }
    catch (sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get allergen by name. Error message: " << e.what();
        return Allergen(); // Return an empty Allergen on error
    }
}


// Print
void Database::printTables() {
    std::vector<Table> tables = getTables();

    CROW_LOG_INFO << "[LIST] Tables: ";
    for (const auto table : tables) {
        std::cout << "\t- Table Number: " << table.getNTable() << ", Bill: " << table.getBill() << ", Discount: " << table.getDiscount() << std::endl;
    }
}

void Database::printEmployees() {
    std::vector<Employee> employees = getEmployees();

    CROW_LOG_INFO << "[LIST] Employees: ";
    for (const auto employee : employees) {
        std::cout << "\t- Name: " << employee.getName() << ", Level: " << employee.getLevel() << ", Start: " << employee.getStart() << ", Finish: " << employee.getFinish() << std::endl;
    }
}

void Database::printProducts() {
    std::vector<Product> products = getProducts();

    CROW_LOG_INFO << "[LIST] Products: ";
    for (const auto product : products) {
        std::cout << "\t- Name: " << product.getName() << ", Price: " << product.getPrice() << std::endl;
    }
}

void Database::printOrders() {
    std::vector<Order> orders = getOrders();

    CROW_LOG_INFO << "[LIST] Orders: ";
    for (const auto order : orders) {
        std::cout << "\t- Time: " << order.getTime() << ", Message: " << order.getMessage() << std::endl;
    }
}

void Database::printIngredients() {
    std::vector<Ingredient> ingredients = getIngredients();

    CROW_LOG_INFO << "[LIST] Ingredients: ";
    for (const auto ingredient : ingredients) {
        std::cout << "\t- Name: " << ingredient.getName() << std::endl;
    }
}

void Database::printAllergens() {
    std::vector<Allergen> allergens = getAllergens();

    CROW_LOG_INFO << "[LIST] Allergens: ";
    for (const auto allergen : allergens) {
        std::cout << "\t- Name: " << allergen.getName() << std::endl;
    }
}


// Set
void Database::setTable_NTable() {
}

void Database::setTable_NClients() {
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
void Database::removeTable(Table* table) {
    try {
        int n_table = table->getNTable();
        double bill = table->getBill();
        double discount = table->getDiscount();

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

void Database::removeEmployee(Employee* employee){
    try {
        std::string name = employee->getName();
        int level = employee->getLevel();
        std::string start = employee->getStart();
        std::string finish = employee->getFinish();

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

void Database::removeProduct(Product* product) {
    try {
        std::string name = product->getName();
        double price = product->getPrice();

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

void Database::removeOrder(Order* order) {
    try {
        std::string time = order->getTime();
        std::string message = order->getMessage();

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

void Database::removeIngredient(Ingredient* ingredient) {
    try {
        std::string name = ingredient->getName();

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

void Database::removeAllergen(Allergen* allergen) {
    try {
        std::string name = allergen->getName();

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