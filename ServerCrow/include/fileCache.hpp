#pragma once
#ifndef _FILE_CACHE_HPP_
#define _FILE_CACHE_HPP_

#include <string>
#include <shared_mutex>
#include "placeholders.hpp"
#include "constants.hpp"
#include "server.hpp"

class Server;

class FileCache
{
public:
	FileCache();
	~FileCache();

	std::string readIndexFile();
	void writeIndexFile();

	std::string readAdminFile();
	void writeAdminFile();

private:
	std::string indexCache;
	std::shared_ptr<std::ifstream> indexFile;

	std::shared_ptr<std::ifstream> adminFile;
	std::string adminCache;

	std::shared_mutex rwMutex;
};

#endif