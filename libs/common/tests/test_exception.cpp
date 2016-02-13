#include <hda/common/exception.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <string>

namespace hda {
namespace common {
namespace tests {

BOOST_AUTO_TEST_CASE(test_exception)
{
	auto thrower = []()
	{
		throw Exception("1 ") << 2 << " test";
	};

	BOOST_CHECK_THROW(thrower(), Exception);
}

BOOST_AUTO_TEST_CASE(test_require)
{
	bool exceptionThrown = false;
	{
		try
		{
			REQUIRE(
				2 + 2 == 3,
				"Something went" << " " << "wrong for the " << 2 << " time"
			);
		}
		catch (const LogicError& ex)
		{
			exceptionThrown = true;
			BOOST_CHECK(
				ex.message().find("Something went wrong for the 2 time") !=
				std::string::npos
			);
		}
	}
	BOOST_CHECK(exceptionThrown);
}

BOOST_AUTO_TEST_CASE(test_assertion)
{
	bool exceptionThrown = false;
	{
		try
		{
			ASSERT(false);
		}
		catch (const AssertionError& ex)
		{
			exceptionThrown = true;
			BOOST_CHECK(ex.message().find("false") != std::string::npos);
		}
	}
	BOOST_CHECK(exceptionThrown);
}

} //namespace tests
} //namespace common
} //namespace hda
