#include "..\include\billApi.hpp"
#include <regex>

BillAPI::BillAPI(std::shared_ptr<std::shared_ptr<Database>> database_ptr)
{
	this->database_ptr = database_ptr;
	this->database = *database_ptr;
}

std::string BillAPI::extractURISegment(std::string& uri)
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

crow::json::wvalue BillAPI::processRequest(Conn& conn, std::string& uri)
{
	crow::json::wvalue data;

	std::string mode = extractURISegment(uri);
	std::transform(mode.begin(), mode.end(), mode.begin(), ::toupper); // We make the string equals to the MySQL keywords DATE, WEEK, MONTH or YEAR

	CROW_LOG_INFO << "[BillAPI] Get bills by " << mode;

	if (mode == cts::MYSQL_DAY ||
		mode == cts::MYSQL_WEEK ||
		mode == cts::MYSQL_MONTH ||
		mode == cts::MYSQL_YEAR)
	{
		const std::string& date = extractURISegment(uri);
		data = buildBillsJSON(database->getBillsAndPaidsByDate(conn, date, mode));
	}
	else if (mode == "EMPLOYEE")
	{
		const std::string employeeName = extractURISegment(uri);
		data = buildBillsJSON(database->getBillsAndPaidsByEmployee(conn, employeeName));
	}
	else if (mode == "")
	{
		data = buildBillsJSON(database->getBillsAndPaids(conn));
	}

	return data;
}