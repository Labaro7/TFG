#include "..\headers\billApi.hpp"

BillAPI::BillAPI(std::shared_ptr<Database> database)
{
	this->database = database;
}

std::string BillAPI::extractDirection(std::string& uri)
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

	if (uri[pos] == '/' && pos < uri.length() - 1)
	{
		pos++;
	}

	uri = uri.substr(pos);

	return direction;
}

crow::json::wvalue BillAPI::processRequest(std::string& uri)
{
	crow::json::wvalue res;
	std::string direction = extractDirection(uri);

	if (direction == "order")
	{
		//res = orderAPI.processRequest(direction);
	}
	else if (direction == "bill")
	{
		//res = orderAPI.processRequest(direction);
	}
	else if (direction == "n_client")
	{
		//res = orderAPI.processRequest(direction);
	}

	return res;
}