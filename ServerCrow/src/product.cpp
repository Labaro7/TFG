#include "..\headers\product.h"

Product::Product() : name("") {}

Product::~Product() {}

std::string Product::getName() { return name; }

double Product::getPrice() { return price; }

void Product::setName(std::string name) { this->name = name; }

void Product::setPrice(double price) { this->price = price; }