#pragma once

#include "Logger.h"
#include <assert.h>
#include <thread>

using namespace std;
using namespace aricanli::general;

void log_test(int n) {
	Logger::Fatal(__LINE__, __FILE__, "fatal error %d ", n, " fdfdv ", 234);
	Logger::Debug(__LINE__, __FILE__, "debugging %d ", n, "Write Args", 3434, 3334);
	Logger::Error(__LINE__, __FILE__, "error %d ", n, "Args Errrrrrrorrrrr");
}

int main()
{
	thread threads[3];
	for (int i = 0; i < 3; i++)
		threads[i] = thread(log_test, i);

	for (int i = 0; i < 3; i++)
		threads[i].join();

	return 0;
}