#pragma once

#include <windows.h>
#include <iostream>

class DiagTimer
{
public:
	void StartTimer()
	{
		LARGE_INTEGER li;
		if (!QueryPerformanceFrequency(&li))
			std::cout << "QueryPerformanceFrequency failed!\n";

		PCFreq = double(li.QuadPart) / 1000.0;

		QueryPerformanceCounter(&li);
		CounterStart = li.QuadPart;
	}

	double GetTimer()
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		return double(li.QuadPart - CounterStart) / PCFreq;
	}

	void ResetTimer()
	{
		PCFreq = 0.0;
		CounterStart = 0;
	}

private:
	double PCFreq = 0.0;
	__int64 CounterStart = 0;
};