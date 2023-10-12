#include "..\headers\worker.h"

Worker::Worker():
	name(""),
	level(0),
	start(""),
	finish(""){
}

Worker::Worker(std::string name, int level, std::string start, std::string finish) : 
	name(name),
	level(level),
	start(start),
	finish(finish){}

Worker::~Worker() {}

// Getters
std::string Worker::getName() { return name; }
int Worker::getLevel() { return level; }
std::string Worker::getStart() { return start; }
std::string Worker::getFinish() { return finish; }

// Setters
void Worker::setName(std::string name) { this->name = name; }
void Worker::setLevel(int level) { this->level = level;  }
void Worker::setStart(std::string start) { this->start = start;  }
void Worker::setFinish(std::string finish) { this->finish = finish; }
void Worker::set(std::string name, int level, std::string start, std::string finish) {
	setName(name);
	setLevel(level);
	setStart(start);
	setFinish(finish);
}
