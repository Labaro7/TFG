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

	std::string extractURISegment(std::string& uri);

	DirectionCode getDirectionCode(std::string& uri);

	void decodeURI(std::string& uri);

	crow::json::wvalue retrieveRequest(std::string& uri);

protected:
	API() = delete;

	std::shared_ptr<Database> database;

	std::shared_ptr<OrderAPI> orderAPI;		// /api/orders
	std::shared_ptr<BillAPI> billAPI;		// /api/bills
	std::shared_ptr<NClientAPI> nClientAPI; // /api/n_clients
};

#endif