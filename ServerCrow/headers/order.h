#ifndef _ORDER_HPP_
#define _ORDER_HPP_

#include <string>

class Order {
public:
	Order();
	~Order();

	// Get
	std::string getTime() const;
	std::string getMessage() const;

	// Set
	void setTime(std::string time);
	void setMessage(std::string message);


private:
	std::string time;
	std::string message;
	// int n_table
}; // class Order
#endif