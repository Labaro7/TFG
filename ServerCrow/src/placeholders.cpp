#include "..\include\placeholders.hpp"
#include "..\include\constants.hpp"

// index.html
std::string insertDataInPlaceHolders(std::unique_ptr<sql::Connection>& conn,
									 std::ifstream* file,
									 const std::string& tablesPricesPlaceholder,
									 Server& server)
{
	std::stringstream ssHTML;
	ssHTML << file->rdbuf();
	file->close();
	std::string contentHTML = ssHTML.str();

	std::vector<Table> tables = server.db()->getTables(conn);

	// Make a HTML with the tables prices
	std::ostringstream ss;

	for (const auto& t : tables)
	{
		const std::string last_modified = server.getLastModifiedFromTable(conn, t);
		ss << "<li class='table'><a href='" << cts::TABLE_NUMBER_HREF << t.n_table << "'><div class = 'tableNumber'>Table: " << t.n_table << "<div class = 'lastModified'>" << last_modified << "</div></div><div class='tablePrice'>" << t.bill * (1 - (t.discount / 100)) << cts::CURRENCY_SYMBOL << "</div></a></li>";
	}

	std::string tablesPricesHTML = ss.str();
	ss.str(""); // Important to clear here

	// Insert previous HTML piece with the table number into HTML
	size_t tablesPricesPlaceholderPos = contentHTML.find(cts::TABLES_PRICES_PLACEHOLDER);
	if (tablesPricesPlaceholderPos != std::string::npos)
	{
		contentHTML.replace(tablesPricesPlaceholderPos, strlen(cts::TABLES_PRICES_PLACEHOLDER), tablesPricesHTML);
	}

	return contentHTML;
}

// table.html
std::string insertDataInPlaceHolders(std::unique_ptr<sql::Connection>& conn,
									 std::ifstream* file,
									 const std::string& tableNumberPlaceholder,
									 const int& n_table,
									 const std::vector<Product>& products,
									 Server& server)
{
	// Data to insert into HTML
	// 1. Table number
	// 2. Fourth row buttons
	// 3. Restaurant products
	// 4. Ticket products
	// 5. Ticket bill

	// Read the content of the HTML into contentHTML
	if (!file->is_open())
	{
		return "";
	}

	std::stringstream ssHTML;
	ssHTML << file->rdbuf();
	file->close();
	std::string contentHTML = ssHTML.str();

	std::ostringstream ss;

	// 1. HTML with the number of clients
	Table t = server.getTableByNumber(conn, n_table);
	int n_clients;
	if (t.isEmpty())
	{
		n_clients = 0;
	}
	else
	{
		n_clients = t.n_clients;
	}

	ss << n_clients << std::endl;
	std::string numClientsHTML = ss.str();
	ss.str(""); // Important to clear here

	// 1. Insert previous HTML piece with the number of clients into HTML
	size_t numClientsPlaceholderPos = contentHTML.find(cts::NUMBER_OF_CLIENTS_PLACEHOLDER);
	if (numClientsPlaceholderPos != std::string::npos)
	{
		contentHTML.replace(numClientsPlaceholderPos, strlen(cts::NUMBER_OF_CLIENTS_PLACEHOLDER), numClientsHTML);
	}

	// 1. HTML with the table number
	ss << "<div id='numTable'>Table: " << n_table << "</div>" << std::endl;
	std::string tableNumberHTML = ss.str();
	ss.str("");

	// 1. Insert previous HTML piece with the table number into HTML
	size_t tableNumberPlaceholderPos = contentHTML.find(cts::TABLE_NUMBER_PLACEHOLDER);
	if (tableNumberPlaceholderPos != std::string::npos)
	{
		contentHTML.replace(tableNumberPlaceholderPos, strlen(cts::TABLE_NUMBER_PLACEHOLDER), tableNumberHTML);
	}

	// 2. HTML with fourth row buttons
	for (int i = 1; i <= cts::N_FOURTH_ROW_BUTTONS; i++)
	{
		ss << "<div class='productsPagesButton' id='productsPagesButton" << i << "' data-number='" << i << "' onclick='displayMenu(this)'>" << i << "</div>" << std::endl;
	}

	std::string pagesButtonsHTML = ss.str();
	ss.str(""); // Important to clear here

	// 2. Insert previous HTML piece with the fourth row buttons into HTML
	size_t pagesButtonsPlaceholderPos = contentHTML.find(cts::PAGES_BUTTONS_PLACEHOLDER);
	if (pagesButtonsPlaceholderPos != std::string::npos)
	{
		contentHTML.replace(pagesButtonsPlaceholderPos, strlen(cts::PAGES_BUTTONS_PLACEHOLDER), pagesButtonsHTML);
	}

	// 3. Get the data of products currently added to the database
	std::vector<std::pair<std::string, float>> productsInfo;

	for (const auto p : products)
	{
		productsInfo.push_back({ p.name, p.price });
	}

	// 3. Generate HTML piece with the products of the restaurant
	std::string productListHTML;

	std::vector<page_t> pages = server.getDataFromPages(conn);
	int i = 0;
	for (const auto& page : pages)
	{
		ss << "<ul class='contentGrid' id='grid" << i + 1 << "'>";

		for (const auto& p : page)
		{
			std::string product_name = p.first.name;
			double product_price = p.first.price;
			std::string product_color = p.first.color;
			int product_deployable = p.first.deployable;
			int product_id = server.getProductIdByName(conn, product_name);
			auto list = p.second;

			// Is product
			if (p.first.deployable == 0 && p.second.empty())
			{
				std::vector<Ingredient> ingredients = server.getIngredientsFromProduct(conn, server.getProductByName(conn, product_name));
				std::vector<Allergen> allergens = server.getAllergensFromProduct(conn, server.getProductByName(conn, product_name));

				if (product_name.size() <= 15)
				{
					if (allergens.size() > 0)
					{
						ss << std::fixed << std::setprecision(2) << "<li class ='grid-product' onclick='addProductToTicket(this)' style='background-color:" << product_color << ";'><div class='products-names'>" << product_name << "</div>" << "<div class='products-prices'>" << product_price << "</div><ul class='ingredients>" << std::endl; // We use this because std::to_string() eliminates the precision set
					}
					else
					{
						ss << std::fixed << std::setprecision(2) << "<li class ='grid-product' onclick='addProductToTicket(this)' style='background-color:" << product_color << ";'><div class='products-names'>" << product_name << "</div>" << "<div class='products-prices'>" << product_price << "</div>" << std::endl;
					}

					for (const auto& q : p.second)
					{
						if (q.price)
						{
							ss << std::fixed << std::setprecision(2) << "<li class='deployable-product' data-name='" << product_name << "' style='background-color: " << q.color << "' onclick='selectElement(this)'><div class='products-names'>" << q.name << "</div><div class='products-prices'>" << q.price << "</div><ul class='ingredients'>";

							for (const auto& ingredient : ingredients)
							{
								ss << "<li class='ingredient'>" << ingredient.name << "</li>";
							}

							ss << "</ul><ul class='allergens'>";

							for (const auto& allergen : allergens)
							{
								ss << "<li class='allergen'>" << allergen.name << "</li>";
							}

							ss << "</li>";
						}
					}

					ss << "</ul><div class='allergenAlertOn'></div>";
				}
				else
					ss << std::fixed << std::setprecision(2) << "<li class ='grid-product-small' onclick='addProductToTicket(this)' style='background-color:" << product_color << ";'><div class='products-names'>" << product_name << "</div>" << "<div class='products-prices'>" << product_price << "</div><ul class='ingredients'>";

				for (const auto& q : p.second)
				{
					if (q.price)
					{
						ss << std::fixed << std::setprecision(2) << "<li class='deployable-product' data-name='" << product_name << "' style='background-color: " << q.color << "' onclick='selectElement(this)'><div class='products-names'>" << q.name << "</div><div class='products-prices'>" << q.price << "</div><ul class='ingredients'>";

						for (const auto& ingredient : ingredients)
						{
							ss << "<li class='ingredient'>" << ingredient.name << "</li>";
						}

						ss << "</ul>";
						if (allergens.size() > 0) ss << "<div class = 'allergenAlertOn'></div>";
						ss << "</li>";
					}
				}
			}

			// Is desployable
			else
			{
				if (p.first.deployable == 0 && p.first.price == 0)
				{
					ss << "<li class='grid-deployable' onclick='openDeployable(this)' style='background-color:" << product_color << ";' data-id='" << product_id << "'>" << product_name << "</li>";
				}

				for (const auto& q : p.second)
				{
					if (q.price)
					{
						product_color = q.color;
						product_deployable = q.deployable;
						std::vector<Ingredient> ingredients = server.getIngredientsFromProduct(conn, server.getProductByName(conn, q.name));
						std::vector<Allergen> allergens = server.getAllergensFromProduct(conn, server.getProductByName(conn, q.name));

						if (allergens.size() > 0)
						{
							ss << std::fixed << std::setprecision(2) << "<li class='deployable-product' data-deployable='" << product_id << "' onclick='addProductToTicket(this)' style = 'background-color:" << product_color << ";'><div class = 'products-names'>" << q.name << "</div><div class = 'products-prices'>" << q.price << "</div><ul class='ingredients'>";

						}
						else
						{
							ss << std::fixed << std::setprecision(2) << "<li class='deployable-product' data-deployable='" << product_id << "' onclick='addProductToTicket(this)' style='background-color:" << product_color << ";'><div class = 'products-names'>" << q.name << "</div><div class = 'products-prices'>" << q.price << "</div><ul class='ingredients'>";
						}

						for (const auto& ingredient : ingredients)
						{
							ss << "<li class='ingredient'>" << ingredient.name << "</li>";
						}

						ss << "</ul><ul class='allergens'>";

						for (const auto& allergen : allergens)
						{
							ss << "<li class='allergen'>" << allergen.name << "</li>";
						}

						ss << "</ul>";
						if (allergens.size() > 0) ss << "<div class = 'allergenAlertOn'></div>";
						ss << "</li>";
					}
				}

				ss << "</li>";
			}
		}

		ss << "</ul>";
		i++;
	}

	productListHTML = ss.str();
	ss.str("");

	// 3. Insert HTML piece with the products of the restaurant into HTML
	std::string changedPlaceholder = cts::PRODUCT_LIST_PLACEHOLDER;

	size_t productListPlaceholderPos = contentHTML.find(changedPlaceholder);
	if (productListPlaceholderPos != std::string::npos)
	{
		contentHTML.insert(productListPlaceholderPos, productListHTML);
	}


	// 4. Get ticket products and ticket bill
	const domain::product_unordered_map ticketProducts = server.getTableByNumber(conn, n_table).products;
	double bill = server.getTableByNumber(conn, n_table).bill;
	double discount = server.getTableByNumber(conn, n_table).discount;

	for (const auto& p : ticketProducts)
	{
		ss << std::fixed << std::setprecision(2) <<
			"<li class='ticketProduct' onclick='openModifyDeleteMenu(this)'>" << std::endl << "<div class='times'><div class='productTimes'>x" << p.second << "</div><div class='oldProductTimes'></div></div><div class='container'><div class='productNames'>" << p.first.name << "</div><div class='details'>" << p.first.details << "</div></div><div class='productPrices'>" << p.first.price << "</div><div class='productTotalPrices'>" << p.second * p.first.price << "</div></li>" << std::endl;
	}

	std::string ticketProductsHTML = ss.str();
	ss.str("");

	// 4. Insert previous HTML piece with the ticket of the table into HTML
	size_t ticketProductsPlaceholderPos = contentHTML.find(cts::TICKET_PRODUCTS_PLACEHOLDER);
	if (ticketProductsPlaceholderPos != std::string::npos)
	{
		contentHTML.replace(ticketProductsPlaceholderPos, strlen(cts::TICKET_PRODUCTS_PLACEHOLDER), ticketProductsHTML);
	}

	// 5. Generate HTML piece with the ticket bill
	ss << "<div id='bill'>";
	ss << "<div id='discount'><div id='minus'>-</div><div id='discountValue'>" << discount << "</div><div id='percentage'>%</div></div>";
	ss << "<div id='currency'></div><div id='price'>" << bill * (1 - (discount / 100)) << cts::CURRENCY_SYMBOL << "</div>";
	ss << "</div>";

	std::string ticketBillHTML = ss.str();
	ss.str("");
	size_t ticketBillPlaceholderPos = contentHTML.find(cts::TICKET_BILL_PLACEHOLDER);
	contentHTML.replace(ticketBillPlaceholderPos, strlen(cts::TICKET_BILL_PLACEHOLDER), ticketBillHTML);

	// 5. Insert HTML piece with the image of DINET
	ss << "<img src='" << cts::BRAND_IMAGE_URL << "'";

	std::string brandImageHTML = ss.str();
	ss.str("");

	size_t brandImagePlaceholderPos = contentHTML.find(cts::BRAND_IMAGE_URL_PLACEHOLDER);
	if (brandImagePlaceholderPos != std::string::npos)
	{
		contentHTML.replace(brandImagePlaceholderPos, strlen(cts::BRAND_IMAGE_URL_PLACEHOLDER), ticketProductsHTML);
	}

	return contentHTML;
}

// admin.html
std::string insertDataInPlaceHolders2(std::unique_ptr<sql::Connection>& conn,
									  std::ifstream* file,
									  const std::string& productListPlaceHolder,
									  Server& server)
{
	// Data to insert into HTML
	// 1. Current products
	// 1.1 Pages buttons
	// 1.2 Current product list
	// 1.3 Current employees
	// 1.4 Current products
	// 1.5 Current ingredients
	// 1.6 Current allergens

	// Read the content of the HTML into contentHTML
	if (!file->is_open())
	{
		return "";
	}

	std::stringstream ssHTML;
	ssHTML << file->rdbuf();
	file->close();
	std::string contentHTML = ssHTML.str();

	std::stringstream ss;

	// 1.1. HTML with fourth row buttons
	for (int i = 1; i <= cts::N_FOURTH_ROW_BUTTONS; i++)
	{
		ss << "<div class='productsPagesButton' id='productsPagesButton" << i << "' data-number='" << i << "' onclick='displayMenu(this)'>" << i << "</div>" << std::endl;
	}

	std::string pagesButtonsHTML = ss.str();
	ss.str(""); // Important to clear here

	// 1.1. Insert previous HTML piece with the fourth row buttons into HTML
	size_t fourthRowButtonsPlaceholderPos = contentHTML.find(cts::PAGES_BUTTONS_PLACEHOLDER);
	if (fourthRowButtonsPlaceholderPos != std::string::npos)
	{
		contentHTML.replace(fourthRowButtonsPlaceholderPos, strlen(cts::PAGES_BUTTONS_PLACEHOLDER), pagesButtonsHTML);
	}

	// 1.2. Get the data of products currently added to the database
	std::vector<Product> products = server.getProducts(conn);
	std::vector<std::pair<std::string, float>> productsInfo;

	for (const auto p : products)
	{
		productsInfo.push_back({ p.name, p.price });
	}

	// 1.2. Generate HTML piece with the products of the restaurant
	std::string productListHTML;

	std::vector<page_t> pages = server.getDataFromPages(conn);
	int i = 0;
	for (const auto& page : pages)
	{
		ss << "<ul class='contentGrid' id='grid" << i + 1 << "'>";

		for (const auto& p : page)
		{
			std::string product_name = p.first.name;
			double product_price = p.first.price;
			int product_deployable = p.first.deployable;
			std::string product_color = p.first.color;

			// Is product
			if (p.first.deployable == 0 && p.second.empty())
			{
				std::vector<Ingredient> ingredients = server.getIngredientsFromProduct(conn, server.getProductByName(conn, product_name));
				std::vector<Allergen> allergens = server.getAllergensFromProduct(conn, server.getProductByName(conn, product_name));

				if (product_name.size() <= 15)
				{
					ss << std::fixed << std::setprecision(2) << "<li class ='grid-product'><div class='products-names'>" << product_name << "</div>" << "<div class='products-prices'>" << product_price << "</div><ul class='ingredients'>" << std::endl; // We use this because std::to_string() eliminates the precision set

					for (const auto& ingredient : ingredients)
					{
						ss << "<li class='ingredient'>" << ingredient.name << "</li>";
					}

					ss << "</ul><ul class='allergens'>";

					for (const auto& allergen : allergens)
					{
						ss << "<li class='allergen'>" << allergen.name << "</li>";
					}

					ss << "</ul></li>";
				}
				else
					ss << std::fixed << std::setprecision(2) << "<li class ='grid-product-small''><div class='products-names'>" << product_name << "</div>" << "<div class='products-prices'>" << product_price << "</div></li>" << std::endl;
			}

			// Is desployable
			else
			{
				if (p.first.deployable == 0 && p.first.price == 0)
				{
					int product_id = server.getProductIdByName(conn, product_name);

					ss << "<li class='grid-deployable' data-id='" << product_id << "' onclick = 'openDeployable(this)' style='background-color: " << product_color << "'>" << product_name << "</li>";

					for (const auto& q : p.second)
					{
						if (q.price)
						{
							std::vector<Ingredient> ingredients = server.getIngredientsFromProduct(conn, server.getProductByName(conn, q.name));
							std::vector<Allergen> allergens = server.getAllergensFromProduct(conn, server.getProductByName(conn, q.name));

							ss << std::fixed << std::setprecision(2) << "<li class='deployable-product' data-name='" << product_name << "' style='background-color: " << q.color << "' onclick='selectElement(this)'><div class='products-names'>" << q.name << "</div><div class='products-prices'>" << q.price << "</div><ul class='ingredients'>";

							for (const auto& ingredient : ingredients)
							{
								ss << "<li class='ingredient'>" << ingredient.name << "</li>";
							}

							ss << "</ul><ul class='allergens'>";

							for (const auto& allergen : allergens)
							{
								ss << "<li class='allergen'>" << allergen.name << "</li>";
							}

							ss << "</ul></li>";
						}
					}

					ss << "</li>" << std::endl;
				}
			}
		}

		ss << "</ul>";
		i++;
	}

	productListHTML = ss.str();
	ss.str("");

	// 1.2. Insert HTML piece with the products of the restaurant into HTML
	std::string changedPlaceholder = cts::PRODUCT_LIST_PLACEHOLDER;

	size_t productListPlaceholderPos = contentHTML.find(changedPlaceholder);
	if (productListPlaceholderPos != std::string::npos)
	{
		contentHTML.insert(productListPlaceholderPos, productListHTML);
	}


	// 1.3
	std::string employeesListHTML;

	std::vector<Employee> employees = server.getEmployees(conn);
	std::sort(employees.begin(), employees.end(), [](auto& a, auto& b)
			  {
				  if (a.firstName != b.firstName) return a.firstName < b.firstName;
				  else return a.lastName < b.lastName;

				  return true;
			  });

	i = 0;
	for (const auto& e : employees)
	{
		ss << "<li class='employee' onclick='selectEmployee(this)' data-n='" << i << "'>";
		ss << "<ul>";
		ss << "<li class='employeeName'>" << e.firstName + " " << e.lastName << "</li>";
		ss << "<ul class='employeeDetails' id='" << e.firstName + " " << e.lastName << "'>";
		ss << "<li class='employeeFirstName'>" << e.firstName << "</li>";
		ss << "<li class='employeeLastName'>" << e.lastName << "</li>";
		ss << "<li class='employeeEmail'>" << e.email << "</li>";
		ss << "<li class='employeeId'>" << e.id << "</li>";
		ss << "<li class='employeeMobileNumber'>" << e.mobileNumber << "</li>";
		ss << "<li class='employeeLevel'>" << e.level << "</li>";
		ss << "<li class='employeeUsername'>" << e.username << "</li>";
		ss << "<li class='employeePassword'>" << e.password_hash << "</li>";
		ss << "<li class='employeeSessionToken'>" << e.session_token << "</li></ul></ul></li>";
		i++;
	}

	employeesListHTML = ss.str();
	ss.str("");

	// 1.3. Insert HTML piece with the employees into HTML
	std::string employeesPlaceholder = cts::CURRENT_EMPLOYEES_PLACEHOLDER;

	size_t employeesListPlaceholderPos = contentHTML.find(employeesPlaceholder);
	if (employeesListPlaceholderPos != std::string::npos)
	{
		contentHTML.insert(employeesListPlaceholderPos, employeesListHTML);
	}


	//1.4
	std::string currentProductsHTML;
	std::vector<Ingredient> productIngredients;
	std::vector<Allergen> productAllergens;

	std::sort(products.begin(), products.end(), [](auto& a, auto& b)
			  {
				  return a.name < b.name;
			  });

	for (const auto& product : products)
	{
		if (product.deployable)
		{
			ss << "<ul class='currentProduct' onclick='selectProduct(this)'><ul class='currentProductName'><li class='currProductName'>" << product.name << "</li></ul>";
			ss << "<ul class='currentProductIngredients'>";

			productIngredients = server.getIngredientsFromProduct(conn, product);
			if (!productIngredients.empty())
			{
				for (const auto& ingredient : productIngredients)
				{
					ss << "<li class='currentProductIngredient'>" << ingredient.name << "</li>";
				}
			}

			ss << "</ul>";
			ss << "<ul class='currentProductAllergens'>";

			productAllergens = server.getAllergensFromProduct(conn, product);
			if (!productAllergens.empty())
			{
				for (const auto& allergen : productAllergens)
				{
					ss << "<li class='currentProductAllergen'>" << allergen.name << "</li>";
				}
			}

			ss << "</ul></ul>";
		}
	}

	currentProductsHTML = ss.str();
	ss.str("");

	std::string productsPlaceholder = cts::CURRENT_PRODUCTS_PLACEHOLDER;

	size_t productsListPlaceholderPos = contentHTML.find(productsPlaceholder);
	if (productsListPlaceholderPos != std::string::npos)
	{
		contentHTML.insert(productsListPlaceholderPos, currentProductsHTML);
	}


	// 1.5
	std::string currentIngredientsHTML;
	std::vector<Ingredient> ingredients = server.getIngredients(conn);

	std::sort(ingredients.begin(), ingredients.end(), [](auto& a, auto& b)
			  {
				  return a.name < b.name;
			  });

	for (const auto& ingredient : ingredients)
	{
		ss << "<li class='currentIngredient' onclick='selectIngredient(this)' id='" << ingredient.name << "'>" << ingredient.name << "</li>";
	}

	currentIngredientsHTML = ss.str();
	ss.str("");

	std::string ingredientsPlaceholder = cts::CURRENT_INGREDIENTS_PLACEHOLDER;

	size_t ingredientsListPlaceholderPos = contentHTML.find(ingredientsPlaceholder);
	if (ingredientsListPlaceholderPos != std::string::npos)
	{
		contentHTML.insert(ingredientsListPlaceholderPos, currentIngredientsHTML);
	}


	// 1.6
	std::string currentAllergensHTML;
	std::vector<Allergen> allergens = server.getAllergens(conn);

	std::sort(allergens.begin(), allergens.end(), [](auto& a, auto& b)
			  {
				  return a.name < b.name;
			  });

	for (const auto& allergen : allergens)
	{
		ss << "<li class='currentAllergen' onclick='selectAllergen(this)' id='" << allergen.name << "'>" << allergen.name << "</li>";
	}

	currentAllergensHTML = ss.str();
	ss.str("");

	std::string allergensPlaceholder = cts::CURRENT_ALLERGENS_PLACEHOLDER;

	size_t allergensListPlaceholderPos = contentHTML.find(allergensPlaceholder);
	if (allergensListPlaceholderPos != std::string::npos)
	{
		contentHTML.insert(allergensListPlaceholderPos, currentAllergensHTML);
	}


	return contentHTML;
}