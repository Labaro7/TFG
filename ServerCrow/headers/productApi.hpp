#pragma once
#ifndef _PRODUCT_API_HPP_
#define _PRODUCT_API_HPP_

#include "crow_all.hpp"
#include "apiRoute.hpp"
#include <string>

class ProductAPI : public ApiRoute
{
public:
	ProductAPI(std::shared_ptr<std::shared_ptr<Database>> database_ptr);

	std::string extractURISegment(std::string& uri) override;

	crow::json::wvalue buildOrderedProductsJSON(const std::unordered_map<int, OrderedProduct>& products);

	crow::json::wvalue buildOrderedProductsJSON(const Table& table);

	crow::json::wvalue processRequest(std::string& uri) override;
};

#endif