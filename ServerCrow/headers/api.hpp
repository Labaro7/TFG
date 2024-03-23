#pragma once
#ifndef _API_HPP_
#define _API_HPP_

#include "crow_all.hpp"
#include <string>
#include "database.hpp"
#include "orderApi.hpp"
#include "billApi.hpp"
#include "nClientApi.hpp"

class Database;

class API
{
public:
	API(std::shared_ptr<Database> database);

	enum class DirectionCode
	{
		ERR = -1,
		ORDER = 0,
		BILL = 1,
		NCLIENT = 2,
	};

	std::string extractDirection(std::string& uri);

	DirectionCode getDirectionCode(std::string& uri);

	crow::json::wvalue retrieveRequest(std::string& uri);

protected:
	API() = delete;
	struct CustomDeleter
	{
		void operator()(int* ptr) const
		{
			std::cout << "Custom deleter: Deleting pointer at address: " << ptr << std::endl;
			delete ptr; // Custom cleanup operation
		}
	};
	std::shared_ptr<Database> database;

	std::shared_ptr<OrderAPI> orderAPI;
	std::shared_ptr<BillAPI> billAPI;
	std::shared_ptr<NClientAPI> nClientAPI;
};

#endif