#pragma once

#include <hda/common/pimpl_utils.hpp>

#include <boost/lexical_cast.hpp>

#include <exception>
#include <ostream>
#include <string>
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

	template<typename T>
	Exception& operator<< (const T& t)
	{
		append(boost::lexical_cast<std::string>(t));
		return *this;
	}

private:
	void append(const std::string& what);
	DECLARE_COPYABLE_PIMPL(Exception)
};

std::ostream& operator<< (std::ostream& stream, const Exception& ex)
{
	stream << ex.message();
	return stream;
}

class LogicError: public Exception
{
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
