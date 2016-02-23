#include <hda/common/pimpl_utils.hpp>
#include <hda/common/exception.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <type_traits>
#include <utility>

namespace hda {
namespace common {
namespace tests {

namespace copyable {

class Probe
{
public:
	Probe(int value);
	int value() const;

private:
	DECLARE_COPYABLE_PIMPL(Probe);
};

struct Probe::Impl
{
	int value;
};

Probe::Probe(int value)
	: impl_(new Impl{value})
{
}

int Probe::value() const
{
	ASSERT(impl_);
	return impl_->value;
}

DEFINE_COPYABLE_PIMPL(Probe);

} //namespace copyable

namespace movable {
class Probe
{
public:
    Probe(int value);
    int value() const;

	//second value may only be set by PimplFactory
	int secondValue() const;

private:
    DECLARE_MOVABLE_PIMPL(Probe);
};

struct Probe::Impl
{
    int value;
	int secondValue;
};

Probe::Probe(int value)
    : impl_(new Impl{value, 112263})
{
}

int Probe::value() const
{
    ASSERT(impl_);
    return impl_->value;
}

int Probe::secondValue() const
{
	ASSERT(impl_);
	return impl_->secondValue;
}

DEFINE_MOVABLE_PIMPL(Probe);

} //namespace movable

BOOST_AUTO_TEST_CASE(test_copyable)
{
	BOOST_CHECK(std::is_copy_constructible<copyable::Probe>::value);
	BOOST_CHECK(std::is_nothrow_move_constructible<copyable::Probe>::value);
	BOOST_CHECK(std::is_copy_assignable<copyable::Probe>::value);
	BOOST_CHECK(std::is_nothrow_move_assignable<copyable::Probe>::value);

	copyable::Probe firstProbe(31337);
	BOOST_CHECK_EQUAL(firstProbe.value(), 31337);

	copyable::Probe secondProbe(firstProbe);
	BOOST_CHECK_EQUAL(firstProbe.value(), 31337);
	BOOST_CHECK_EQUAL(secondProbe.value(), 31337);

	copyable::Probe thirdProbe = std::move(firstProbe);
	BOOST_CHECK_EQUAL(thirdProbe.value(), 31337);
	BOOST_CHECK_THROW(firstProbe.value(), AssertionError);
}

BOOST_AUTO_TEST_CASE(test_movable)
{
	BOOST_CHECK(not std::is_copy_constructible<movable::Probe>::value);
	BOOST_CHECK(std::is_nothrow_move_constructible<movable::Probe>::value);
	BOOST_CHECK(not std::is_copy_assignable<movable::Probe>::value);
	BOOST_CHECK(std::is_nothrow_move_assignable<movable::Probe>::value);

	movable::Probe firstProbe(31337);
	BOOST_CHECK_EQUAL(firstProbe.value(), 31337);

	movable::Probe secondProbe(std::move(firstProbe));
	BOOST_CHECK_EQUAL(secondProbe.value(), 31337);
	BOOST_CHECK_THROW(firstProbe.value(), AssertionError);
}

BOOST_AUTO_TEST_CASE(test_pimpl_factory)
{
	auto factored = PimplFactory<movable::Probe>::create(100500, 555555);
	BOOST_CHECK_EQUAL(factored.value(), 100500);
	BOOST_CHECK_EQUAL(factored.secondValue(), 555555);
}

} //namespace tests
} //namespace common
} //namespace hda
