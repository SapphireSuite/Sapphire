// Copyright 2020 Sapphire development team.All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_TIME_GUARD
#define SAPPHIRE_CORE_TIME_GUARD

#include <Core/Time/Tick.hpp>
#include <Core/Time/MilliSecond.hpp>
#include <Core/Time/Second.hpp>
#include <Core/Time/Minute.hpp>
#include <Core/Time/Hour.hpp>
#include <Core/Time/DateTime.hpp>

namespace Sa
{
	/**
	*	\file Time.hpp
	*
	*	\brief \b Definition of Sapphire's \b Time static accessors.
	*
	*	\ingroup Time
	*	\{
	*/


	/**
	*	\brief Static class which provide <b>time service</b> access.
	*/
	class Time
	{
#if SA_WIN

		/// \cond Internal
		/**
		*	\brief Nested class for Time::sInstance initialization.
		*
		*	Windows Implementation.
		*/
		struct TimeInitializer
		{
			/// Queried start time value.
			const uint64 start = 0u;

			/// Queried hardware frequency value.
			const float hardwareFrequency = 0.0f;
		};
		/// \endcond Internal

		/**
		*	\brief Queried time at the beginnig of the program (in hardware counts).
		*
		*	Windows implementation.
		*/
		const uint64 mStart = 0u;

		/**
		*	\brief Hardware dependant constant to convert <b> hardware ticks </b> to <b> Sappire's ticks </b> (multiplication).
		*
		*	Windows implementation.
		*/
		const float mHardwareFrequency = -1.0f;

		/**
		*	\brief \e Value Constructor to init constants.
		*
		*	Windows Implementation.
		*/
		Time(const TimeInitializer& _initializer) noexcept;

		/**
		*	\brief Platform dependant constant initiallizer.
		*
		*	Windows Implementation.
		*/
		static TimeInitializer Init();

#else

		/**
		*	\brief Queried time at the beginnig of the program (in ticks).
		*
		*	Unix implementation.
		*/
		const Tick mStart = 0.0f;

		/**
		*	\brief \e Default Constructor.
		*
		*	Unix Implementation.
		*/
		Time();

#endif

		/// Static always initialized instance.
		static Time sInstance;

	public:

		/**
		*	\brief Get the time in \c seconds since the beginning of the program.
		*
		*	\return time in seconds.
		*/
		SA_ENGINE_API static Second Now();

		/**
		*	\brief Get the time in \c ticks since the beginning of the program.
		*
		*	\return time in ticks.
		*/
		SA_ENGINE_API static Tick Ticks();

		/**
		*	\brief Get the current local date time.
		*
		*	\return time in seconds.
		*/
		SA_ENGINE_API static DateTime Date() noexcept;

		/**
		*	\brief Get the current date time at Greenwich.
		*
		*	\return time in seconds.
		*/
		SA_ENGINE_API static DateTime GMDate() noexcept;

		/**
		*	\brief Generate a \b seed based on the time.
		*
		*	Return the time since the Epoch (00:00:00 UTC, January 1st, 1970).
		*	This value is usually used for seed generation.
		*
		*	\return Seed based on time.
		*/
		SA_ENGINE_API static uint64 Seed() noexcept;

		/**
		*	\brief Sleep the program for an amount of time (in millisecond).
		*
		*	\param[in] _ms		Amount of time to sleep in millisecond.
		*/
		SA_ENGINE_API static void Sleep(MilliSecond _ms) noexcept;
	};


	/** \} */
}

#endif // GUARD
