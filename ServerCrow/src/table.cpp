#include "..\headers\table.h"

Table::Table() : n_table(0), n_clients(0), bill(0.0), discount(0.0) {}

Table::Table(int n_table) : n_table(n_table), n_clients(0), bill(0.0), discount(0.0) {}

Table::Table(int n_table, int n_clients) : n_table(n_table), n_clients(n_clients), bill(0.0), discount(0.0) {}

Table::~Table() {}


// Get 
int Table::getNTable() const { return n_table; }

int Table::getNClients() const { return n_clients; }

double Table::getBill() const { return bill; }

double Table::getDiscount() const { return discount; }


// Set
void Table::setNTable(int n) { this->n_table = n_table; }

void Table::setNClients(int n_clients) { this->n_clients = n_clients; }

void Table::setBill(double bill) { this->bill = bill; }

void Table::setDiscount(double discount) { this->discount = discount; }

void Table::set(Table* table) {
	setNTable(table->getNTable());
	setNClients(table->getNClients());
	setBill(table->getBill());
	setDiscount(table->getDiscount());
}


// Empty
bool Table::isEmpty() {
	return n_table == 0 && n_clients == 0 && bill == 0 && discount == 0;
}