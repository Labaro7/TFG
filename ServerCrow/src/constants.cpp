#include "..\headers\constants.h"
#include <string>

// Crow constants
const std::string SERVER_IP = "192.168.1.66";
const int SERVER_PORT = 18080;
const std::string SERVER_NAME = "CrowCpp";
const std::string CRT_FILE_PATH = "C:\\Users\\User\\Desktop\\TFG\\ServerCrow\\ServerCrow\\ssl\\server.crt"; // TODO: Put relative path
const std::string KEY_FILE_PATH = "C:\\Users\\User\\Desktop\\TFG\\ServerCrow\\ServerCrow\\ssl\\server.key"; // TODO: Put relative path

// HTML constants
const char* TABLES_PRICES_PLACEHOLDER = "<!-- PLACEHOLDER: TABLES PRICES -->";
const char* TABLE_NUMBER_PLACEHOLDER = "<!-- PLACEHOLDER: TABLE NUMBER -->"; // It is char* because of strlen()
const char* PRODUCT_LIST_PLACEHOLDER = "<!-- PLACEHOLDER: LIST OF PRODUCTS X -->";
const char* TICKET_PRODUCTS_PLACEHOLDER = "<!-- PLACEHOLDER: TICKET PRODUCTS -->";
const char* TICKET_BILL_PLACEHOLDER = "<!-- PLACEHOLDER: TABLE BILL -->";
const char* FOURTH_ROW_BUTTONS_PLACEHOLDER = "<!-- PLACEHOLDER: FOURTH ROW BUTTONS -->";
int N_FOURTH_ROW_BUTTONS = 5;

const char* CURRENT_PRODUCTS_PLACEHOLDER = "<!-- PLACEHOLDER: CURRENT PRODUCTS -->";
const char* CURRENT_EMPLOYEES_PLACEHOLDER = "<!-- PLACEHOLDER: CURRENT EMPLOYEES -->";
const char* CURRENT_INGREDIENTS_PLACEHOLDER = "<!-- PLACEHOLDER: CURRENT INGREDIENTS -->";
const char* CURRENT_ALLERGENS_PLACEHOLDER = "<!-- PLACEHOLDER: CURRENT ALLERGENS -->";

// MySQL constants
const std::string HOST = "127.0.0.1";
const int PORT = 3306;
const std::string USERNAME = "user";
const std::string PASSWORD = "user";
const std::string DATABASE_NAME = "example_db";