#pragma once
#ifndef _PLACEHOLDERS_HPP_
#define _PLACEHOLDERS_HPP_

#include "..\include\server.hpp"
#include "..\include\database.hpp"
#include <string>

// These functions insert data from the DB into the HTML placeholders.

// index.html
std::string insertDataInPlaceHolders(std::unique_ptr<sql::Connection>& conn,
									 std::ifstream* file,
									 const std::string& tablesPricesPlaceholder,
									 Server& server);

// table.html
std::string insertDataInPlaceHolders(std::unique_ptr<sql::Connection>& conn,
									 std::ifstream* file,
									 const std::string& tableNumberPlaceholder,
									 const int& n_table, 
									 const std::vector<Product>& products, 
									 Server& server);

// add.html
std::string insertDataInPlaceHolders2(std::unique_ptr<sql::Connection>& conn,
									  std::ifstream* file,
									  const std::string& productListPlaceHolder,
									  Server& server);
#endif