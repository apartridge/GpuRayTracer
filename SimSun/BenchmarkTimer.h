#pragma once
#include <Windows.h>
#include <time.h>

class BenchmarkTimer
{
public:
	BenchmarkTimer();
	void start();
	void stop();
	double elapsedSec();
	double elapsedMSec();
	void reset();
private:
	double elapsedTime;
	LARGE_INTEGER frequency, tick_start;
	bool running;
};