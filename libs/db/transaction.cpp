#include <hda/db/transaction.hpp>
#include "transaction_impl.hpp"

namespace hda {
namespace db {

RawResult Transaction::execute(const std::string& query)
{
	return impl_->transaction->exec(query);
}

std::string Transaction::quote(const std::string& rawString) const
{
	return impl_->transaction->quote(rawString);
}

Connection Transaction::commit() &&
{
	//doing stuff
	impl_->transaction->commit();

	//making world consistent
	Transaction moved(std::move(*this));
	//closing existing transaction (already committed)
	moved.impl_->transaction.reset();
	return std::move(moved.impl_->connection);
}

Connection Transaction::rollback() &&
{
	impl_->transaction->abort();

	Transaction moved(std::move(*this));
	//closing existing transaction (already rolled back)
	moved.impl_->transaction.reset();
	return std::move(moved.impl_->connection);
}

DEFINE_MOVABLE_PIMPL(Transaction)

} //namespace db
} //namespace hda
