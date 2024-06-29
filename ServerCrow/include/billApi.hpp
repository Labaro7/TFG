#pragma once
#ifndef _BILL_API_HPP_
#define _BILL_API_HPP_

#include "crow_all.hpp"
#include "apiRoute.hpp"
#include <string>

class BillAPI : public ApiRoute
{
public:
	BillAPI(std::shared_ptr<std::shared_ptr<Database>> database_ptr);

	std::string extractURISegment(std::string& uri) override;

	crow::json::wvalue buildBillsJSON(std::vector<BillAndPaid> billsAndPaids);

	crow::json::wvalue processRequest(Conn& conn, std::string& uri) override;
};

#endif