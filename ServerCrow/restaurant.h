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
};


void Restaurant::addTable(std::shared_ptr<Table> table) {
	tables.push_back(table);
}

void Restaurant::removeTable(int tableNumber) {
	tables.pop_back();
}

void Restaurant::printTables() {
	for (auto t : tables) {
		std::cout << t->getNumber() << " ";
	}
}

#endif