#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "threadpool.hpp"


TEST_CASE("Threadpool initialization", "[Threadpool_init]")
{
	gsc_concurrency::Threadpool tpool(5);
	auto task = tpool.add_task([](int res) {
		if (res == 1)
			std::cout << "res==1";

	}, 1);
	tpool.start_pool();
	task.get();
}