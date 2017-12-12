#pragma once

#include <hda/db/fwd.hpp>
#include <hda/db/connection.hpp>

namespace hda {
namespace db {

struct Connection::Impl
{
    Pool& pool;
	//FIXME:
	//pqxx3 isn't aware about move ctors,
	//while pqxx4 breaks my compiler with spurious diagnostics
    RawConnectionHolder connection;
};

} //namespace db
} //namespace hda

