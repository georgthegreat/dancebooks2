#pragma once

#include <memory>
#include <utility>

namespace hda {
namespace common {
namespace pimpl {

#define DECLARE_PIMPL(classname) \
	public: \
		~classname(); \
	private: \
		class Impl; \
		std::unique_ptr<Impl> impl_; \

#define DECLARE_MOVABLE_PIMPL(classname) \
	DECLARE_PIMPL(classname) \
	public: \
		classname(const classname&) = delete; \
		classname& operator= (const classname&) = delete; \
		classname(classname&&); \
		classname& operator= (classname&&); \

#define DEFINE_PIMPL(classname) \
	classname::~classname() = default;

#define DEFINE_MOVABLE_PIMPL(classname) \
	DEFINE_PIMPL(classname) \
	classname::classname(classname&& toMove) : \
		impl_(std::move(toMove.impl_)) \
	{ \
	} \
	\
	classname& \
	classname::operator= (classname&& toMove) \
	{ \
		impl_ = std::move(toMove.impl_); \
		return *this; \
	} \

#define DECLARE_COPYABLE_PIMPL(classname) \
	DECLARE_PIMPL(classname) \
	public: \
		classname(const classname&); \
		classname& operator= (const classname&); \
		classname(classname&&); \
		classname& operator= (classname&&); \

#define DEFINE_COPYABLE_PIMPL(classname) \
	DEFINE_MOVABLE_PIMPL(classname) \
	classname::classname(const classname& toCopy) : \
		impl_(new Impl(*toCopy.impl_)) \
	{ \
	} \
	\
	classname& \
	classname::operator= (const classname& toCopy)  \
	{ \
		impl_.reset(new Impl(*toCopy.impl_)); \
		return *this; \
	} \

} //namespace pimpl
} //namespace common
} //namespace hda
