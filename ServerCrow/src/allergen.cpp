#include "..\headers\allergen.h"

Allergen::Allergen() : name("") {}

Allergen::~Allergen() {}


// Get
std::string Allergen::getName() const { return name; }


// Set
void Allergen::setName(std::string name) { this->name = name; }


// Empty
bool Allergen::isEmpty() { return name == ""; }