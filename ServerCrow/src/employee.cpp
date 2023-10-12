#include "..\headers\employee.h"

Employee::Employee():
	name(""),
	level(0),
	start(""),
	finish(""){
}

Employee::Employee(std::string name, int level, std::string start, std::string finish) : 
	name(name),
	level(level),
	start(start),
	finish(finish){}

Employee::~Employee() {}

// Getters
std::string Employee::getName() { return name; }
int Employee::getLevel() { return level; }
std::string Employee::getStart() { return start; }
std::string Employee::getFinish() { return finish; }

// Setters
void Employee::setName(std::string name) { this->name = name; }
void Employee::setLevel(int level) { this->level = level;  }
void Employee::setStart(std::string start) { this->start = start;  }
void Employee::setFinish(std::string finish) { this->finish = finish; }
void Employee::set(std::string name, int level, std::string start, std::string finish) {
	setName(name);
	setLevel(level);
	setStart(start);
	setFinish(finish);
}
