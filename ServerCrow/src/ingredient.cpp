#include "..\headers\ingredient.h"

Ingredient::Ingredient() : name(0) {}
Ingredient::~Ingredient() {}

std::string Ingredient::getName() { return name; }

void Ingredient::setName(std::string name) { this->name = name; }