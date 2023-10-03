#pragma once
#ifndef _RESTAURANT_HPP_
#define _RESTAURANT_HPP_

#include "worker.h"
#include "table.h"
#include <vector>

class Restaurant {
public:



private:
	std::vector<Worker> workers;
	std::vector<Table> tables;
	double totalMoney;
};

#endif