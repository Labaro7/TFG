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

std::string Worker::getName() { return name; }
int Worker::getRank() { return rank; }
std::string Worker::getStart() { return start; }
std::string Worker::getFinish() { return finish; }
