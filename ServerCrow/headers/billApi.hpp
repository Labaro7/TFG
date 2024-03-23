#pragma once
#ifndef _BILL_API_HPP_
#define _BILL_API_HPP_

#include "crow_all.hpp"
#include "apiRoute.hpp"
#include <string>

class BillAPI : public ApiRoute
{
public:
	BillAPI(std::shared_ptr<Database> database);

	std::string extractDirection(std::string& uri) override;

	crow::json::wvalue processRequest(std::string& uri) override;
};

#endif