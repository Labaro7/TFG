#include "..\headers\ingredient.h"

Ingredient::Ingredient() : name("") {}

Ingredient::~Ingredient() {}


// Get
std::string Ingredient::getName() const { return name; }


// Set
void Ingredient::setName(std::string name) { this->name = name; }


// Empty
bool Ingredient::isEmpty() { return name == ""; }