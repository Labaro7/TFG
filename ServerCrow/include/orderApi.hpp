#pragma once
#ifndef _ORDER_API_HPP_
#define _ORDER_API_HPP_

#include "crow_all.hpp"
#include "apiRoute.hpp"
#include <string>

class OrderAPI : public ApiRoute
{
public:
	OrderAPI(std::shared_ptr<std::shared_ptr<Database>> database_ptr);

	std::string extractURISegment(std::string& uri) override;

	crow::json::wvalue buildOrdersJSON(std::vector<Order> orders);

	crow::json::wvalue processRequest(Conn& conn, std::string& uri) override;
};

#endif