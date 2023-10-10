#pragma once
#ifndef _TABLE_HPP_
#define _TABLE_HPP_

#include "product.h"
#include <map>

class Table {
public:
	Table();
	Table(int n_table);
	~Table();
	
	int getTableNumber();
	void setTableNumber(int n);


private:
	//std::map<Product, int> products;
	//double totalPrice;
	//unsigned clients;
	//double discount;
	int n_table;
};


#endif