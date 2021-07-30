#pragma once

#include "Logger.h"
#include <assert.h>
#include <thread>

using namespace std;
using namespace aricanli::general;

void log_test(int n) {
	LOG_DEBUG("debugging %d", n, "Write Args", 3434, 3334);
	LOG_WARN("warning %d", n, "Write Args Warn", 1234, 1234, 7853);
	LOG_ERROR("error %d", n, "Args Errrrrrrorrrrr");
	LOG_FATAL("fatal error %d", n);
	LOG_INFO("info : %d", n);
	LOG_VERBOSE("Verbose %d", n);
}

int main()
{
	Logger::enable_file_output("C:\\Users\\hp\\source\\repos\\log.txt");
	thread threads[3];
	for (int i = 0; i < 3; i++)
		threads[i] = thread(log_test, i);

	for (int i = 0; i < 3; i++)
		threads[i].join();

	return 0;
}