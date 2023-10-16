#ifndef __ALLERGEN_HPP_
#define __ALLERGEN_HPP_

#include <string>

class Allergen {
public:
	Allergen();
	~Allergen();

	std::string getName() const;

	void setName(std::string name);


private:
	std::string name;

	/*
	GLUTEN
	CRUSTACEANS
	EGGS
	FISH
	PEANUTS
	SOY
	MILKY
	TREE NUTS
	CELERY
	MUSTARD
	SESAME
	SULFITES
	LUPIN
	SULFITES
	*/
}; // class Allergen
#endif