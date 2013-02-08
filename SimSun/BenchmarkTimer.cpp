#include "BenchmarkTimer.h"
#include <cassert>

BenchmarkTimer::BenchmarkTimer()
{
	QueryPerformanceFrequency(&frequency);
	elapsedTime = 0;
	running = false;
}

void BenchmarkTimer::start()
{
	assert(!running);
	running = true;
	QueryPerformanceCounter(&tick_start);
}

void BenchmarkTimer::stop()
{
	assert(running);
	running = false;
	LARGE_INTEGER tick_end;
	QueryPerformanceCounter(&tick_end);
	elapsedTime += (float)(tick_end.QuadPart - tick_start.QuadPart)/frequency.QuadPart;
}

double BenchmarkTimer::elapsedSec()
{
	return elapsedTime;
}

double BenchmarkTimer::elapsedMSec()
{
	return elapsedTime*1000;
}

void BenchmarkTimer::reset()
{
	elapsedTime = 0;
	running = false;
}