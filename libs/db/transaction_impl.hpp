#pragma once

#include <hda/db/fwd.hpp>
#include <hda/db/connection.hpp>

#include <pqxx/transaction>

namespace hda {
namespace db {

struct Transaction::Impl
{
	Connection connection;
	RawTransactionHolder transaction;
};

} //namespace db
} //namespace hda
