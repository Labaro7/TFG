#pragma once
#ifndef _TABLE_HPP_
#define _TABLE_HPP_

#include "product.h"
#include <map>

class Table {
public:
	Table();
	Table(int n_table);
	Table(int n_table, int n_clients);
	~Table();
	
	// Get
	int getNTable() const;
	int getNClients() const;
	double getBill() const;
	double getDiscount() const;

	// Set
	void setNTable(int n_table);
	void setNClients(int n_clients);
	void setBill(double bill);
	void setDiscount(double discount);
	void set(Table* table);

private:
	int n_table;
	int n_clients;
	double bill;
	double discount;

}; // class Table
#endif