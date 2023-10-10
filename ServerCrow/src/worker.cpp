#include "..\headers\worker.h"

Worker::Worker():
	name(""),
	rank(0),
	start(""),
	finish(""){
}

Worker::Worker(std::string name, int rank, std::string start, std::string finish) : 
	name(name),
	rank(rank),
	start(start),
	finish(finish){}

Worker::~Worker() {}

// Getters
std::string Worker::getName() { return name; }
int Worker::getRank() { return rank; }
std::string Worker::getStart() { return start; }
std::string Worker::getFinish() { return finish; }

// Setters
void Worker::setName(std::string name) { this->name = name; }
void Worker::setRank(int rank) { this->rank = rank;  }
void Worker::setStart(std::string start) { this->start = start;  }
void Worker::setFinish(std::string finish) { this->finish = finish; }
void Worker::set(std::string name, int rank, std::string start, std::string finish) {
	setName(name);
	setRank(rank);
	setStart(start);
	setFinish(finish);
}
