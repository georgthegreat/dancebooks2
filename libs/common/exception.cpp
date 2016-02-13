#include <hda/common/exception.hpp>
#include <hda/common/pimpl_utils.hpp>

namespace hda {
namespace common {

struct Exception::Impl
{
	std::string what;
};

Exception::Exception() :
	impl_(new Impl{})
{
}

Exception::Exception(std::string what) :
	impl_(new Impl{std::move(what)})
{
}

const char*
Exception::what() const noexcept
{
	return impl_->what.c_str();
}

const std::string&
Exception::message() const
{
	return impl_->what;
}

void
Exception::append(const std::string& what)
{
	impl_->what += what;
}

std::ostream& operator<< (std::ostream& stream, const Exception& ex)
{
    stream << ex.message();
    return stream;
}

DEFINE_COPYABLE_PIMPL(Exception);

} //namespace common
} //namespace hda
