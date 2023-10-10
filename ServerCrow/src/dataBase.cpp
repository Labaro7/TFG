#include "..\headers\dataBase.h"


DataBase::DataBase() : stmt(0), pstmt(0) {
    try
    {
        driver = get_driver_instance();
        con = driver->connect(server, username, password);
        stmt = con->createStatement();

    }
    catch (sql::SQLException e)
    {
        std::cout << "Could not connect to server. Error message: " << e.what() << std::endl;
        system("pause");
        exit(1);
    }

    con->setSchema(dataBaseName);
}

DataBase::~DataBase() {
    delete con;
    delete stmt;
    delete pstmt;
}


void DataBase::saveTable(int n_table) {
    pstmt = con->prepareStatement("INSERT INTO tables(n_table) VALUES(?)");
    pstmt->setInt(1, n_table);
    pstmt->execute();

    std::cout << "Table " << n_table << " inserted into tables." << std::endl;
}

void DataBase::saveWorker(Worker* worker) {
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

    std::cout << "Worker " << name << " with rank " << rank << " starting at " << start << " inserted into workers." << std::endl;
}

void DataBase::saveProduct(Product* product) {

}

std::vector<Table> DataBase::getTables() {
    std::vector<Table> tables;
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM tables");
    Table table;

    while (res->next()) {
        int id = res->getInt("id_tables");
        int n_table = res->getInt("n_table");

        table.setTableNumber(n_table);

        std::cout << "Id: " << id << ", Table Number: " << n_table << std::endl;
        tables.push_back(table);
    } 

    return tables;
}

Table DataBase::getTable(int n_table) {
    Table t;
    std::stringstream query;
    query << "SELECT * FROM tables WHERE n_table = " << n_table;
    sql::ResultSet* res = stmt->executeQuery(query.str());

    while (res->next()) {
        int n = res->getInt("n_table");
        t.setTableNumber(n);
    }

    return t;

}

// Getters.
sql::Driver* DataBase::getDriver() { return driver; }
sql::Connection* DataBase::getCon() { return con; }
sql::Statement* DataBase::getStmt() { return stmt; }
sql::PreparedStatement* DataBase::getPstmt() { return pstmt; }

// Setters.