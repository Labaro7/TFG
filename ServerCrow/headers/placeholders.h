#pragma once
#ifndef _PLACEHOLDERS_HPP_
#define _PLACEHOLDERS_HPP_

#include "..\headers\server.h"
#include <string>


// These functions insert data from the DB into the HTML placeholders.


// index.html
std::string insertDataInPlaceHolders(std::ifstream* file, const std::string tablesPricesPlaceholder, Server& server);

// table.html
std::string insertDataInPlaceHolders(std::ifstream* file, const std::string tableNumberPlaceholder, const int n_table, const std::vector<Product> products, Server& server);

#endif