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


// ------------------------------- MySQL methods ------------------------------- //

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


// Row
void Database::MySqlInsertRowIntoTable(std::string name, std::string values){
    sql::Statement* stmt = con->createStatement();
    stmt->execute("INSERT INTO " + name + " VALUES (" + values + ")");
}

void Database::MySqlSelectAllFromTable(std::string name) { // TODO: aa

}

void Database::MySqlSelectRowFromTable(std::string name) { // TODO: bb

}

void Database::MySqlUpdateRowFromTable(std::string name, std::string updates, std::string condition) {
    sql::Statement* stmt = con->createStatement();
    stmt->execute("UPDATE " + name + " SET " + updates + " WHERE " + condition);
}

void Database::MySqlDeleteAllRowsFromTable(std::string name) {
    sql::Statement* stmt = con->createStatement();
    stmt->execute("TRUNCATE TABLE " + name);
}

// ------------------------------- /MySQL methods ------------------------------- //


// Save
void Database::saveTable(Table* table) {
    int n_table = table->getNTable();
    int n_clients = table->getNClients();
    double bill = table->getBill();
    double discount = table->getDiscount();

    pstmt = con->prepareStatement("INSERT INTO tables(n_table) VALUES(?,?,?,?)");
    pstmt->setInt(1, n_table);
    pstmt->setInt(2, n_clients);
    pstmt->setDouble(3, bill);
    pstmt->setDouble(4, discount);
    pstmt->execute();

    std::cout << "Table " << n_table <<
        " with " << n_clients << " clients inserted into tables." << std::endl;
}

void Database::saveWorker(Worker* worker) {
    std::string name = worker->getName();
    int rank = worker->getRank();
    std::string start = worker->getStart();
    std::string finish = worker->getFinish();

    pstmt = con->prepareStatement("INSERT INTO workers(name, `rank`, start, finish) VALUES(?,?,?,?)"); // Error corrected: The word "rank" is reserved in MySQL, it has to be between ``.

    pstmt->setString(1, name);
    pstmt->setInt(2, rank);
    pstmt->setString(3, start);
    pstmt->setString(4, finish);
    pstmt->execute();

    std::cout << "Worker " << name <<
        " with rank " << rank <<
        " starting at " << start <<
        " inserted into workers." << std::endl;
}

void Database::saveProduct(Product* product) {
    std::string name = product->getName();
    double price = product->getPrice();


    pstmt = con->prepareStatement("INSERT INTO workers(name, price) VALUES(?,?)"); // Error corrected: The word "rank" is reserved in MySQL, it has to be between ``.

    pstmt->setString(1, name);
    pstmt->setDouble(2, price);
    pstmt->execute();

    std::cout << "Product " << name <<
        " with price " << price <<
        " inserted into products." << std::endl;
}


//Get
std::vector<Table> Database::getTables() {
    std::vector<Table> tables;
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM tables"); // TODO: change tables for a varible that corresponds to the table name
    Table table;

    while (res->next()) {
        int id = res->getInt("id_tables");
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

std::vector<Worker> Database::getWorkers() {
    std::vector<Worker> workers;
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM workers"); // TODO: change workers for a varible that corresponds to the table name
    Worker worker;

    while (res->next()) {
        int id = res->getInt("id_workers");
        std::string name = res->getString("name");
        int rank = res->getInt("rank");
        std::string start = res->getString("start");
        std::string finish = res->getString("finish");

        worker.set(name, rank, start, finish);

        //std::cout << "Id: " << id << ". Worker name: " << name << " with rank " << rank << " and start time " << start << std::endl;
        workers.push_back(worker);
    }

    return workers;
}

Worker Database::getWorkerByName(std::string name) {
    Worker worker;
    std::stringstream query;
    query << "SELECT * FROM workers WHERE name = '" << name << "'"; // String needs to be inside '' // TODO: change workers for a varible that corresponds to the table name
    sql::ResultSet* res = stmt->executeQuery(query.str());

    if (res->next()) {
        std::string name_ = res->getString("name");
        int rank = res->getInt("rank");
        std::string start = res->getString("start");
        std::string finish = res->getString("finish");

        worker.set(name_, rank, start, finish);
    }

    return worker;
}


// Remove