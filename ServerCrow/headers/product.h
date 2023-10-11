#pragma once
#ifndef _PRODUCT_HPP_
#define _PRODUCT_HPP_

#include <string>
#include <vector>

class Product {
public:
	Product();
	~Product();

	std::string getName();
	double getPrice();

private:
	std::string name;
	double price;
	//std::vector<std::string> ingredients;
	//std::vector<std::string> extras;
	//std::vector<std::string> allergens;
	//std::string messageToKitchen;
	//unsigned n_order;

}; // class Product
#endif
