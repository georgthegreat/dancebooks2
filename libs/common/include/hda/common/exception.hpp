#pragma once

#include <hda/common/pimpl_utils.hpp>

#include <boost/lexical_cast.hpp>

#include <exception>
#include <ostream>
#include <string>
#include <type_traits>
#include <utility>

namespace hda {
namespace common {

class Exception: public std::exception
{
public:
	Exception();
	Exception(std::string what);

	virtual const char* what() const noexcept override;
	const std::string& message() const;

	void append(const std::string& what);
private:
	DECLARE_COPYABLE_PIMPL(Exception)
};

template<typename Ex, typename T>
typename std::enable_if<
	std::is_base_of<Exception, Ex>::value,
	Ex
>::type operator<< (Ex ex, const T& t)
{
	ex.append(boost::lexical_cast<std::string>(t));
	return ex;
}

std::ostream& operator<< (std::ostream& stream, const Exception& ex);

class LogicError: public Exception
{
public:
	using Exception::Exception;
};

class AssertionError: public Exception
{
public:
	AssertionError(const std::string& what)
		: Exception("Assertion failed: " + what)
	{
	}
};

#define __STR(X) #X

#define REQUIRE(expression, message) \
	if (not (expression)) \
	{ \
		throw LogicError() << message; \
	}

#define ASSERT(expression) \
	if (not (expression)) \
	{ \
		throw AssertionError(__STR(expression)); \
	}

} //namespace common
} //namespace hda
