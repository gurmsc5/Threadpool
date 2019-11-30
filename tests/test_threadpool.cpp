#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "threadpool.hpp"


TEST_CASE("Threadpool initialization", "[Threadpool_init]")
{
	gsc_concurrency::Threadpool tpool(5);

	int origVar = 5;
	int varToAdd = 1;

	std::string origString = "Gurmehar";
	std::string stringToAdd = "Cheema";

	auto task = tpool.add_task([&origVar](int addVar) {
		origVar += addVar;

	}, varToAdd);

	auto task2 = tpool.add_task([&origString](const std::string& addStr)
		{
			origString += " " + addStr;
		}, stringToAdd);

	tpool.start_pool();
	task.get();

	REQUIRE(origVar == 6);
	REQUIRE(origString == "Gurmehar Cheema");
}