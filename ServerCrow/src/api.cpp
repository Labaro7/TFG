#include "..\headers\api.hpp"

API::API(std::shared_ptr<Database> database)
{
	this->database = database;
	orderAPI = std::make_shared<OrderAPI>(database);
	billAPI = std::make_shared<BillAPI>(database);
	nClientAPI = std::make_shared<NClientAPI>(database);
}

std::string API::extractURISegment(std::string& uri)
{
	std::string direction;
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

	if (pos < (uri.length() - 1) && uri[pos] == '/')
	{
		pos++;
	}

	uri = uri.substr(pos);

	return direction;
}

void API::decodeURI(std::string& uri)
{
	std::stringstream decoded;
	for (size_t i = 0; i < uri.length(); ++i)
	{
		if (uri[i] == '%' && i + 2 < uri.length())
		{
			int hex;
			sscanf_s(uri.substr(i + 1, 2).c_str(), "%x", &hex);
			decoded << static_cast<char>(hex);
			i += 2;
		}
		else if (uri[i] == '+')
		{
			decoded << ' ';
		}
		else
		{
			decoded << uri[i];
		}
	}

	uri = decoded.str();
}

API::DirectionCode API::getDirectionCode(std::string& uri)
{
	DirectionCode res;

	decodeURI(uri);
	std::string direction = extractURISegment(uri);

	if (direction == "orders")
	{
		res = DirectionCode::ORDER;
	}
	else if (direction == "bills")
	{
		res = DirectionCode::BILL;
	}
	else if (direction == "n_clients")
	{
		res = DirectionCode::NCLIENT;
	}
	else
	{
		res = DirectionCode::ERR;
	}

	return res;
}

crow::json::wvalue API::retrieveRequest(std::string& uri)
{
	try
	{
		CROW_LOG_INFO << "[API] Retrieved request with URI: " << uri;

		crow::json::wvalue res;
		DirectionCode dirCode = getDirectionCode(uri);

		switch (dirCode)
		{
			case DirectionCode::ORDER:
				CROW_LOG_INFO << "[OrderAPI] Processing request with URI: " << uri;
				res = orderAPI->processRequest(uri);

				break;

			case DirectionCode::BILL:
				CROW_LOG_INFO << "[BillAPI] Processing request with URI: " << uri;
				res = billAPI->processRequest(uri);

				break;

			case DirectionCode::NCLIENT:
				CROW_LOG_INFO << "[NClientAPI] Processing request with URI: " << uri;
				res = nClientAPI->processRequest(uri);

				break;

			default:
				CROW_LOG_WARNING << "[API] Not a valid URI for the API";
				return res;

				break;
		}

		CROW_LOG_INFO << "[API] Returning response";

		return res;
	}
	catch (const std::exception& e)
	{
		CROW_LOG_WARNING << "[EXCEPTION] Could not get retrieve request by API. Error message: " << e.what();
	}
}