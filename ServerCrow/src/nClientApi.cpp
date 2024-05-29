#include "..\include\nClientApi.hpp"
#include <regex>

NClientAPI::NClientAPI(std::shared_ptr<std::shared_ptr<Database>> database_ptr)
{
	this->database_ptr = database_ptr;
	this->database = *database_ptr;
}

std::string NClientAPI::extractURISegment(std::string& uri)
{
	std::string direction = "";
	int initial_pos = 0;

	if (uri[0] == '/')
	{
		initial_pos = 1;
	}

	size_t pos = uri.find('/', initial_pos);
	if (pos != std::string::npos)
	{
		pos = uri.find('/', initial_pos);
	}
	else
	{
		pos = uri.length();
	}

	direction = uri.substr(0, pos);

	if (pos < uri.length() - 1 && uri[pos] == '/')
	{
		pos++;
	}

	uri = uri.substr(pos);

	if (uri == "/")
	{
		uri = " ";
	}

	return direction;
}

crow::json::wvalue NClientAPI::buildNClientsJSON(const int& n_clients)
{
	crow::json::wvalue data;
	data["n_clients"] = n_clients;

	return data;
}

crow::json::wvalue NClientAPI::processRequest(std::string& uri)
{
	crow::json::wvalue data;
	std::string mode = extractURISegment(uri);

	std::transform(mode.begin(), mode.end(), mode.begin(), ::toupper); // We make the string equals to the MySQL keywords DATE, WEEK, MONTH or YEAR

	CROW_LOG_INFO << "[NClientAPI] Get number of clients by " << mode;

	if (mode == cts::MYSQL_DAY ||
		mode == cts::MYSQL_WEEK ||
		mode == cts::MYSQL_MONTH ||
		mode == cts::MYSQL_YEAR)
	{
		const std::string& date = extractURISegment(uri);
		data = buildNClientsJSON(database->getNClientsByDate(date, mode));
	}
	else if (mode == "EMPLOYEE")
	{
		const std::string employeeName = extractURISegment(uri);
		data = buildNClientsJSON(database->getNClientsByEmployee(employeeName));

	}
	else if(mode == "")
	{
		data = buildNClientsJSON(database->getNClients());
	}

	return data;
}