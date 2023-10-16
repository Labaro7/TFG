#pragma once
#ifndef _PRODUCT_HPP_
#define _PRODUCT_HPP_

#include <string>
#include <vector>

class Product {
public:
	Product();
	~Product();


	// Get
	std::string getName() const;
	double getPrice() const;


	// Set
	void setName(std::string name);
	void setPrice(double price);
	void set(std::string name, double price);


	// Empty
	bool isEmpty();

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
