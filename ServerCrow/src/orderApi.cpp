#include "..\headers\orderApi.hpp"
#include <regex>

OrderAPI::OrderAPI(std::shared_ptr<std::shared_ptr<Database>> database_ptr)
{
	this->database_ptr = database_ptr;
	this->database = *database_ptr;
}

std::string OrderAPI::extractURISegment(std::string& uri)
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

crow::json::wvalue OrderAPI::buildOrdersJSON(std::vector<Order> orders)
{
	crow::json::wvalue::list data;

	if (orders.size() > 0)
	{
		std::reverse(orders.begin(), orders.end());

		for (const auto& order : orders)
		{
			crow::json::wvalue order_json;
			order_json["id"] = order.id;
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
	}

	return data;
}

crow::json::wvalue OrderAPI::processRequest(std::string& uri)
{
	crow::json::wvalue data;

	std::string mode = extractURISegment(uri);
	std::transform(mode.begin(), mode.end(), mode.begin(), ::toupper); // We make the string equals to the MySQL keywords DATE, WEEK, MONTH or YEAR

	CROW_LOG_INFO << "[OrderAPI] Get orders by " << mode << " with " << uri;

	if (mode == cts::MYSQL_DAY ||
		mode == cts::MYSQL_WEEK ||
		mode == cts::MYSQL_MONTH ||
		mode == cts::MYSQL_YEAR)
	{
		const std::string& date = extractURISegment(uri);
		data = buildOrdersJSON(database->getOrdersByDate(date, mode));
	}
	else if (mode == "ID")
	{
		int id = 0;
		try
		{
			id = std::stoi(extractURISegment(uri));
		}
		catch (const std::invalid_argument& e)
		{
			CROW_LOG_WARNING << "Invalid argument in std::stoi";
		}

		const Order& order = database->getOrderById(id);

		if (!order.isEmpty())
		{
			data = buildOrdersJSON({ order });
		}
		else
		{
			const std::vector<Order> emptyVector;
			data = buildOrdersJSON(emptyVector);
		}
	}
	else if (mode == "EMPLOYEE")
	{
		const std::string employeeName = extractURISegment(uri);
		data = buildOrdersJSON(database->getOrdersByEmployee(employeeName));
	}
	else if (mode == "N_TABLE")
	{
		std::string n_table = extractURISegment(uri);

		data = buildOrdersJSON(database->getOrdersByNTable(n_table));
	}
	else if (mode == "METHOD")
	{
		const std::string method = extractURISegment(uri);
		data = buildOrdersJSON(database->getOrdersByMethod(method));
	}
	else if (mode == "")
	{
		data = buildOrdersJSON(database->getOrders());
	}

	//std::cout << data.dump() << std::endl;
	return data;
}
