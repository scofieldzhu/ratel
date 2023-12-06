#include "test_logger.h"
#include "spdlog/spdlog.h"

void TestCase_SPDLogger()
{
	spdlog::set_pattern("[%H:%M:%S %z] [%^%L%$] [thread %t] %v");
	spdlog::info("Welcome to spdlog version {}.{}.{}  !", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR,
		SPDLOG_VER_PATCH);

}

