#include "..\headers\billApi.hpp"
#include <regex>

BillAPI::BillAPI(std::shared_ptr<Database> database)
{
	this->database = database;
}

std::string BillAPI::extractURISegment(std::string& uri)
{
	std::string direction;
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

	return direction;
}

crow::json::wvalue BillAPI::buildBillsJSON(std::vector<BillAndPaid> billsAndPaids)
{
	crow::json::wvalue data;
	crow::json::wvalue totalObject;
	double totalBills = 0;
	double totalPaids = 0;
	crow::json::wvalue::list billsAndPaidsObject;

	for (const auto& billAndPaid : billsAndPaids)
	{
		crow::json::wvalue billAndPaidObject;

		billAndPaidObject["bill"] = billAndPaid.bill;
		totalBills += billAndPaid.bill;

		billAndPaidObject["paid"] = billAndPaid.paid;
		totalPaids += billAndPaid.paid;

		billAndPaidObject["date"] = billAndPaid.date;

		billsAndPaidsObject.push_back(billAndPaidObject);
	}

	totalObject["totalBills"] = totalBills;
	totalObject["totalPaids"] = totalPaids;
	data["total"] = std::move(totalObject);
	data["data"] = std::move(billsAndPaidsObject);

	return data;
}

crow::json::wvalue BillAPI::processRequest(std::string& uri)
{
	crow::json::wvalue data;

	std::string mode = extractURISegment(uri);
	std::transform(mode.begin(), mode.end(), mode.begin(), ::toupper); // We make the string equals to the MySQL keywords DATE, WEEK, MONTH or YEAR

	CROW_LOG_INFO << "[BillAPI] Get bills by " << mode;

	if (mode == MYSQL_DAY ||
		mode == MYSQL_WEEK ||
		mode == MYSQL_MONTH ||
		mode == MYSQL_YEAR)
	{
		const std::string& date = extractURISegment(uri);
		data = buildBillsJSON(database->getBillsAndPaidsByDate(date, mode));
	}
	else if (mode == "EMPLOYEE")
	{
		const std::string employeeName = extractURISegment(uri);
		data = buildBillsJSON(database->getBillsAndPaidsByEmployee(employeeName));
	}
	else if (mode == "")
	{
		data = buildBillsJSON(database->getBillsAndPaids());
	}

	return data;
}