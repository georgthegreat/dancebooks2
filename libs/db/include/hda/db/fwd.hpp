#pragma once

#include <pqxx/connection>
#include <pqxx/transaction>

#include <memory>

namespace hda {
namespace db {

class Pool;
class Connection;
class Transaction;

using RawResult = pqxx::result;

using RawConnection = pqxx::connection;
using RawConnectionHolder = std::unique_ptr<RawConnection>;

using RawTransaction = pqxx::transaction_base;
using RawTransactionHolder = std::unique_ptr<RawTransaction>;

} //namespace db
} //namespace hda
