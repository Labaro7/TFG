#pragma once
#ifndef _CONSTANTS_HPP_
#define _CONSTANTS_HPP_

#include <string>

// Crow constants
extern const std::string SERVER_IP;
extern const int SERVER_PORT;
extern const std::string SERVER_NAME;
extern const std::string CRT_FILE_PATH;
extern const std::string KEY_FILE_PATH;
extern const bool AUTH_NEEDED;
extern const std::string SESSION_TOKEN_NAME;
const std::string SERVER_PORT_STRING = std::to_string(SERVER_PORT);

// Routes
const std::string SERVER_DOMAIN = "https://" + SERVER_IP + ":" + SERVER_PORT_STRING + "/";
const std::string TABLE_NUMBER_HREF = SERVER_DOMAIN + "table?tableInput=";
extern const std::string LOGIN_HTML_FILE_PATH;
extern const std::string INDEX_HTML_FILE_PATH;
extern const std::string TABLE_HTML_FILE_PATH;
extern const std::string ADD_HTML_FILE_PATH;

// HTML constants
extern const char* TABLES_PRICES_PLACEHOLDER;
extern const char* TABLE_NUMBER_PLACEHOLDER;
extern const char* PRODUCT_LIST_PLACEHOLDER;
extern const char* TICKET_PRODUCTS_PLACEHOLDER;
extern const char* TICKET_BILL_PLACEHOLDER;
extern const char* PAGES_BUTTONS_PLACEHOLDER;
extern int N_FOURTH_ROW_BUTTONS;

extern const char* CURRENT_PRODUCTS_PLACEHOLDER;
extern const char* CURRENT_EMPLOYEES_PLACEHOLDER;
extern const char* CURRENT_INGREDIENTS_PLACEHOLDER;
extern const char* CURRENT_ALLERGENS_PLACEHOLDER;

// MySQL constants
extern const std::string HOST;
extern const int PORT;
extern const std::string USERNAME;
extern const std::string PASSWORD;
extern const std::string DATABASE_NAME;

// Other constants
extern const std::string ASCII_CHARACTERS;
extern const int HASH_LENGTH;
extern const int SESSION_TOKEN_LENGTH;

#endif