#pragma once
#ifndef _WORKER_HPP_
#define _WORKER_HPP_

#include <string>

class Worker {
public:
	Worker();
	Worker(std::string name, int rank, std::string start, std::string finish);
	~Worker();

	std::string getName();
	int getRank();
	std::string getStart();
	std::string getFinish();
	

private:
	std::string name;
	int rank;
	std::string start; // Make a Date type?
	std::string finish;
};

#endif