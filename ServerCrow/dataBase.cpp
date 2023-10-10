#include "dataBase.h"


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
    pstmt = con->prepareStatement("INSERT INTO workers(name, rank, start, finish) VALUES(?,?,?,?)"); // Error here
    std::string name = worker->getName();
    int rank = worker->getRank();
    std::string start = worker->getStart();
    std::string finish = worker->getFinish();
    
    pstmt->setString(1, name);
    pstmt->setInt(2, rank);
    pstmt->setString(3, start);
    pstmt->setString(4, finish);
    pstmt->execute();
    std::cout << "Worker " << name << " with rank " << rank << " starting at " << start << " inserted into workers." << std::endl;
}

// Getters.
sql::Driver* DataBase::getDriver() { return driver; }
sql::Connection* DataBase::getCon() { return con; }
sql::Statement* DataBase::getStmt() { return stmt; }
sql::PreparedStatement* DataBase::getPstmt() { return pstmt; }

// Setters.