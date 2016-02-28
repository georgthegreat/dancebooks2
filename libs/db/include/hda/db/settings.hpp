#pragma once

#include <hda/common/pimpl_utils.hpp>

#include <cstdint>
#include <string>

namespace hda {
namespace db {

class Settings
{
public:
	Settings();

	//makes connection string acceptable by pqxx::conn constructor
	std::string connectionString() const;

	const std::string& host() const;
	uint16_t port() const;
	const std::string& user() const;
	const std::string& password() const;
	const std::string& databaseName() const;

	Settings& setHost(std::string host);
	Settings& setPort(uint16_t port);
	Settings& setUser(std::string user);
	Settings& setPassword(std::string password);
	Settings& setDatabaseName(std::string databaseName);

private:
	DECLARE_COPYABLE_PIMPL(Settings);
};

} //namespace db
} //namespace hda
