#ifndef __INGREDIENT_HPP_
#define __INGREDIENT_HPP_

#include <string>

class Ingredient {
public:
	Ingredient();
	~Ingredient();


	// Get
	std::string getName() const;


	// Set
	void setName(std::string name);


	// Get
	bool isEmpty();


private:
	std::string name;

}; // class Ingredient
#endif