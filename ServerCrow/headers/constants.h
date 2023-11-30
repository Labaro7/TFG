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

// HTML constants
extern const char* TABLES_PRICES_PLACEHOLDER;
extern const char* TABLE_NUMBER_PLACEHOLDER;
extern const char* TICKET_PRODUCTS_PLACEHOLDER;
extern const char* TICKET_BILL_PLACEHOLDER;
extern const char* FOURTH_ROW_BUTTONS_PLACEHOLDER;
extern int N_FOURTH_ROW_BUTTONS;

// MySQL constants
extern const std::string HOST;
extern const int PORT;
extern const std::string USERNAME;
extern const std::string PASSWORD;
extern const std::string DATABASE_NAME;

#endif