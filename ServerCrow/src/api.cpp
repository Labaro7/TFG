#include "..\include\api.hpp"

API::API(std::shared_ptr<Database> database)
{
	this->database = database;
	this->database_ptr = std::make_shared<std::shared_ptr<Database>>(database);

	orderAPI = std::make_shared<OrderAPI>(database_ptr);
	billAPI = std::make_shared<BillAPI>(database_ptr);
	nClientAPI = std::make_shared<NClientAPI>(database_ptr);
	productAPI = std::make_shared<ProductAPI>(database_ptr);
}

std::string API::extractURISegment(std::string& uri)
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

	if (pos < (uri.length() - 1) && uri[pos] == '/')
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
	else if (direction == "products")
	{
		res = DirectionCode::PRODUCTS;
	}
	else
	{
		res = DirectionCode::ERR;
	}

	return res;
}

crow::json::wvalue API::retrieveRequest(Conn& conn, std::string& uri)
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
				res = orderAPI->processRequest(conn, uri);

				break;

			case DirectionCode::BILL:
				CROW_LOG_INFO << "[BillAPI] Processing request with URI: " << uri;
				res = billAPI->processRequest(conn, uri);

				break;

			case DirectionCode::NCLIENT:
				CROW_LOG_INFO << "[NClientAPI] Processing request with URI: " << uri;
				res = nClientAPI->processRequest(conn, uri);

				break;

			case DirectionCode::PRODUCTS:
				CROW_LOG_INFO << "[ProductAPI] Processing request with URI: " << uri;
				res = productAPI->processRequest(conn, uri);

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

void API::setDatabase(std::shared_ptr<std::shared_ptr<Database>> database_ptr)
{
	this->database_ptr = database_ptr;
	this->database = *database_ptr;
}
