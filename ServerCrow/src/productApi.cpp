#include "..\headers\productApi.hpp"

ProductAPI::ProductAPI(std::shared_ptr<Database> database)
{
	this->database = database;
}

std::string ProductAPI::extractURISegment(std::string& uri)
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

crow::json::wvalue ProductAPI::buildOrderedProductsJSON(const std::unordered_map<int, OrderedProduct>& products)
{
	crow::json::wvalue::list data;

	// Sort by most sold

	for (const auto& product : products)
	{
		crow::json::wvalue products_json;
		products_json["id"] = product.second.id;
		products_json["name"] = product.second.name;
		products_json["page"] = product.second.page;
		products_json["menu"] = product.second.menu;
		products_json["price"] = product.second.price;
		products_json["sold"] = product.second.sold;
		products_json["percent"] = product.second.percent;
		products_json["revenue"] = product.second.revenue;
		products_json["totalRevenue"] = product.second.totalRevenue;

		data.push_back(products_json);
	}

	return data;
}

bool isNumeric(const std::string& str)
{
	for (char c : str)
	{
		if (!std::isdigit(c))
		{
			return false;
		}
	}
	return true;
}

crow::json::wvalue ProductAPI::processRequest(std::string& uri)
{
	crow::json::wvalue data;

	std::string mode = extractURISegment(uri);

	std::transform(mode.begin(), mode.end(), mode.begin(), ::toupper); // We make the string equals to the MySQL keywords DATE, WEEK, MONTH or YEAR

	CROW_LOG_INFO << "[ProductAPI] Get products by " << mode << " with " << uri;

	if (mode == MYSQL_DAY ||
		mode == MYSQL_WEEK ||
		mode == MYSQL_MONTH ||
		mode == MYSQL_YEAR)
	{
		const std::string& date = extractURISegment(uri);
		const std::unordered_map<int, OrderedProduct>& products = database->getOrderedProductsByDate(date, mode);

		if (!products.empty())
		{
			data = buildOrderedProductsJSON(products);
		}
	}
	else if (mode == "ID")
	{
		if (isNumeric(uri))
		{
			const int product_id = std::stoi(extractURISegment(uri));
			OrderedProduct product = database->getOrderedProductById(product_id);
			std::unordered_map<int, OrderedProduct> products;

			if (!product.isEmpty())
			{
				products[product.id] = product;
				data = buildOrderedProductsJSON(products);
			}
		}
	}
	else if (mode == "NAME")
	{
		const std::string product_name = extractURISegment(uri);
		OrderedProduct product = database->getOrderedProductByName(product_name);
		std::unordered_map<int, OrderedProduct> products;

		if (!product.isEmpty())
		{
			products[product.id] = product;
			data = buildOrderedProductsJSON(products);
		}
	}
	else if (mode == "PRICE")
	{
		if (isNumeric(uri))
		{
			const double price = std::stod(extractURISegment(uri));
			std::unordered_map<int, OrderedProduct> products = database->getOrderedProductsByPrice(price);

			if (!products.empty())
			{
				data = buildOrderedProductsJSON(products);	
			}
		}
	}
	else if (mode == "")
	{
		data = buildOrderedProductsJSON(database->getOrderedProducts());
	}

	return data;
}
