#include "../include/fileCache.hpp"
#include <sstream>

FileCache::FileCache()
{
	std::cout << "CREATED " << std::endl;
	indexFile = std::make_shared<std::ifstream>(cts::INDEX_HTML_FILE_PATH);
	if (indexFile)
	{
		Server& server = Server::getInstance();
		std::cout << "CREATED2 " << std::endl;
		indexCache = insertDataOnIndex(indexFile, server);
		std::cout << "CREATED3 " << std::endl;
	}
	
	adminFile = std::make_shared<std::ifstream>(cts::ADMIN_HTML_FILE_PATH);
	if (adminFile)
	{
		std::stringstream adminHTML;
		adminHTML << adminFile->rdbuf();
		adminCache = adminHTML.str();
	}

	if (!indexFile->is_open())
	{
		throw std::runtime_error("Failed to open INDEX_HTML_FILE_PATH");
	}
	if (!adminFile->is_open())
	{
		throw std::runtime_error("Failed to open ADMIN_HTML_FILE_PATH");
	}

	indexFile->close();
	adminFile->close();
}

FileCache::~FileCache()
{
	std::cout << "DESTROYED " << std::endl;

}


std::string FileCache::readIndexFile()
{
	std::shared_lock<std::shared_mutex> lock(rwMutex);

	std::cout << "INDEX FILE " << indexFile << std::endl;

	return indexFile ? indexCache : "";
}

void FileCache::writeIndexFile()
{
	std::unique_lock<std::shared_mutex> lock(rwMutex);

	//indexCache = insertDataInPlaceHolders(indexFile, cts::TABLES_PRICES_PLACEHOLDER, Server::getInstance());
}


std::string FileCache::readAdminFile()
{
	std::shared_lock<std::shared_mutex> lock(rwMutex);

	return adminFile ? adminCache : "";
}

void FileCache::writeAdminFile()
{
	std::unique_lock<std::shared_mutex> lock(rwMutex);

	adminCache = insertDataInPlaceHolders2(adminFile, cts::TABLES_PRICES_PLACEHOLDER, Server::getInstance());

}
