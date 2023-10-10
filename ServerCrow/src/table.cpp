#include "..\headers\table.h"

Table::Table() : n_table(0) {}

Table::Table(int n_table) : n_table(n_table){}

Table::~Table() {}

int Table::getTableNumber() { return n_table; }

void Table::setTableNumber(int n) { n_table = n; }