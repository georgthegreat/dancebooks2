#include <hda/db/settings.hpp>
#include <hda/db/pool.hpp>
#include <hda/db/connection.hpp>
#include <hda/db/transaction.hpp>

#include "config.h"

#define BOOST_TEST_DYN_LINK
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <chrono>
#include <cstddef>
#include <future>
#include <iostream>
#include <thread>

namespace hda {
namespace db {
namespace tests {

using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::duration_cast;

namespace {
Pool makePool()
{
	auto settings = Settings()
		.setHost(DB_SERVER_HOST)
		.setPort(DB_SERVER_PORT)
		.setUser(DB_SERVER_USER)
		.setPassword(DB_SERVER_PASSWORD)
		.setDatabaseName(DB_SERVER_DATABASE)
	;
	const size_t MAX_SIZE = 1;
	return Pool(settings, MAX_SIZE);
}

} //anonymous namespace

BOOST_AUTO_TEST_CASE(test_connection_pool)
{
	#ifndef HAVE_DB_SERVER
		return;
	#endif
	auto pool = makePool();
	BOOST_CHECK_EQUAL(pool.currentSize(), 0);
	BOOST_CHECK_EQUAL(pool.freeConnectionsSize(), 0);

	{
		auto conn = pool.getConnection();
		BOOST_CHECK_EQUAL(pool.currentSize(), 1);
		BOOST_CHECK_EQUAL(pool.freeConnectionsSize(), 0);
	}

	BOOST_CHECK_EQUAL(pool.currentSize(), 1);
	BOOST_CHECK_EQUAL(pool.freeConnectionsSize(), 1);
}

BOOST_AUTO_TEST_CASE(test_concurrect_connection)
{
	#ifndef HAVE_DB_SERVER
		return;
	#endif
	auto pool = makePool();
	auto blocker = [&]()
	{
		auto conn = pool.getConnection();
		std::this_thread::sleep_for(seconds(3));
	};

	auto start = std::chrono::steady_clock::now();
	std::async(std::launch::async, blocker);
	//waiting for blocker to get the connection
	std::this_thread::sleep_for(milliseconds(100));
	auto conn = pool.getConnection();
	auto end = std::chrono::steady_clock::now();
	auto passedMs = duration_cast<milliseconds>(end - start);

	std::cout <<
		"Waited for connection for " <<
		passedMs.count() << " " << "milliseconds" << std::endl
	;
	BOOST_CHECK(passedMs.count() > 2700);
	BOOST_CHECK(passedMs.count() < 3300);
	BOOST_CHECK_EQUAL(pool.currentSize(), 1);
	BOOST_CHECK_EQUAL(pool.freeConnectionsSize(), 0);
}

BOOST_AUTO_TEST_CASE(test_transaction)
{
	#ifndef HAVE_DB_SERVER
		return;
	#endif
	auto pool = makePool();
	{
		auto txn = pool.getConnection().makeReadOnlyTransaction();
		auto result = txn.execute("SELECT 1");
		BOOST_REQUIRE_EQUAL(result.size(), 1);
		BOOST_CHECK_EQUAL(result.front().front().as<std::string>(), "1");
	}
	//connection is properly returned to pool after transactions end
	BOOST_CHECK_EQUAL(pool.currentSize(), 1);
	BOOST_CHECK_EQUAL(pool.freeConnectionsSize(), 1);
	{
		auto conn = pool.getConnection();
		auto txn = std::move(conn).makeReadOnlyTransaction();
		conn = std::move(txn).commit();
		txn = std::move(conn).makeReadOnlyTransaction();
		auto result = txn.execute("SELECT 1");
		BOOST_REQUIRE_EQUAL(result.size(), 1);
		BOOST_CHECK_EQUAL(result.front().front().as<std::string>(), "1");
	}
}

} //namespace tests
} //namespace db
} //namespace hda
