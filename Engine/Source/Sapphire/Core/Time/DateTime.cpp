// Copyright 2020 Sapphire development team.All Rights Reserved.

#include <Core/Time/DateTime.hpp>

#include <Core/Debug/Debug.hpp>

namespace Sa
{
	const char8* DateTime::GetDayName() const
	{
		SA_ASSERT(weekDay >= 0 && weekDay <= 6, OutOfRange, Tools, weekDay, 0u, 6u);

		return DateTime::sDays[weekDay];
	}

	const char8* DateTime::GetMonthName() const
	{
		SA_ASSERT(month >= 1 && month <= 12, OutOfRange, Tools, month, 1u, 12u);
		
		return DateTime::sMonths[month - 1];
	}
}
