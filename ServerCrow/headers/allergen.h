#ifndef __ALLERGEN_HPP_
#define __ALLERGEN_HPP_

#include <string>

class Allergen {
public:
	Allergen();
	~Allergen();


	// Get
	std::string getName() const;


	// Set
	void setName(std::string name);


	// Empty
	bool isEmpty();


private:
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
	std::string name;

}; // class Allergen
#endif