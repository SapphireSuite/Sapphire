// Copyright 2020 Sapphire development team.All Rights Reserved.

#include <Core/Time/DateTime.hpp>

namespace Sa
{
	const char8* DateTime::GetDayName() const
	{
		// TODO Add dange check.
		return DateTime::sDays[weekDay];
	}

	const char8* DateTime::GetMonthName() const
	{
		// TODO Add dange check.
		return DateTime::sMonths[month - 1];
	}
}
