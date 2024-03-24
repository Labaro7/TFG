#include "..\headers\nClientApi.hpp"
#include <regex>

NClientAPI::NClientAPI(std::shared_ptr<Database> database)
{
	this->database = database;
}

std::string NClientAPI::extractURISegment(std::string& uri)
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

crow::json::wvalue NClientAPI::processRequest(std::string& uri)
{
	crow::json::wvalue res;
	std::string mode = extractURISegment(uri);

	size_t pos = uri.find('/');
	if (pos != std::string::npos)
	{
		mode = uri.substr(0, pos);
	}


	return res;
}