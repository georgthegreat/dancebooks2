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
	using Exception::Exception;
};

class AssertionError: public Exception
{
	using Exception::Exception;
};

#define __STR(X) #X

#define __REQUIRE_3(expression, exObj, message) \
	{ \
		bool val = static_cast<bool>(expression); \
		if (not val) \
		{ \
			throw exObj << message; \
		} \
	}

#define __REQUIRE_2(expression, message) \
	__REQUIRE_3(expression, LogicError(), message)


#define __GET_REQUIRE(_1,_2,_3,WHAT,...) WHAT

#define REQUIRE(...) \
	__GET_REQUIRE(__VA_ARGS__,__REQUIRE_3,__REQUIRE_2)(__VA_ARGS__) \

#define ASSERT(expression) \
	__REQUIRE_3(expression, AssertionError(), "Assertion failed: " << __STR(expression))

} //namespace common
} //namespace hda
