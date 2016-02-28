#include <hda/db/pool.hpp>
#include <hda/db/connection.hpp>
#include <hda/db/settings.hpp>
#include "connection_impl.hpp"

#include <cstddef>
#include <condition_variable>
#include <deque>
#include <mutex>

namespace hda {
namespace db {

struct Pool::Impl
{
	Impl(const Settings& settings_, size_t maxSize_) :
		connectionString(settings_.connectionString()),
		maxSize(maxSize_)
	{
	}

	const std::string connectionString;
	size_t currentSize = 0;
	const size_t maxSize;

	std::deque<Connection> freeConnections;

	std::mutex mutex;
	size_t waitersCount = 0;
	std::condition_variable cv;
};

Pool::Pool(const Settings& settings, size_t maxSize) :
	impl_(new Impl(settings, maxSize))
{
}

size_t Pool::currentSize() const
{
	std::unique_lock<std::mutex> lock(impl_->mutex);
	return impl_->currentSize;
}

size_t Pool::freeConnectionsSize() const
{
	std::unique_lock<std::mutex> lock(impl_->mutex);
	return impl_->freeConnections.size();
}

Connection Pool::getConnection()
{
	auto hasFreeConnection = [&]() -> bool
	{
		return (not impl_->freeConnections.empty());
	};

	auto getFreeConnection = [&]() -> Connection
	{
		Connection connection(std::move(impl_->freeConnections.front()));
		impl_->freeConnections.pop_front();
		return connection;
	};

	std::unique_lock<std::mutex> lock(impl_->mutex);

	//trying to return free connection if it is available
	if (hasFreeConnection())
	{
		return getFreeConnection();
	}

	//no free connections - trying to create one
	if (impl_->currentSize < impl_->maxSize)
	{
		++impl_->currentSize;
		return common::PimplFactory<Connection>::create(
			*this,
			RawConnectionHolder(new RawConnection(impl_->connectionString))
		);
	}

	//we have to wait for connections to appear
	impl_->waitersCount++;
	impl_->cv.wait(lock, hasFreeConnection);
	impl_->waitersCount--;
	return getFreeConnection();
}

void Pool::emplace(Connection connection)
{
	//WARN:
	//need locking mutex to protect freeConnections queue
	//from concurrent access
	//
	//Locking may be avoided
	//if cv and queue would be protected by different mutexes
	std::unique_lock<std::mutex> lock(impl_->mutex);
	impl_->freeConnections.push_back(std::move(connection));
	if (impl_->waitersCount > 0)
	{
		impl_->cv.notify_one();
	}
}

DEFINE_MOVABLE_PIMPL(Pool)

} //namespace db
} //namespace hda
