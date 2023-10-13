#include "..\headers\order.h"

Order::Order(): time(""), message("") {} // TODO: time = currentTime

Order::~Order() {}


// Get
std::string Order::getTime() { return time; }

std::string Order::getMessage() { return message; }


// Set
void Order::setTime(std::string time) { this->time = time; }

void Order::setMessage(std::string message) { this->message = message; }