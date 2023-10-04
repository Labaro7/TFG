#pragma once
#ifndef _TABLE_HPP_
#define _TABLE_HPP_

#include "product.h"
#include <map>

class Table {
public:
	Table() {}
	Table(unsigned n_table) : n_table(n_table) {}
	~Table() {}

	unsigned getNumber();


private:
	//std::map<Product, int> products;
	//double totalPrice;
	//unsigned clients;
	//double discount;
	unsigned n_table;
};

unsigned Table::getNumber() {
	return n_table;
}

#endif