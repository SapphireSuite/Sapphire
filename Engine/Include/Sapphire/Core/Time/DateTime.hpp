// Copyright 2020 Sapphire development team.All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_DATE_TIME_GUARD
#define SAPPHIRE_CORE_DATE_TIME_GUARD

#include <Core/Types/Char.hpp>

#include <Core/Time/Second.hpp>
#include <Core/Time/Minute.hpp>
#include <Core/Time/Hour.hpp>

namespace Sa
{
	/**
	*	\file DateTime.hpp
	*
	*	\brief \b Definition of Sapphire's \b Date time.
	*
	*	\ingroup Time
	*	\{
	*/


	/**
	*	\brief \e Time-relative struct which provide <b>date time service</b>.
	*/
	struct DateTime
	{
		/// Week days 3-char8 name.
		SA_ENGINE_API static constexpr const char8* sDays[7]{ "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };

		/// Month 3-char8 name.
		SA_ENGINE_API static constexpr const char8* sMonths[12]{ "Jan", "Fev", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

		/// second number [0, 59].
		uint8 second = 0;

		/// minute number [0, 59].
		uint8 minute = 0;

		/// hour number [0, 23].
		uint8 hour = 0;

		/// day of the month number [1, 31].
		uint8 day = 0;

		/// month number [1, 12].
		uint8 month = 0;

		/// Year number.
		uint16 year = 0;

		/// day number of the week [0, 6].
		uint8 weekDay = 0;

		/**
		*	\brief \e Getter of day name.
		*
		*	\return 3 char8 of day name.
		*/
		SA_ENGINE_API const char8* GetDayName() const;

		/**
		*	\brief \e Getter of month name.
		*
		*	\return 3 char8 of month name.
		*/
		SA_ENGINE_API const char8* GetMonthName() const;
	};


	/** \} */
}

#endif // GUARD
