#include <hda/common/exception.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace hda {
namespace common {
namespace tests {

BOOST_AUTO_TEST_CASE(test_exception)
{
	BOOST_CHECK_THROW(ASSERT(false), AssertionError);
}

} //namespace tests
} //namespace common
} //namespace hda
