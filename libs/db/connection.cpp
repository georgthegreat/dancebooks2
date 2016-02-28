#include <hda/db/connection.hpp>
#include <hda/db/transaction.hpp>
#include <hda/db/pool.hpp>
#include "connection_impl.hpp"
#include "transaction_impl.hpp"

namespace hda {
namespace db {

/**
 * Connection requires custom destructor
 * returning current connection into pool (instead of actual descruction)
 *
 * Unable to use DEFINE_MOVABLE_PIMPL macro
 */
Connection::Connection(Connection&&) noexcept = default;
Connection& Connection::operator=(Connection&&) noexcept = default;
Connection::Connection(std::unique_ptr<Impl> impl) noexcept :
	impl_(std::move(impl))
{
}

Connection::~Connection()
{
	if (static_cast<bool>(impl_))
	{
		Pool& pool = impl_->pool;
		pool.emplace(std::move(*this));
	}
}

Transaction Connection::makeReadOnlyTransaction() &&
{
	return common::PimplFactory<Transaction>::create(
		std::move(*this),
		RawTransactionHolder(new pqxx::read_transaction(*(impl_->connection)))
	);
}

Transaction Connection::makeWriteableTransaction() &&
{
	return common::PimplFactory<Transaction>::create(
		std::move(*this),
		RawTransactionHolder(new pqxx::work(*(impl_->connection)))
	);
}

} //namespace db
} //namespace hda
