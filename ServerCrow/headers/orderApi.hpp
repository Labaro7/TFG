#pragma once
#ifndef _ORDER_API_HPP_
#define _ORDER_API_HPP_

#include "crow_all.hpp"
#include "apiRoute.hpp"
#include <string>

class OrderAPI : public ApiRoute
{
public:
	OrderAPI(std::shared_ptr<Database> database);

	std::string extractDirection(std::string& uri) override;

	crow::json::wvalue processRequest(std::string& uri) override;
	crow::json::wvalue buildOrdersJSON(std::vector<Order> orders);
	crow::json::wvalue getOrdersByDay(const std::string date);
	crow::json::wvalue getOrdersByWeek(const std::string date);
	crow::json::wvalue getOrdersByMonth(const std::string date);
	crow::json::wvalue getOrdersByYear(const std::string date);

private:
	OrderAPI() = delete;
};

#endif