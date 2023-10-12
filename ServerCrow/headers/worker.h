#pragma once
#ifndef _WORKER_HPP_
#define _WORKER_HPP_

#include <string>

class Worker {
public:
	Worker();
	Worker(std::string name, int level, std::string start, std::string finish);
	~Worker();

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

}; // class Worker
#endif