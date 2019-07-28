// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <cmath>

#include "UnitTest.hpp"

#include <Sapphire/Collections/Time>
using namespace Sa;

int main()
{
	LOG("=== Start ===");


	LOG("\n === MilliSecond ===");
	SA_TEST(500.0_ms, ==, 500.0f);
	SA_TEST(MilliSecond(896_sec), ==, 896.0f * 1000.0f);
	SA_TEST(MilliSecond(536_min), ==, 536.0f * 60000.0f);
	SA_TEST(MilliSecond(3_hr), ==, 3.0f * 3600000.0f);


	LOG("\n === Second ===");
	SA_TEST(Second(500.0_ms), ==, 0.5f);
	SA_TEST(896_sec, ==, 896.0f);
	SA_TEST(Second(536_min), ==, 536.0f * 60.0f);
	SA_TEST(Second(3_hr), ==, 3.0f * 3600.0f);


	LOG("\n === Minute ===");
	SA_TEST(Minute(500.0_ms), ==, 500.0f / 60000.0f);
	SA_TEST(Minute(753_sec), ==, 753.0f / 60.0f);
	SA_TEST(536_min, ==, 536.0f);
	SA_TEST(Minute(3_hr), ==, 3.0f * 60.0f);


	LOG("\n === Hour ===");
	SA_TEST(Hour(500.0_ms), ==, 500.0f / 3600000.0f);
	SA_TEST(Hour(896_sec), ==, 896.0f / 3600.0f);
	SA_TEST(Hour(536_min), ==, 536.0f / 60.0f);
	SA_TEST(3_hr, ==, 3.0f);


	LOG("\n=== Chrono ===");

	Chrono time;
	time.Start();

	Time::Sleep(1354_ms);

	Tick end = time.End();

	// 1.0ms of tolerence (hardware dependent).
	SA_TRY_TEST(std::abs(MilliSecond(end) - 1354.0f), <, 1.0f)


	LOG("\n=== Date ===");

	const DateTime date = Time::Date();
	LOG("Local Date:\t\t" << date.GetDayName() << ' ' << \
		date.GetMonthName() << '\t' << \
		static_cast<uint32>(date.day) << '/' << \
		static_cast<uint32>(date.month) << '/' << \
		static_cast<uint32>(date.year) << '\t' << \
		static_cast<uint32>(date.hour) << ':' << \
		static_cast<uint32>(date.minute) << ':' << \
		static_cast<uint32>(date.second));

	const DateTime gmDate = Time::GMDate();
	LOG("Greenwich Mean Date:\t" << gmDate.GetDayName() << ' ' << \
		gmDate.GetMonthName() << '\t' << \
		static_cast<uint32>(gmDate.day) << '/' << \
		static_cast<uint32>(gmDate.month) << '/' << \
		static_cast<uint32>(gmDate.year) << '\t' << \
		static_cast<uint32>(gmDate.hour) << ':' << \
		static_cast<uint32>(gmDate.minute) << ':' << \
		static_cast<uint32>(gmDate.second));


	LOG("\n=== End ===");

	return UnitTest::Exit;
}
