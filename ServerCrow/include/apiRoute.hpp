#pragma once
#ifndef _API_ROUTE_HPP_
#define _API_ROUTE_HPP_

#include "crow_all.hpp"
#include "database.hpp"
#include <string>

class ApiRoute
{
public:
	virtual std::string extractURISegment(std::string& uri) = 0;

	using Conn = std::unique_ptr<sql::Connection>;
	virtual crow::json::wvalue processRequest(Conn& conn, std::string& uri) = 0;

protected:
	std::unique_ptr<sql::Connection> conn;
	std::shared_ptr<Database> database;
	std::shared_ptr<std::shared_ptr<Database>> database_ptr;
};

#endif