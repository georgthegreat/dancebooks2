#include <hda/db/settings.hpp>

#include <hda/common/pimpl_utils.hpp>

#include <cstdint>
#include <sstream>
#include <string>

namespace hda {
namespace db {

namespace {

const std::string DEFAULT_HOST = "localhost";
const uint16_t DEFAULT_PORT = 5432;

} //anonymous namespace

struct Settings::Impl
{
	std::string host = DEFAULT_HOST;
	uint16_t port = DEFAULT_PORT;
	std::string user;
	std::string password;
	std::string databaseName;
};

Settings::Settings() :
	impl_(new Impl{})
{
}

std::string Settings::connectionString() const
{
	std::ostringstream ss;
	ss << "host" << "=" << impl_->host << " ";
	ss << "port" << "=" << impl_->port << " ";
	if (not impl_->user.empty()) {
		ss << "user" << "=" << impl_->user << " ";
	}
	if (not impl_->password.empty()) {
		ss << "password" << "=" << impl_->password << " ";
	}
	ss << "dbname" << "=" << impl_->databaseName << " ";

	return ss.str();
}

const std::string& Settings::host() const
{
	return impl_->host;
}

uint16_t Settings::port() const
{
	return impl_->port;
}

const std::string& Settings::user() const
{
	return impl_->user;
}

const std::string& Settings::password() const
{
	return impl_->password;
}

const std::string& Settings::databaseName() const
{
	return impl_->databaseName;
}

Settings& Settings::setHost(std::string host)
{
	impl_->host = std::move(host);
	return *this;
}

Settings& Settings::setPort(uint16_t port)
{
	impl_->port = port;
	return *this;
}

Settings& Settings::setUser(std::string user)
{
	impl_->user = std::move(user);
	return *this;
}

Settings& Settings::setPassword(std::string password)
{
	impl_->password = std::move(password);
	return *this;
}

Settings& Settings::setDatabaseName(std::string databaseName)
{
	impl_->databaseName = std::move(databaseName);
	return *this;
}

DEFINE_COPYABLE_PIMPL(Settings);

} //namespace db
} //namespace hda
