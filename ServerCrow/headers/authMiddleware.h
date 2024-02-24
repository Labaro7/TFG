#pragma once
#ifndef _AUTHMIDDLEWARE_HPP_
#define _AUTHMIDDLEWARE_HPP_

#include "crow_all.h"
#include "constants.h"
#include "server.h"
#include "database.h"

class AuthMiddleware
{
public:
	struct context
	{
	};

	void before_handle(crow::request& req,
					   crow::response& res,
					   context& ctx);

	void after_handle(crow::request& req,
					  crow::response& res,
					  context& ctx);

private:
	Database database;
};

#endif // !_AUTHMIDDLEWARE_HPP_

