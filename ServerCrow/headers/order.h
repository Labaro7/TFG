#ifndef __ORDER_HPP__
#define __ORDER_HPP__

#include <string>

class Order {
public:
	Order();
	~Order();

	// Get
	std::string getTime();
	std::string getMessage();

	// Set
	void setTime(std::string time);
	void setMessage(std::string message);


private:
	std::string time;
	std::string message;
	// int n_table
}; // class Order
#endif