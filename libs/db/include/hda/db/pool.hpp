#pragma once

#include <hda/db/settings.hpp>
#include <hda/db/connection.hpp>

#include <hda/common/pimpl_utils.hpp>

#include <cstddef>

namespace hda {
namespace db {

/**
 * Implements thread-safe database connection pool
 * with limited maximum size.
 *
 * Provides automatic Connection/Transaction resource management
 *
 * Connections are lazy initialized if needed, so it's (probably)
 * safe to pass relatively huge maxSize
 */
class Pool
{
public:
	Pool(const Settings& settings, size_t maxSize);

	//Pool must stay alive for the whole Connection lifetime
	Connection getConnection();

	/*
	 * Mainly for testing purposes
	 */
	size_t currentSize() const;
	size_t freeConnectionsSize() const;

private:
	//emplace will be used by ~Connection()
	//for returning self into the pool
	friend class Connection;
	void emplace(Connection conn);

	DECLARE_MOVABLE_PIMPL(Pool);
};

} //namespace db
} //namespace hda
