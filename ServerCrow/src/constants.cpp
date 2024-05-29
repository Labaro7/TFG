#include "..\headers\constants.hpp"

// Crow constants
const std::string SERVER_IP = "192.168.1.66";
constexpr int SERVER_PORT = 18080;
const std::string SERVER_NAME = "DINET";
const std::string CRT_FILE_PATH = "C:\\Users\\User\\Desktop\\TFG\\ServerCrow\\ServerCrow\\ssl\\server.crt"; // TODO: Put relative path
const std::string KEY_FILE_PATH = "C:\\Users\\User\\Desktop\\TFG\\ServerCrow\\ServerCrow\\ssl\\server.key"; // TODO: Put relative path
const bool AUTH_NEEDED = false;

// Routes
// TODO: Change to relative paths.
extern const std::string LOGIN_HTML_FILE_PATH = "C:\\Users\\User\\Desktop\\TFG\\ServerCrow\\ServerCrow\\templates\\login.html";
extern const std::string INDEX_HTML_FILE_PATH = "C:\\Users\\User\\Desktop\\TFG\\ServerCrow\\ServerCrow\\templates\\index.html";
extern const std::string TABLE_HTML_FILE_PATH = "C:\\Users\\User\\Desktop\\TFG\\ServerCrow\\ServerCrow\\templates\\table.html";
extern const std::string ADMIN_HTML_FILE_PATH = "C:\\Users\\User\\Desktop\\TFG\\ServerCrow\\ServerCrow\\templates\\admin.html";

// HTML constants
const char* TABLES_PRICES_PLACEHOLDER = "<!-- PLACEHOLDER: TABLES PRICES -->";
const char* NUMBER_OF_CLIENTS_PLACEHOLDER = "<!-- PLACEHOLDER: NUMBER OF CLIENTS -->";
const char* TABLE_NUMBER_PLACEHOLDER = "<!-- PLACEHOLDER: TABLE NUMBER -->";
const char* PRODUCT_LIST_PLACEHOLDER = "<!-- PLACEHOLDER: LIST OF PRODUCTS -->";
const char* TICKET_PRODUCTS_PLACEHOLDER = "<!-- PLACEHOLDER: TICKET PRODUCTS -->";
const char* TICKET_BILL_PLACEHOLDER = "<!-- PLACEHOLDER: TABLE BILL -->";
const char* PAGES_BUTTONS_PLACEHOLDER = "<!-- PLACEHOLDER: PRODUCTS PAGES BUTTONS -->";
const char* BRAND_IMAGE_URL_PLACEHOLDER = "<!-- PLACEHOLDER: BRAND IMAGE URL -->";
const char* BRAND_IMAGE_URL = "";
const char* BRAND_IMAGE = ("https://" + SERVER_IP + ":" + std::to_string(SERVER_PORT) + BRAND_IMAGE_URL).c_str();
int N_FOURTH_ROW_BUTTONS = 6;

const char* CURRENT_PRODUCTS_PLACEHOLDER = "<!-- PLACEHOLDER: CURRENT PRODUCTS -->";
const char* CURRENT_EMPLOYEES_PLACEHOLDER = "<!-- PLACEHOLDER: CURRENT EMPLOYEES -->";
const char* CURRENT_INGREDIENTS_PLACEHOLDER = "<!-- PLACEHOLDER: CURRENT INGREDIENTS -->";
const char* CURRENT_ALLERGENS_PLACEHOLDER = "<!-- PLACEHOLDER: CURRENT ALLERGENS -->";

const std::string EUR = "EUR";
const std::string EUR_SYMBOL = "&#8364";
const std::string USD = "USD";
const std::string USD_SYMBOL = "&#36";
const std::string GBP = "GBP";
const std::string GBP_SYMBOL = "&#163";
const std::string JPY = "JPY";
const std::string JPY_SYMBOL = "&#165";

const std::string SPACE_SYMBOL = "&nbsp";
std::string CURRENCY = EUR;
std::string CURRENCY_SYMBOL = SPACE_SYMBOL + EUR_SYMBOL;

// MySQL constants
const std::string HOST = "127.0.0.1";
const int PORT = 3306;
const std::string USERNAME = "user";
const std::string PASSWORD = "user";
const std::string DATABASE_NAME = "example_db";
const std::string MYSQL_DAY = "DATE";
const std::string MYSQL_WEEK = "WEEK";
const std::string MYSQL_MONTH = "MONTH";
const std::string MYSQL_YEAR = "YEAR";

// Other constants
const std::string ASCII_CHARACTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_-+=<>";
const int HASH_LENGTH = 32;
const std::string SESSION_TOKEN_NAME = "session_token";
const int SESSION_TOKEN_LENGTH = 32;