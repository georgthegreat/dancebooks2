#pragma once

#include <hda/db/fwd.hpp>

#include <hda/common/pimpl_utils.hpp>

namespace hda {
namespace db {

class Transaction
{
public:
	RawResult execute(const std::string& query);
	std::string quote(const std::string& rawString) const;

	Connection commit() &&;
	Connection rollback() &&;

private:
	DECLARE_MOVABLE_PIMPL(Transaction);
};

} //namespace db
} //namespace hda
