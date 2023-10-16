#include "..\headers\allergen.h"

Allergen::Allergen() : name("") {}
Allergen::~Allergen() {}

std::string Allergen::getName() const { return name; }

void Allergen::setName(std::string name) { this->name = name; }