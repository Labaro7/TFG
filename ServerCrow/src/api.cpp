#include "..\headers\api.hpp"

struct CustomDeleter
{
	void operator()(int* ptr) const
	{
		std::cout << "Custom deleter: Deleting pointer at address: " << ptr << std::endl;
		delete ptr; // Custom cleanup operation
	}
};

API::API(std::shared_ptr<Database> database)
{
	this->database = database;
	orderAPI = std::make_shared<OrderAPI>(database);
	std::cout << "Const " << (this->database == nullptr) << std::endl;
	billAPI = std::make_shared<BillAPI>(database);
	nClientAPI = std::make_shared<NClientAPI>(database);
}

std::string API::extractDirection(std::string& uri)
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

API::DirectionCode API::getDirectionCode(std::string& uri)
{
	DirectionCode res;
	std::string direction = extractDirection(uri);

	if (direction == "order")
	{
		res = DirectionCode::ORDER;
	}
	else if (direction == "bill")
	{
		res = DirectionCode::BILL;
	}
	else if (direction == "n_client")
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
	CROW_LOG_INFO << "[API] Retrieved request with URI: " << uri;

	crow::json::wvalue res;
	DirectionCode dirCode = getDirectionCode(uri);

	switch (dirCode)
	{
		case DirectionCode::ORDER:
			CROW_LOG_INFO << "[OrderAPI] Processing request with URI: " << uri;
			res = orderAPI->processRequest(uri);

			std::cout << res.dump() << std::endl;
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