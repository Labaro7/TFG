#include "..\headers\dataBase.h"

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
        std::cerr << "Could not create database. Error message: " << e.what() << std::endl;
    }
}

void Database::MySqlDropDatabase(std::string name) {
    try {
        sql::Statement* stmt = con->createStatement();
        stmt->execute("DROP DATABASE " + name);
    }
    catch (sql::SQLException& e) {
        std::cerr << "Could not drop database. Error message: " << e.what() << std::endl;
    }
}

void Database::MySqlUseDatabase(std::string name) {
    try {
        sql::Statement* stmt = con->createStatement();
        stmt->execute("USE " + name);
    }
    catch (sql::SQLException& e) {
        std::cerr << "Could not use database. Error message: " << e.what() << std::endl;
    }
}

void Database::MySqlSaveChangesToDataBase() {
    try {
        sql::Statement* stmt = con->createStatement();
        stmt->execute("COMMIT");
    }
    catch (sql::SQLException& e) {
        std::cerr << "Could not save changes to the database. Error message: " << e.what() << std::endl;
    }
}

// Table
void Database::MySqlCreateTable(std::string name, std::string definition) {
    try {
        sql::Statement* stmt = con->createStatement();
        stmt->execute("CREATE TABLE " + name + " (" + definition + ")");
        std::cout << "Table created: " << name << std::endl;
    }
    catch (sql::SQLException& e) {
        std::cerr << "Could not create table. Error message: " << e.what() << std::endl;
    }
}

void Database::MySqlDropTable(std::string name) {
    try {
        sql::Statement* stmt = con->createStatement();
        stmt->execute("DROP TABLE IF EXISTS " + name);
    }
    catch (sql::SQLException& e) {
        std::cerr << "Could not drop table. Error message: " << e.what() << std::endl;
    }
}

void Database::MySqlModifyTable(std::string name, std::string modification) {
    try {
        sql::Statement* stmt = con->createStatement();
        stmt->execute("ALTER TABLE " + name + " " + modification);
    }
    catch (sql::SQLException& e) {
        std::cerr << "Could not modify table. Error message: " << e.what() << std::endl;
    }
}

void Database::MySqlEmptyTable(std::string name) {
    try {
        sql::Statement* stmt = con->createStatement();
        stmt->execute("DELETE FROM " + name);
    }
    catch (sql::SQLException& e) {
        std::cerr << "Could not empty table. Error message: " << e.what() << std::endl;
    }
}


// ------------------------------- /MySQL queries ------------------------------- //


void Database::initialize() {
    // TODO: Get tables names and columns from a file that specifies the format wanted
    MySqlCreateTable("tables", "table_id INT AUTO_INCREMENT PRIMARY KEY, n_table INT, n_clients INT, bill DOUBLE, discount DOUBLE");
    MySqlCreateTable("employees", "employee_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45), level INT, start VARCHAR(45), finish VARCHAR(45)");
    MySqlCreateTable("products", "product_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45), price DOUBLE");
    MySqlCreateTable("orders", "order_id INT AUTO_INCREMENT PRIMARY KEY, time VARCHAR(45), message VARCHAR(45)");
    MySqlCreateTable("ingredients", "ingredient_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45)");
    MySqlCreateTable("allergens", "allergen_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45)");
    MySqlCreateTable("restaurants", "restaurant_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45)");

    // TODO: Create Junction tables

}

// Save
void Database::saveTable(Table* table) {
    try {
        int n_table = table->getNTable();
        int n_clients = table->getNClients();
        double bill = table->getBill();
        double discount = table->getDiscount();

        std::ostringstream oss;
        oss << n_table << "," << n_clients << "," << bill << "," << discount;
        std::string values = oss.str();

        pstmt = con->prepareStatement("INSERT INTO tables(n_table, n_clients, bill, discount) VALUES(?,?,?,?)");
        pstmt->setInt(1, n_table);
        pstmt->setInt(2, n_clients);
        pstmt->setDouble(3, bill);
        pstmt->setDouble(4, discount);
        pstmt->execute();

        std::cout << "Table " << n_table <<
            " with " << n_clients <<
            " clients inserted into tables." << std::endl;
    }
    catch (sql::SQLException& e) {
        std::cerr << "Could not save table. Error message: " << e.what() << std::endl;
    }
}

void Database::saveEmployee(Employee* employee) {
    try {
        std::string name = employee->getName();
        int level = employee->getLevel();
        std::string start = employee->getStart();
        std::string finish = employee->getFinish();

        std::ostringstream oss;
        oss << name << "," << level << "," << start << "," << finish;
        std::string values = oss.str();

        pstmt = con->prepareStatement("INSERT INTO employees(name, level, start, finish) VALUES(?,?,?,?)"); // Error corrected: The word "rank" is reserved in MySQL, it has to be between ``.

        pstmt->setString(1, name);
        pstmt->setInt(2, level);
        pstmt->setString(3, start);
        pstmt->setString(4, finish);
        pstmt->execute();

        std::cout << "Employee " << name <<
            " with level " << level <<
            " starting at " << start <<
            " inserted into employees." << std::endl;
    }
    catch (sql::SQLException& e) {
        std::cerr << "Could not save employee. Error message: " << e.what() << std::endl;
    }
}

void Database::saveProduct(Product* product) {
    try {
        std::string name = product->getName();
        double price = product->getPrice();

        std::ostringstream oss;
        oss << name << "," << name << "," << price;
        std::string values = oss.str();

        pstmt = con->prepareStatement("INSERT INTO products(name, price) VALUES(?,?)");

        pstmt->setString(1, name);
        pstmt->setDouble(2, price);
        pstmt->execute();

        std::cout << "Product " << name <<
            " with price " << price <<
            " inserted into products." << std::endl;
    }
    catch (sql::SQLException& e) {
        std::cerr << "Could not save product. Error message: " << e.what() << std::endl;
    }
}

void Database::saveOrder(Order* order) {
    try {
        std::string time = order->getTime();
        std::string message = order->getMessage();

        std::ostringstream oss;
        oss << time << "," << message;
        std::string values = oss.str();

        pstmt = con->prepareStatement("INSERT INTO orders(time, message) VALUES(?,?)");

        pstmt->setString(1, time);
        pstmt->setString(2, message);
        pstmt->execute();

        std::cout << "Order with time " << time <<
            " and message " << message <<
            " inserted into orders." << std::endl;
    }
    catch (sql::SQLException& e) {
        std::cerr << "Could not save order. Error message: " << e.what() << std::endl;
    }
}

void Database::saveIngredient(Ingredient* ingredient) {
    try {
        std::string name = ingredient->getName();

        std::ostringstream oss;
        oss << name;
        std::string values = oss.str();

        pstmt = con->prepareStatement("INSERT INTO ingredients(name) VALUES(?)");
        pstmt->setString(1, name);
        pstmt->execute();

        std::cout << "Ingredient with name " << name <<
            " inserted into ingredients." << std::endl;
    }
    catch (sql::SQLException& e) {
        std::cerr << "Could not save ingredient. Error message: " << e.what() << std::endl;
    }
}

void Database::saveAllergen(Allergen* allergen) {
    try {
        std::string name = allergen->getName();

        std::ostringstream oss;
        oss << name;
        std::string values = oss.str();

        pstmt = con->prepareStatement("INSERT INTO allergens(name) VALUES(?)");
        pstmt->setString(1, name);
        pstmt->execute();

        std::cout << "Allergen with name " << name <<
            " inserted into allergens." << std::endl;
    }
    catch (sql::SQLException& e) {
        std::cerr << "Could not save allergen. Error message: " << e.what() << std::endl;
    }
}


//Get
std::vector<Table> Database::getTables() const {
    try {
        std::vector<Table> tables;

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
        std::cerr << "Could not get tables. Error message: " << e.what() << std::endl;
        return std::vector<Table>();
    }
}

Table Database::getTableByNumber(int n_table) const {
    try {
        Table table;
        std::stringstream query;

        query << "SELECT * FROM tables WHERE n_table = " << n_table; // TODO: change tables for a varible that corresponds to the table name

        sql::ResultSet* res = stmt->executeQuery(query.str());

        if (res->next()) {
            int n_table = res->getInt("n_table");
            int n_clients = res->getInt("n_clients");
            double bill = res->getDouble("bill");
            double discount = res->getDouble("discount");

            table.setNTable(n_table);
            table.setNClients(n_clients);
            table.setBill(bill);
            table.setDiscount(discount);
        }

        return table;
    }
    catch (sql::SQLException& e) {
        std::cerr << "Could not get table by number. Error message: " << e.what() << std::endl;
        return Table();
    }
}

std::vector<Employee> Database::getEmployees() const {
    try {
        std::vector<Employee> employees;
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
        std::cerr << "Could not get employees. Error message: " << e.what() << std::endl;
        return std::vector<Employee>();
    }
}

Employee Database::getEmployeeByName(std::string name) const {
    try {
        Employee employee;
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
        std::cerr << "Could not get employee by name. Error message: " << e.what() << std::endl;
        return Employee();
    }
}

std::vector<Product> Database::getProducts() const {
    try {
        std::vector<Product> products;
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
        std::cerr << "Could not get products. Error message: " << e.what() << std::endl;
        return std::vector<Product>();
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
        std::cerr << "Could not get orders. Error message: " << e.what() << std::endl;
    }

    return orders;
}

Product Database::getOrderByTime(std::string time) const {
    Product product; // Adjust this to return the correct type

    try {
        std::stringstream query;
        query << "SELECT * FROM products WHERE time = '" << time << "'";
        sql::ResultSet* res = stmt->executeQuery(query.str());

        if (res->next()) {
            // Populate the product based on the result
            // You'll need to adjust this part based on your database schema
            std::string name = res->getString("name");
            double price = res->getDouble("price");

            product.setName(name);
            product.setPrice(price);
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "Could not get product by time. Error message: " << e.what() << std::endl;
    }

    return product;
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
        std::cerr << "Could not get product by name. Error message: " << e.what() << std::endl;
        return Product(); // Return an empty Product on error
    }
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
        std::cerr << "Could not get ingredients. Error message: " << e.what() << std::endl;
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
        std::cerr << "Could not get ingredient by name. Error message: " << e.what() << std::endl;
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
        std::cerr << "Could not get allergens. Error message: " << e.what() << std::endl;
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
        std::cerr << "Could not get allergen by name. Error message: " << e.what() << std::endl;
        return Allergen(); // Return an empty Allergen on error
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

        std::cout << "Table with n_table " << n_table <<
            ", bill " << bill <<
            " and discount " << discount << 
            " has been deleted." << std::endl;
    }
    catch (sql::SQLException& e) {
        std::cerr << "Could not remove table. Error message: " << e.what() << std::endl;
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

        std::cout << "Employee with name " << name <<
            ", level " << level <<
            ", start " << start <<
            ", and finish " << finish <<
            " has been deleted." << std::endl;
    }
    catch (sql::SQLException& e) {
        std::cerr << "Could not remove employee. Error message: " << e.what() << std::endl;
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

        std::cout << "Product with name " << name <<
            " and price " << price <<
            " has been deleted." << std::endl;
    }
    catch (sql::SQLException& e) {
        std::cerr << "Could not remove product. Error message: " << e.what() << std::endl;
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

        std::cout << "Order with time " << time <<
            " and message " << message <<
            " has been deleted." << std::endl;
    }
    catch (sql::SQLException& e) {
        std::cerr << "Could not remove order. Error message: " << e.what() << std::endl;
    }
}

void Database::removeIngredient(Ingredient* ingredient) {
    try {
        std::string name = ingredient->getName();

        pstmt = con->prepareStatement("DELETE FROM ingredients WHERE name = ?");
        pstmt->setString(1, name);
        pstmt->execute();

        std::cout << "Ingredient with name " << name <<
            " has been deleted." << std::endl;
    }
    catch (sql::SQLException& e) {
        std::cerr << "Could not remove ingredient. Error message: " << e.what() << std::endl;
    }
}

void Database::removeAllergen(Allergen* allergen) {
    try {
        std::string name = allergen->getName();

        pstmt = con->prepareStatement("DELETE FROM allergens WHERE name = ?");
        pstmt->setString(1, name);
        pstmt->execute();

        std::cout << "Allergen with name " << name <<
            " has been deleted." << std::endl;
    }
    catch (sql::SQLException& e) {
        std::cerr << "Could not remove allergen. Error message: " << e.what() << std::endl;
    }
}