#pragma once

#include <hda/db/fwd.hpp>

#include <hda/common/pimpl_utils.hpp>

namespace hda {
namespace db {

class Connection
{
public:
	//no public constructors defined here
	//use Pool for getting connections

	Transaction makeReadOnlyTransaction() &&;
	Transaction makeWriteableTransaction() &&;

private:
	DECLARE_MOVABLE_PIMPL(Connection);
};

} //namespace db
} //namespace hda
