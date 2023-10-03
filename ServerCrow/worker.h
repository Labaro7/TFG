#pragma once
#ifndef _WORKER_HPP_
#define _WORKER_HPP_

#include <string>

class Worker {
public:
	Worker() {}
	~Worker() {}

private:
	std::string name;
	unsigned rank;
	unsigned start; // Make a Date type?
	unsigned finish;
};

#endif