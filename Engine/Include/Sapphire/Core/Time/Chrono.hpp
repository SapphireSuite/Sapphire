// Copyright 2020 Sapphire development team.All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_CHRONO_GUARD
#define SAPPHIRE_CORE_CHRONO_GUARD

#include <Core/Time/Tick.hpp>

namespace Sa
{
	/**
	*	\file Chrono.hpp
	*
	*	\brief \b Definition of Sapphire's \b Chrono type.
	*
	*	\ingroup Time
	*	\{
	*/

	/**
	*	\brief Chronometer class to measure a <b>period of time</b>.
	*/
	class Chrono
	{
		/// The handled start time.
		Tick mStart;

	public:

		/**
		*	\brief \b Start the chronometer.
		*/
		SA_ENGINE_API void Start() noexcept;

		/**
		*	\brief \b End the chronometer.
		*
		*	\return The mesured time since the \b Start() call.
		*/
		SA_ENGINE_API Tick End() noexcept;

		/**
		*	\brief \b End the chronometer and instantly call \b Start() again.
		*
		*	\return The mesured time since the previous \b Start() call.
		*/
		SA_ENGINE_API Tick Restart() noexcept;
	};


	/** \} */
}

#endif // GUARD
