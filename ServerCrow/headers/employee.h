#pragma once
#ifndef _EMPLOYEE_HPP_
#define _EMPLOYEE_HPP_

#include <string>

class Employee {
public:
	Employee();
	Employee(std::string name, int level, std::string start, std::string finish);
	~Employee();

	// Getters
	std::string getName();
	int getLevel();
	std::string getStart();
	std::string getFinish();

	// Setters
	void setName(std::string name);
	void setLevel(int level);
	void setStart(std::string start);
	void setFinish(std::string finish);
	void set(std::string name, int level, std::string start, std::string finish);

private:
	std::string name;
	int level;
	std::string start; // Make a Date type?
	std::string finish; // Make a Date type?
	//std::string id;

}; // class Employee
#endif