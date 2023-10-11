#pragma once
#ifndef _RESTAURANT_HPP_
#define _RESTAURANT_HPP_

#include "worker.h"
#include "table.h"
#include <vector>

class Restaurant {
public:

	void addTable(std::shared_ptr<Table> table);
	void printTable(int n_table);	
	void printTables();
	void removeTable(int tableNumber);


private:
	std::vector<Worker> workers;
	std::vector<std::shared_ptr<Table>> tables;
	double totalMoney;

}; // class Restaurant
#endif