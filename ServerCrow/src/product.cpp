#include "..\headers\product.h"

Product::Product() : name(""), price(0) {}

Product::~Product() {}

std::string Product::getName() const { return name; }

double Product::getPrice() const { return price; }

void Product::setName(std::string name) { this->name = name; }

void Product::setPrice(double price) { this->price = price; }

void Product::set(std::string name, double price) { this->name = name; this->price = price; }