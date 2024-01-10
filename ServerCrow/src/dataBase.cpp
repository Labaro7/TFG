#include "../headers/dataBase.h"
#include "../headers/crow_all.h"
#include "../headers/domain.h"

Database::Database() : pstmt() {
    try{
        connection_properties = {
            {"hostName", HOST},
            {"port", PORT},
            {"userName", USERNAME},
            {"password", PASSWORD},
            {"schema", DATABASE_NAME},
            {"OPT_RECONNECT", true}
        };

        driver = get_driver_instance();
        con = static_cast<std::shared_ptr<sql::Connection>>(driver->connect(connection_properties));
        stmt = con->createStatement();
    }
    catch (const sql::SQLException e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not construct database." << e.what();
        exit(1);
    }
}

Database::~Database() {
    con->close();

    // These have to be explicitly deleted. con is shared_ptr so it doesnt need to
    delete stmt;
    delete pstmt;
}



// ------------------------------- MySQL queries ------------------------------- //

// Database
void Database::MySqlCreateDatabase(std::string name) {
    try {
        sql::Statement* stmt = con->createStatement();
        stmt->execute("CREATE DATABASE " + name);
    }
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not create database. Error message: " << e.what();
    }
}

void Database::MySqlDropDatabase(std::string name) {
    try {
        sql::Statement* stmt = con->createStatement();
        stmt->execute("DROP DATABASE " + name);
    }
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not drop database. Error message: " << e.what();
    }
}

void Database::MySqlUseDatabase(std::string name) {
    try {
        sql::Statement* stmt = con->createStatement();
        stmt->execute("USE " + name);
    }
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not use database. Error message: " << e.what();
    }
}

void Database::MySqlSaveChangesToDataBase() {
    try {
        sql::Statement* stmt = con->createStatement();
        stmt->execute("COMMIT");
    }
    catch (const sql::SQLException& e) {
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
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not create table. Error message: " << e.what();
    }
}

void Database::MySqlDropTable(std::string name) {
    try {
        sql::Statement* stmt = con->createStatement();
        stmt->execute("DROP TABLE IF EXISTS " + name);
    }
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not drop table. Error message: " << e.what();
    }
}

void Database::MySqlModifyTable(std::string name, std::string modification) {
    try {
        sql::Statement* stmt = con->createStatement();
        stmt->execute("ALTER TABLE " + name + " " + modification);
    }
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not modify table. Error message: " << e.what();
    }
}

void Database::MySqlEmptyTable(std::string name) {
    try {
        sql::Statement* stmt = con->createStatement();
        stmt->execute("DELETE FROM " + name);
    }
    catch (const sql::SQLException& e) {
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
    /*std::vector<std::string> productNames = {"DRINKS", "BEERS", "CAFETERIA",
    "WHITE WINES", "RED WINES", "ROSSE WINES",
    "SPARKLING WINES", "COCKTAILS", "RUM",
    "GIN", "WHISKY", "VODKA" };

    std::vector<double> prices = { 10.99, 15.99, 8.50,
    10.99, 15.99, 8.50,
    10.99, 15.99, 8.50,
    10.99, 15.99, 8.50 };

    for (size_t i = 0; i < productNames.size(); i++) {
        Product product = { productNames[i], prices[i], "#FFFFFF", 1, false};
        saveProduct(product);
    }*/
}

void Database::initializeOrdersTable() {
    /*std::vector<std::string> times = { "2023-10-04 15:45:00", "2023-10-05 12:30:00", "2023-10-06 18:15:00" };
    std::vector<std::string> messages = { "Order 1", "Order 2", "Order 3" };

    for (size_t i = 0; i < times.size(); i++) {
        Order order = { times[i], messages[0] };

        saveOrder(order);
    }*/
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
    MySqlCreateTable("products", "product_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45), price DOUBLE, color VARCHAR(45), page INT, deployable BOOLEAN");
    MySqlCreateTable("orders", "order_id INT AUTO_INCREMENT PRIMARY KEY, n_table INT, n_clients INT, bill DOUBLE, discount DOUBLE, employee VARCHAR(45), date VARCHAR(45)");
    MySqlCreateTable("ingredients", "ingredient_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45)");
    MySqlCreateTable("allergens", "allergen_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45)");
    MySqlCreateTable("restaurants", "restaurant_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45)");

    // TODO: Create Junction tables
    // Create the junction tables to the ManyToMany relationships
    MySqlCreateTable("tableproduct", "table_id INT, product_id INT, amount INT, PRIMARY KEY(table_id, product_id), FOREIGN KEY(table_id) REFERENCES tables(table_id), FOREIGN KEY(product_id) REFERENCES products(product_id)");
    MySqlCreateTable("orderproduct", "order_id INT, product_id INT, amount INT, PRIMARY KEY(order_id, product_id)");
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
        std::unique_lock<std::mutex> lock(mutex);

        // We must drop first the tables that have a reference to another
        stmt->execute("DROP TABLE IF EXISTS tableproduct;");
        stmt->execute("DROP TABLE IF EXISTS orderproduct;");
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
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not drop tables. Error message: " << e.what();
    }
}


// Save
void Database::saveTable(const Table& table) {
    try {
        //std::unique_lock<std::mutex> lock(mutex);

        pstmt = con->prepareStatement("INSERT INTO tables(n_table, n_clients, bill, discount) VALUES(?,?,?,?)");
        pstmt->setInt(1, table.n_table);
        pstmt->setInt(2, table.n_clients);
        pstmt->setDouble(3, table.bill);
        pstmt->setDouble(4, table.discount);
        pstmt->execute();

        CROW_LOG_INFO << "[ADDED] Table " << table.n_table <<
            " with " << table.n_clients <<
            " clients inserted into tables.";
    }
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not save table. Error message: " << e.what();
    }
}

void Database::saveEmployee(const Employee& employee) {
    try {
        //std::unique_lock<std::mutex> lock(mutex);

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
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not save employee. Error message: " << e.what();
    }
}

void Database::saveProduct(const Product& product) {
    try {
        //std::unique_lock<std::mutex> lock(mutex);

        std::string name = product.name;
        double price = product.price;
        std::string color = product.color;
        int page = product.page;
        int deployable = product.deployable;

        if (getProductByName(name).isEmpty()) {
            pstmt = con->prepareStatement("INSERT INTO products(name, price, color, page, deployable) VALUES(?,?,?,?,?)");
            pstmt->setString(1, name);
            pstmt->setDouble(2, price);
            pstmt->setString(3, color);
            pstmt->setInt(4, page);
            pstmt->setInt(5, deployable);
            pstmt->execute();

            CROW_LOG_INFO << "[ADDED] Product " << name <<
                " with price " << price <<
                " inserted into products.";
        }
        else {
            CROW_LOG_WARNING << "[EXCEPTION] Product is already in the database.";
        }
    }
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not save product. Error message: " << e.what();
    }
}

void Database::saveOrder(const Order& order) {
    try {
        //std::unique_lock<std::mutex> lock(mutex);

        pstmt = con->prepareStatement("INSERT INTO orders(n_table, n_clients, bill, discount, employee, date) VALUES(?,?,?,?,?,?)");
        pstmt->setInt(1, order.n_table);
        pstmt->setInt(2, order.n_clients);
        pstmt->setDouble(3, order.bill);
        pstmt->setDouble(4, order.discount);
        pstmt->setString(5, order.employee);
        pstmt->setString(6, order.date);
        pstmt->execute();

        std::stringstream query;
        query << "SELECT table_id FROM tables WHERE n_table = '" << order.n_table << "'";
        sql::ResultSet* res = stmt->executeQuery(query.str());
        query.str("");

        if (res->next()) {
            int table_id = res->getInt("table_id");

            query << "SELECT * FROM tableproduct WHERE table_id = '" << table_id << "'";
            res = stmt->executeQuery(query.str());
            query.str("");

            while(res->next()) {
                int product_id = res->getInt("product_id");
                int amount = res->getInt("amount");

                saveOrderProduct(order, product_id, amount);
            }

            CROW_LOG_INFO << "[ADDED] Order with n_table " << order.n_table <<
                " with bill " << order.bill <<
                " with discount " << order.discount <<
                " by employee " << order.employee <<
                " at date " << order.date;
        }
    }
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not save order. Error message: " << e.what();
    }
}

void Database::saveOrderProduct(const Order& order, const int& product_id, const int& amount) {
    try {
        std::unique_lock<std::mutex> lock(mutex);

        std::stringstream query;
        query << "SELECT order_id FROM orders WHERE n_table = '" << order.n_table << "' AND n_clients = '" << order.n_clients << "' AND bill = '" << order.bill << "' AND discount = '" << order.discount << "' AND employee = '" << order.employee << "' AND date = '" << order.date << "'";
        sql::ResultSet* res = stmt->executeQuery(query.str());
        query.str("");

        if (res->next()) {
            int order_id = res->getInt("order_id");

            pstmt = con->prepareStatement("INSERT INTO orderproduct(order_id, product_id, amount) VALUES(?,?,?)");
            pstmt->setInt(1, order_id);
            pstmt->setInt(2, product_id);
            pstmt->setInt(3, amount);
            pstmt->execute();
        }
    }
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not save order/product. Error message: " << e.what();
    }
}

void Database::saveIngredient(const Ingredient& ingredient) {
    try {
        //std::unique_lock<std::mutex> lock(mutex);

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
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not save ingredient. Error message: " << e.what();
    }
}

void Database::saveAllergen(const Allergen& allergen) {
    try {
        //std::unique_lock<std::mutex> lock(mutex);

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
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not save allergen. Error message: " << e.what();
    }
}

void Database::saveTableProduct(Table& table, const Product& product, const int& times) {
    try {
        std::unique_lock<std::mutex> lock(mutex);

        int table_id = 0;
        int n_table = table.n_table;
        //double discount = table.discount;
        
        int product_id = 0;
        std::string name = product.name;

        std::stringstream query;

        query << "SELECT * FROM tables WHERE n_table = " << n_table;
        sql::ResultSet* res = stmt->executeQuery(query.str());
        query.str("");
    
        if (res->next()) {
            table_id = res->getInt("table_id");
            double new_bill = res->getDouble("bill") + (product.price * times);

            pstmt = con->prepareStatement("UPDATE tables SET bill = ? WHERE table_id = ?");
            pstmt->setDouble(1, new_bill);
            pstmt->setInt(2, table_id);
            pstmt->execute();

            CROW_LOG_INFO << "[UPDATED] Table " << table.n_table <<
                " updated bill is " << new_bill;

            query << "SELECT product_id FROM products WHERE name = '" << name << "'";
            res = stmt->executeQuery(query.str());
            query.str("");

            if (res->next()) {
                product_id = res->getInt("product_id");

                query << "SELECT amount FROM tableproduct WHERE table_id = " << table_id << " AND product_id = " << product_id;
                res = stmt->executeQuery(query.str());
                query.str("");

                if (res->next()) {
                    int new_amount = res->getInt("amount") + times;

                    pstmt = con->prepareStatement("UPDATE tableproduct SET amount = ? WHERE table_id = ? AND product_id = ?");
                    pstmt->setInt(1, new_amount);
                    pstmt->setInt(2, table_id);
                    pstmt->setInt(3, product_id);
                    pstmt->execute();

                    CROW_LOG_INFO << "[UPDATED] Tableproduct with table_id " << table_id <<
                        " and product_id " << product_id << " to amount " << new_amount;
                }
                else {
                    pstmt = con->prepareStatement("INSERT INTO tableproduct(table_id, product_id, amount) VALUES(?,?, ?)");
                    pstmt->setInt(1, table_id);
                    pstmt->setInt(2, product_id);
                    pstmt->setInt(3, times);
                    pstmt->execute();

                    CROW_LOG_INFO << "[ADDED] Tableproduct with table_id " << table_id <<
                        " and product_id " << product_id << " and amount " << times << " inserted into tableproduct.";
                }
            }
        }
    }
    catch(const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not save table/product. Error message: " << e.what();
    }
}

void Database::saveProductIngredient(const Product& product, const Ingredient& ingredient) {
    try {
        std::unique_lock<std::mutex> lock(mutex);
    }
    catch (const sql::SQLException& e) {

    }
}

void Database::saveProductOrder(const Product& product, const Order& order) {
    try {
        std::unique_lock<std::mutex> lock(mutex);
    }
    catch (const sql::SQLException& e) {

    }
}


//Get
std::vector<Table> Database::getTables() {
    std::unique_lock<std::mutex> lock(mutex);
    std::vector<Table> tables;

    try {
        //std::unique_lock<std::mutex> lock(mutex);

        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM tables"); // TODO: change tables for a varible that corresponds to the table name

        while (res->next()) {
            Table table;

            int id = res->getInt("table_id");
            table.n_table = res->getInt("n_table");
            // TODO: Add std::unordered_map<std::string, int> products
            table.bill = res->getDouble("bill");
            table.discount = res->getDouble("discount");

            tables.push_back(table);
        }

        // Sort the tables in ascending order
        std::sort(tables.begin(), tables.end(), [](Table a, Table b) { return a.n_table < b.n_table;  });

        return tables;
    }
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get tables. Error message: " << e.what();
        return tables;
    }
}

Table Database::getTableByNumber(const int n_table) {
    Table table;

    try {
        //std::unique_lock<std::mutex> lock(mutex);

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

            product_unordered_map products;
            while (res->next()) {
                int product_id = res->getInt("product_id");
                int amount = res->getInt("amount");

                query << "SELECT * FROM products WHERE product_id = " << product_id; // TODO: change tables for a varible that corresponds to the table name
                sql::ResultSet* res2 = stmt->executeQuery(query.str());
                query.str("");

                if (res2->next()) {
                    Product p(res2->getString("name"), res2->getDouble("price"), res2->getString("color"), res2->getInt("page"), res2->getBoolean("deployable"));

                    products[p] = amount;
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
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get table by number. Error message: " << e.what();
        return table;
    }
}

std::vector<Employee> Database::getEmployees() {
    std::vector<Employee> employees;

    try {
        //std::unique_lock<std::mutex> lock(mutex);

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
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get employees. Error message: " << e.what();
        return employees;
    }
}

Employee Database::getEmployeeByName(const std::string name) {
    Employee employee;

    try {
        //std::unique_lock<std::mutex> lock(mutex);

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
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get employee by name. Error message: " << e.what();
        return employee;
    }
}

std::vector<Product> Database::getProducts() {
    std::vector<Product> products;

    try {
        //std::unique_lock<std::mutex> lock(mutex);

        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM products"); // TODO: change employees for a varible that corresponds to the table name
        
        while (res->next()) {
            //int id = res->getInt("product_id");
            std::string name = res->getString("name");
            double price = res->getDouble("price");
            std::string color = res->getString("color");
            int page = res->getInt("page");
            bool deployable = res->getBoolean("deployable");

            Product product = { name, price, color, page, deployable};

            //std::cout << "Id: " << id << ". Employee name: " << name << " with level " << level << " and start time " << start << std::endl;
            products.push_back(product);
        }

        return products;
    }
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get products. Error message: " << e.what();
        return products;
    }
}

Product Database::getProductByName(const std::string name) {
    Product product;
    try {
        //std::unique_lock<std::mutex> lock(mutex);
        
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
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get product by name. Error message: " << e.what();
        return product; // Return an empty Product on error
    }
}

int Database::getProductIdByName(const std::string name) {
    int id = 0;

    try {
        //std::unique_lock<std::mutex> lock(mutex);

        std::stringstream query;
        query << "SELECT product_id FROM products WHERE name = '" << name << "'";
        sql::ResultSet* res = stmt->executeQuery(query.str());

        if (res->next()) {
            id = res->getInt("product_id");
        }

        return id;
    }
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get product id by name. Error message: " << e.what();
        return id; // Return an empty Product on error
    }
}

std::vector<Product> Database::getProductsByDeployableId(int deployable_id) {
    std::vector<Product> products;
    try {
        //std::unique_lock<std::mutex> lock(mutex);

        std::stringstream query;
        query << "SELECT * FROM products WHERE deployable = '" << deployable_id << "'";
        sql::ResultSet* res = stmt->executeQuery(query.str());

        while(res->next()) {
            std::string name = res->getString("name");
            double price = res->getDouble("price");
            std::string color = res->getString("color");
            int page = res->getInt("page");
            int deployable = res->getInt("deployable");

            Product p(name, price, color, page, deployable);
            products.push_back(p);
        }

        return products;
    }
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get products by deployable id. Error message: " << e.what();
        return products; // Return an empty vector on error
    }
}

std::pair<int, std::vector<Product>> Database::getProductsAndIds() {
    std::vector<Product> products = getProducts();

    return std::pair<int, std::vector<Product>>();
}

std::vector<Order> Database::getOrders() {
    /*std::vector<Order> orders;

    try {
        //std::unique_lock<std::mutex> lock(mutex);

        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM orders");

        while (res->next()) {
            std::string time = res->getString("time");
            std::string message = res->getString("message");

            Order order = { time, message };

            orders.push_back(order);
        }
    }
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get orders. Error message: " << e.what();
    }

    return orders;*/
}

Order Database::getOrderByTime(const std::string time) {
    /*Order order;

    try {
        //std::unique_lock<std::mutex> lock(mutex);

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
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get order by time. Error message: " << e.what();
    }

    return order;*/
}

std::vector<Ingredient> Database::getIngredients() {
    std::vector<Ingredient> ingredients;

    try {
        //std::unique_lock<std::mutex> lock(mutex);

        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM ingredients");

        while (res->next()) {
            std::string name = res->getString("name");
            Ingredient ingredient = { name };
            ingredients.push_back(ingredient);
        }

        return ingredients;
    }
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get ingredients. Error message: " << e.what();
        return ingredients;
    }
}

Ingredient Database::getIngredientByName(const std::string name) {
    Ingredient ingredient;

    try {
        //std::unique_lock<std::mutex> lock(mutex);

        std::stringstream query;
        query << "SELECT * FROM ingredients WHERE name = '" << name << "'";
        sql::ResultSet* res = stmt->executeQuery(query.str());

        if (res->next()) {
            std::string name = res->getString("name");
            ingredient.name = name;
        }

        return ingredient;
    }
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get ingredient by name. Error message: " << e.what();
        return ingredient; // Return an empty Ingredient on error
    }
}

std::vector<Allergen> Database::getAllergens() {
    std::vector<Allergen> allergens;

    try {
        //std::unique_lock<std::mutex> lock(mutex);

        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM allergens");

        while (res->next()) {
            std::string name = res->getString("name");
            Allergen allergen = { name };
            allergens.push_back(allergen);
        }

        return allergens;
    }
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get allergens. Error message: " << e.what();
        return std::vector<Allergen>();
    }
}

Allergen Database::getAllergenByName(const std::string name) {
    Allergen allergen;

    try {
        //std::unique_lock<std::mutex> lock(mutex);

        std::stringstream query;
        query << "SELECT * FROM allergens WHERE name = '" << name << "'";
        sql::ResultSet* res = stmt->executeQuery(query.str());

        if (res->next()) {
            std::string name = res->getString("name");
            allergen.name = name;
        }

        return allergen;
    }
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not get allergen by name. Error message: " << e.what();
        return allergen; // Return an empty Allergen on error
    }
}

std::vector<page_t> Database::getDataFromPages(){
    std::vector<page_t> pages(N_FOURTH_ROW_BUTTONS);
    int i = 0;

    for (auto& p : pages) {
        std::stringstream query;
        query << "SELECT * FROM products WHERE page = '" << i+1 << "'";
        sql::ResultSet* res = stmt->executeQuery(query.str());
        query.str("");
        std::cout << "pag " << i << std::endl;

        while(res->next()) {
            std::string name = res->getString("name");
            double price = res->getDouble("price");
            std::string color = res->getString("color");
            int page = res->getInt("page");
            bool deployable = res->getBoolean("deployable");

            Product p1(name, price, color, page, deployable);
            std::vector<Product> deployable_vector;

            if (deployable == 0) {
                if (price == 0) {
                    query << "SELECT product_id FROM products WHERE name = '" << name << "' AND price = 0 AND color = '" << color << "' AND page = '" << page << "' AND deployable = 0";
                    sql::ResultSet* res2 = stmt->executeQuery(query.str());
                    query.str("");

                    if (res2->next()) {
                        int deployable_id = res->getInt("product_id");

                        query << "SELECT * FROM products WHERE deployable = '" << deployable_id << "'";
                        res2 = stmt->executeQuery(query.str());
                        query.str("");

                        deployable_vector = { Product("", 0.0, "#FFFFFF", 0, 0)};
                        while (res2->next()) {
                            name = res2->getString("name");
                            price = res2->getDouble("price");
                            color = res2->getString("color");
                            page = res2->getInt("page");
                            deployable = res2->getBoolean("deployable");
      
                            Product p2(name, price, color, page, deployable);
                            deployable_vector.push_back(p2);
                        }
                    }
                }
            }
            
            p.push_back({ p1, deployable_vector });
            if (!deployable_vector.empty()) std::cout << p1.name << " " << deployable_vector.back().name << std::endl;
        }

        i++;
    }

    return pages;
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
    /*std::vector<Order> orders = getOrders();

    CROW_LOG_INFO << "[LIST] Orders: ";
    for (const auto order : orders) {
        std::cout << "\t- Time: " << order.time << ", Message: " << order.message << std::endl;
    }*/
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



// Change
void Database::moveTable(int current_n_table, const int new_n_table) {
    try {
        std::stringstream query;

        query << "SELECT * FROM tables WHERE n_table = " << current_n_table;
        sql::ResultSet* res = stmt->executeQuery(query.str());
        query.str("");

        res->next();
        int current_table_id = res->getInt("table_id");

        query << "SELECT table_id, bill FROM tables WHERE n_table = " << new_n_table;
        res = stmt->executeQuery(query.str());
        query.str("");

        Table curr_table = getTableByNumber(current_n_table);
        Table new_table = getTableByNumber(new_n_table);

        // If theres an existing table
        if (res->next()) {
            int new_table_id = res->getInt("table_id");
            int new_table_bill = res->getInt("bill");

            query << "SELECT product_id, amount FROM tableproduct WHERE table_id =" << current_table_id;
            res = stmt->executeQuery(query.str());
            query.str("");

            // For each product of current table
            while (res->next()) {
                int current_table_product_id = res->getInt("product_id");
                int current_table_product_amount = res->getInt("amount");

                query << "SELECT * FROM tableproduct WHERE table_id =" << new_table_id << " AND product_id = " << current_table_product_id;
                sql::ResultSet* res2 = stmt->executeQuery(query.str());
                query.str("");

                // If product_id matches between current and new table. Update amount and new table bill
                if (res2->next()) {
                    int new_table_product_id = res2->getInt("product_id");
                    int new_table_amount = res2->getInt("amount");

                    query << "SELECT * FROM tableproduct WHERE table_id =" << current_table_id << " AND product_id = " << current_table_product_id;
                    sql::ResultSet* res3 = stmt->executeQuery(query.str());
                    query.str("");

                    if (res3->next()) {
                        int current_table_amount = res3->getInt("amount");
                        new_table_amount += current_table_amount;

                        // Update new table product amount
                        pstmt = con->prepareStatement("UPDATE tableproduct SET amount = ? WHERE table_id = ? AND product_id = ?");
                        pstmt->setInt(1, new_table_amount);
                        pstmt->setInt(2, new_table_id);
                        pstmt->setInt(3, current_table_product_id);
                        pstmt->execute();

                        query << "SELECT price FROM products WHERE product_id =" << current_table_product_id;
                        res3 = stmt->executeQuery(query.str());
                        query.str("");

                        if (res3->next()) {
                            int current_table_product_price = res3->getInt("price");
                            new_table_bill += current_table_product_price * current_table_product_amount;

                            // Update new table bill
                            pstmt = con->prepareStatement("UPDATE tables SET bill = ? WHERE table_id = ?");
                            pstmt->setInt(1, new_table_bill);
                            pstmt->setInt(2, new_table_id);
                            pstmt->execute();

                            // Delete the current table tableproduct row
                            pstmt = con->prepareStatement("DELETE FROM tableproduct WHERE table_id = ? AND product_id = ?");
                            pstmt->setInt(1, current_table_id);
                            pstmt->setInt(2, current_table_product_id);
                            pstmt->execute();
                        }
                    }
                }
                else { // If product doesnt match, change its table_id and update table bill
                    pstmt = con->prepareStatement("UPDATE tableproduct SET table_id = ? WHERE table_id = ? AND product_id = ?");
                    pstmt->setInt(1, new_table_id);
                    pstmt->setInt(2, current_table_id);
                    pstmt->setInt(3, current_table_product_id);
                    pstmt->execute();

                    query << "SELECT price FROM products WHERE product_id =" << current_table_product_id;
                    sql::ResultSet* res3 = stmt->executeQuery(query.str());
                    query.str("");

                    res3->next();
                    int current_table_product_price = res3->getInt("price");
                    new_table_bill += (current_table_product_price * current_table_product_amount);

                    pstmt = con->prepareStatement("UPDATE tables SET bill = ? WHERE table_id = ?");
                    pstmt->setInt(1, new_table_bill);
                    pstmt->setInt(2, new_table_id);
                    pstmt->execute();
                }
            }

            removeTable(curr_table);
;       }
        else { // If there is no existing table just change n_table
            pstmt = con->prepareStatement("UPDATE tables SET n_table = ? WHERE table_id = ?");
            pstmt->setInt(1, new_n_table);
            pstmt->setInt(2, current_table_id);
            pstmt->execute();
        }
    }
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not change table number. Error message: " << e.what();
    }
}


// Remove
void Database::removeTable(const Table& table) {
    try {
        int n_table = table.n_table;
        double bill = table.bill;
        double discount = table.discount;

        std::stringstream query;
        query << "SELECT * FROM tables WHERE n_table = " << table.n_table;
        sql::ResultSet* res = stmt->executeQuery(query.str());
        query.str("");

        if (res->next()) {
            int table_id = res->getInt("table_id");

            pstmt = con->prepareStatement("DELETE FROM tableproduct WHERE table_id = ?");
            pstmt->setInt(1, table_id);
            pstmt->execute();

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
    }
    catch (const sql::SQLException& e) {
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
    catch (const sql::SQLException& e) {
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
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not remove product. Error message: " << e.what();
    }
}

void Database::removeOrder(const Order& order) {
    /*try {
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
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not remove order. Error message: " << e.what();
    }*/
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
    catch (const sql::SQLException& e) {
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
    catch (const sql::SQLException& e) {
        CROW_LOG_WARNING << "[EXCEPTION] Could not remove allergen. Error message: " << e.what();
    }
}