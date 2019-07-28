// Copyright 2020 Sapphire development team.All Rights Reserved.

#include <string>
#include <iostream>

#include <ctime>

#include <Core/Time/Time.hpp>

#if SA_WIN

	#include <Core/Support/Windows.hpp>

#else

	#include <unistd.h>

#endif

namespace Sa
{
#if SA_WIN
	Time Time::sInstance(Init());

	Time::Time(const TimeInitializer& _initializer) noexcept :
		mStart{ _initializer.start },
		mHardwareFrequency{ _initializer.hardwareFrequency }
	{
	}

	Time::TimeInitializer Time::Init()
	{
		LARGE_INTEGER start;

		// TODO: Add Check on return 0.
		QueryPerformanceCounter(&start);

		LARGE_INTEGER frequency;

		// TODO: Add Check on return 0.
		QueryPerformanceFrequency(&frequency);

		return TimeInitializer{ static_cast<uint64>(start.QuadPart), Second::ToTicks / static_cast<float>(frequency.QuadPart) };
	}

#else

	Time Time::sInstance;

	Time::Time() : mStart{ Ticks() }
	{
	}

#endif

	Second Time::Now()
	{
		return Time::Ticks(); // Auto convertion.
	}

	Tick Time::Ticks()
	{
#if SA_WIN

		LARGE_INTEGER end;

		// TODO: Add Check on return 0.
		QueryPerformanceCounter(&end);

		return (end.QuadPart - sInstance.mStart) * sInstance.mHardwareFrequency;

#else

		struct timespec end;

		// TODO: Add Check on return -1.
		clock_gettime(CLOCK_MONOTONIC, &end);

		return (end.tv_sec * Second::ToTicks + end.tv_nsec / 1000.0f) - sInstance.mStart;

#endif
	}

	DateTime Time::Date() noexcept
	{
		tm qTime; // Queried time.
		const time_t currTime = time(nullptr);

#if SA_WIN
		localtime_s(&qTime, &currTime);
#else
		localtime_r(&currTime, &qTime);
#endif

		return DateTime
		{
			static_cast<uint8>(qTime.tm_sec),
			static_cast<uint8>(qTime.tm_min),
			static_cast<uint8>(qTime.tm_hour),
			static_cast<uint8>(qTime.tm_mday),
			static_cast<uint8>(qTime.tm_mon + 1),		// January is month 1.
			static_cast<uint16>(qTime.tm_year + 1900),	// tm_year is the year since 1900.
			static_cast<uint8>(qTime.tm_wday)
		};
	}
	
	DateTime Time::GMDate() noexcept
	{
		tm qTime; // Queried time.
		const time_t currTime = time(nullptr);

#if SA_WIN
		gmtime_s(&qTime, &currTime);
#else
		gmtime_r(&currTime, &qTime);
#endif

		return DateTime
		{
			static_cast<uint8>(qTime.tm_sec),
			static_cast<uint8>(qTime.tm_min),
			static_cast<uint8>(qTime.tm_hour),
			static_cast<uint8>(qTime.tm_mday),
			static_cast<uint8>(qTime.tm_mon + 1),		// January is month 1.
			static_cast<uint16>(qTime.tm_year + 1900),	// tm_year is the year since 1900.
			static_cast<uint8>(qTime.tm_wday)
		};
	}

	uint64 Time::Seed() noexcept
	{
		return time(nullptr);
	}

	void Time::Sleep(MilliSecond _ms) noexcept
	{
#if SA_WIN
		::Sleep(static_cast<uint32>(_ms));
#else
		usleep(Tick(_ms));
#endif
	}
}
