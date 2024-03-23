#include "..\headers\orderApi.hpp"

OrderAPI::OrderAPI(std::shared_ptr<Database> database)
{
	this->database = database;
}

std::string OrderAPI::extractDirection(std::string& uri)
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

	std::string direction = extractDirection(uri);

	CROW_LOG_INFO << "[OrderApi] Get orders by " << direction;

	std::replace(uri.begin(), uri.end(), '%', ' ');

	if (direction == "day")
	{
		return buildOrdersJSON(database->getOrdersByDate(uri, MYSQL_DAY));
	}
	else if (direction == "week")
	{
		return buildOrdersJSON(database->getOrdersByDate(uri, MYSQL_WEEK));

	}
	else if (direction == "month")
	{
		return buildOrdersJSON(database->getOrdersByDate(uri, MYSQL_MONTH));

	}
	else if (direction == "year")
	{
		return buildOrdersJSON(database->getOrdersByDate(uri, MYSQL_YEAR));

	}

	return res;
}
