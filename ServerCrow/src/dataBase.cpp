#include "..\headers\dataBase.h"

Database::Database() : stmt(0), pstmt(0) {
    try{
        driver = get_driver_instance();
        con = driver->connect(server, username, password);
        stmt = con->createStatement();
    }
    catch (sql::SQLException e){
        std::cout << "Could not connect to server. Error message: " << e.what() << std::endl;
        system("pause");
        exit(1);
    }

    con->setSchema(dataBaseName);
}

Database::~Database() {
    delete con; // They must be deleted when they are not going to be used anymore
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
    sql::Statement* stmt = con->createStatement();
    stmt->execute("CREATE DATABASE " + name);
}

void Database::MySqlDropDatabase(std::string name) {
    sql::Statement* stmt = con->createStatement();
    stmt->execute("DROP DATABASE " + name);
}

void Database::MySqlUseDatabase(std::string name) {
    sql::Statement* stmt = con->createStatement();
    stmt->execute("USE " + name);
}

void Database::MySqlSaveChangesToDataBase() {
    sql::Statement* stmt = con->createStatement();
    stmt->execute("COMMIT");
}


// Table
void Database::MySqlCreateTable(std::string name, std::string definition) {
    sql::Statement* stmt = con->createStatement();
    stmt->execute("CREATE TABLE " + name + " (" + definition + ")");
    std::cout << "hey" << std::endl;
}

void Database::MySqlDropTable(std::string name) {
    sql::Statement* stmt = con->createStatement();
    stmt->execute("DROP TABLE IF EXISTS " + name);
}

void Database::MySqlModifyTable(std::string name, std::string modification) {
    sql::Statement* stmt = con->createStatement();
    stmt->execute("ALTER TABLE " + name + " " + modification);
}

void Database::MySqlEmptyTable(std::string name) {
    sql::Statement* stmt = con->createStatement();
    stmt->execute("DELETE FROM " + name);
}

// ------------------------------- /MySQL queries ------------------------------- //


void Database::initialize() {
    // TODO: Get tables names and columns from a file that specifies the format wanted
    MySqlCreateTable("tables", "table_id INT AUTO_INCREMENT PRIMARY KEY, n_table INT, n_clients INT, bill DOUBLE, discount DOUBLE");
    MySqlCreateTable("employees", "employee_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45), level INT, start VARCHAR(45), finish VARCHAR(45)");
    MySqlCreateTable("products", "product_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45)");
    MySqlCreateTable("orders", "order_id INT AUTO_INCREMENT PRIMARY KEY, time VARCHAR(45), message VARCHAR(45)");
    MySqlCreateTable("ingredients", "ingredient_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45)");
    MySqlCreateTable("allergens", "allergen_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45)");
    MySqlCreateTable("restaurants", "restaurant_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(45)");

    // TODO: Create Junction tables

}

// Save
void Database::saveTable(Table* table) {
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

void Database::saveEmployee(Employee* employee) {
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

void Database::saveProduct(Product* product) {
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

void Database::saveOrder(Order* order) {
    std::string time = order->getTime();
    std::string message = order->getMessage();

    std::ostringstream oss;
    oss << time << "," << message;
    std::string values = oss.str();

    pstmt = con->prepareStatement("INSERT INTO orders(time, message) VALUES(?,?)");

    pstmt->setString(1, time);
    pstmt->setString(2, message);
    pstmt->execute();

    std::cout << "Order with time" << time <<
        " and message " << message <<
        " inserted into orders." << std::endl;
}

void Database::saveIngredient(Ingredient* ingredient) {
    std::string name = ingredient->getName();

    std::ostringstream oss;
    oss << name;
    std::string values = oss.str();

    pstmt = con->prepareStatement("INSERT INTO ingredients(name) VALUES(?)");

    pstmt->setString(1, name);

    std::cout << "Ingredient with name" << name << 
        " inserted into ingredients." << std::endl;
}

void Database::saveAllergen(Allergen* allergen) {
    std::string name = allergen->getName();

    std::ostringstream oss;
    oss << name;
    std::string values = oss.str();

    pstmt = con->prepareStatement("INSERT INTO allergens(name) VALUES(?)");

    pstmt->setString(1, name);

    std::cout << "Allergen with name" << name <<
        " inserted into allergens." << std::endl;
}


//Get
std::vector<Table> Database::getTables() {
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

Table Database::getTableByNumber(int n_table) {
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

std::vector<Employee> Database::getEmployees() {
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

Employee Database::getEmployeeByName(std::string name) {
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

}

void Database::removeEmployee(Employee* employee){
}

void Database::removeProduct(Product* product){
}

void Database::removeOrder(Order* order){
}

void Database::removeIngredient(Ingredient* ingredient){
}

void Database::removeAllergen(Allergen* allergen){
}