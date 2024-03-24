#include "..\headers\billApi.hpp"
#include <regex>

BillAPI::BillAPI(std::shared_ptr<Database> database)
{
	this->database = database;
}

std::string BillAPI::extractURISegment(std::string& uri)
{
	std::string direction;
	int initial_pos = 0;

	if (uri[0] == '/')
	{
		initial_pos = 1;
	}

	size_t pos = uri.find('/', initial_pos);
	if (pos == std::string::npos)
	{
		pos = uri.find('\0', initial_pos);
	}

	direction = uri.substr(0, pos);

	if (pos < uri.length() - 1 && uri[pos] == '/')
	{
		pos++;
	}

	uri = uri.substr(pos);

	return direction;
}

crow::json::wvalue BillAPI::buildBillsJSON(std::vector<Order> orders)
{
	crow::json::wvalue::list data;



	return data;
}

crow::json::wvalue BillAPI::processRequest(std::string& uri)
{
	crow::json::wvalue res;

	std::string mode = extractURISegment(uri);

	CROW_LOG_INFO << "[BillAPI] Get orders by " << mode;

	return res;
}