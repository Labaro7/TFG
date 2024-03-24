#include "..\headers\orderApi.hpp"
#include <regex>

OrderAPI::OrderAPI(std::shared_ptr<Database> database)
{
	this->database = database;
}

std::string OrderAPI::extractURISegment(std::string& uri)
{
	std::cout << "uri " << uri << std::endl;
	std::string direction;
	int initial_pos = 0;

	if (uri[0] == '/')
	{
		initial_pos = 1;
		std::cout << "b " << std::endl;
	}

	size_t pos = uri.find('/', initial_pos);
	std::cout << "abdula1 " << pos << std::endl;
	if (pos != std::string::npos)
	{
		pos = uri.find('/', initial_pos);
		std::cout << "abdula2 " << pos << std::endl;
	}
	else
	{
		pos = uri.length();
	}

	direction = uri.substr(0, pos);
	std::cout << "abdula " << pos << " " << uri.length() << std::endl;

	if (pos < uri.length() - 1 && uri[pos] == '/')
	{
		pos++;
	}

	uri = uri.substr(pos);

	return direction;
}

crow::json::wvalue OrderAPI::buildOrdersJSON(std::vector<Order> orders)
{
	crow::json::wvalue::list data;

	for (const auto& order : orders)
	{
		crow::json::wvalue order_json;
		order_json["n_table"] = order.n_table;
		order_json["n_clients"] = order.n_clients;
		order_json["bill"] = order.bill;
		order_json["paid"] = order.paid;
		order_json["discount"] = order.discount;
		order_json["method"] = order.method;
		order_json["employee"] = order.employee;
		order_json["date"] = order.date;

		crow::json::wvalue::list orderProducts_json;
		for (const auto& product : order.products)
		{
			crow::json::wvalue orderProduct_json;
			orderProduct_json["name"] = product.first.name;
			orderProduct_json["price"] = product.first.price;
			orderProduct_json["amount"] = product.second;

			orderProducts_json.push_back(orderProduct_json);
		}
		order_json["products"] = std::move(orderProducts_json);

		data.push_back(order_json);
	}

	return data;
}

crow::json::wvalue OrderAPI::processRequest(std::string& uri)
{
	crow::json::wvalue res;

	std::string mode = extractURISegment(uri);
	std::transform(mode.begin(), mode.end(), mode.begin(), ::toupper); // We make the string equals to the MySQL keywords DATE, WEEK, MONTH or YEAR

	CROW_LOG_INFO << "[OrderAPI] Get orders by " << mode << " with " << uri;

	if (mode == MYSQL_DAY ||
		mode == MYSQL_WEEK ||
		mode == MYSQL_MONTH ||
		mode == MYSQL_YEAR)
	{
		res = buildOrdersJSON(database->getOrdersByDate(uri, mode));
	}
	else if (mode == "EMPLOYEE")
	{
		const std::string employee = extractURISegment(uri);
		res = buildOrdersJSON(database->getOrdersByEmployee(employee));
	}
	else if (mode == "METHOD")
	{
		const std::string method = extractURISegment(uri);
		res = buildOrdersJSON(database->getOrdersByMethod(method));
	}

	return res;
}
