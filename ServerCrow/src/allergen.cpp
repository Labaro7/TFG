#include "..\headers\allergen.h"

Allergen::Allergen() : name(0) {}
Allergen::~Allergen() {}

std::string Allergen::getName() { return name; }

void Allergen::setName(std::string name) { this->name = name; }