#include "..\headers\product.h"

Product::Product() : name(""), price(0) {}

Product::~Product() {}


// Get
std::string Product::getName() const { return name; }

double Product::getPrice() const { return price; }


// Set
void Product::setName(std::string name) { this->name = name; }

void Product::setPrice(double price) { this->price = price; }

void Product::set(std::string name, double price) { this->name = name; this->price = price; }


// Empty
bool Product::isEmpty() { return name == "" && price == 0; }