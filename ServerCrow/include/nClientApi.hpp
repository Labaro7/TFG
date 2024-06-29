#pragma once
#ifndef _NCLIENT_API_HPP_
#define _NCLIENT_API_HPP_

#include "crow_all.hpp"
#include "apiRoute.hpp"
#include <string>

class NClientAPI : public ApiRoute
{
public:
	NClientAPI(std::shared_ptr<std::shared_ptr<Database>> database_ptr);

	std::string extractURISegment(std::string& uri) override;

	crow::json::wvalue buildNClientsJSON(const int& n_clients);

	crow::json::wvalue processRequest(Conn& conn, std::string& uri) override;
};

#endif