#ifndef __INGREDIENT_HPP_
#define __INGREDIENT_HPP_

#include <string>

class Ingredient {
public:
	Ingredient();
	~Ingredient();

	std::string getName();

	void setName(std::string name);


private:
	std::string name;

}; // class Ingredient
#endif