#include "..\headers\ingredient.h"

Ingredient::Ingredient() : name("") {}
Ingredient::~Ingredient() {}

std::string Ingredient::getName() const { return name; }

void Ingredient::setName(std::string name) { this->name = name; }