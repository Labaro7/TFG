#include "..\headers\product.h"

Product::Product() {}

Product::~Product() {}

std::string Product::getName() { return name; }

double Product::getPrice() { return price; }