// Copyright 2020 Sapphire development team.All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_TICK_GUARD
#define SAPPHIRE_CORE_TICK_GUARD

#include <Core/Types/Int.hpp>
#include <Core/Support/EngineAPI.hpp>

namespace Sa
{
	class MilliSecond;
	class Second;
	class Minute;
	class Hour;

	/**
	*	\file Tick.hpp
	*
	*	\brief \b Definition of Sapphire's \b Tick type.
	*
	*	\ingroup Time
	*	\{
	*/


	/**
	*	\brief \e Time-relative class which provide \b conversions with other \e time-relative classes.
	*
	*	1 Tick is 1 MicroSecond (1e-6 second).
	*/
	class Tick
	{
		/// The handled time.
		float mTime = 0u;

	public:
		/// Constant to convert Ticks to MilliSeconds.
		static constexpr float ToMilliSeconds = 1.0f / 1000.0f;

		/// Constant to convert Ticks to Seconds.
		static constexpr float ToSeconds = 1.0f / 1000000.0f;

		/// Constant to convert Ticks to Minutes.
		static constexpr float ToMinutes = 1.0f / 60000000.0f;

		/// Constant to convert Ticks to Hours.
		static constexpr float ToHours = 1.0f / 3600000000.0f;

		/**
		*	\brief \b Default contructor.
		*/
		Tick() = default;

		/**
		*	\brief \e Value contructor without conversion.
		*
		*	\param[in] _time	Time ticks as \c float.
		*/
		SA_ENGINE_API Tick(float _time) noexcept;

		/**
		*	\brief \e Value contructor without conversion.
		*
		*	\param[in] _time	Time ticks as \c uint64.
		*/
		SA_ENGINE_API Tick(uint64 _time) noexcept;


		/**
		*	\brief \e Value contructor with conversion.
		*
		*	\param[in] _time	Time in MilliSecond.
		*/
		SA_ENGINE_API explicit Tick(MilliSecond _time) noexcept;

		/**
		*	\brief \e Value contructor with conversion.
		*
		*	\param[in] _time	Time in Second.
		*/
		SA_ENGINE_API explicit Tick(Second _time) noexcept;

		/**
		*	\brief \e Value contructor with conversion.
		*
		*	\param[in] _time	Time in Minute.
		*/
		SA_ENGINE_API explicit Tick(Minute _time) noexcept;

		/**
		*	\brief \e Value contructor with conversion.
		*
		*	\param[in] _time	Time in Hour.
		*/
		SA_ENGINE_API explicit Tick(Hour _time) noexcept;


		/**
		*	\brief Access the handled value.
		*
		*	\return ticks as \c float without conversion.
		*/
		SA_ENGINE_API operator float() const noexcept;
		
		/**
		*	\brief \e Cast operator to \c MilliSecond with proper conversion.
		*
		*	\return \c MilliSecond converted from \c Tick.
		*/
		SA_ENGINE_API operator MilliSecond() const noexcept;

		/**
		*	\brief \e Cast operator to \c Second with proper conversion.
		*
		*	\return \c Second converted from \c Tick.
		*/
		SA_ENGINE_API operator Second() const noexcept;

		/**
		*	\brief \e Cast operator to \c Minute with proper conversion.
		*
		*	\return \c Minute converted from \c Tick.
		*/
		SA_ENGINE_API operator Minute() const noexcept;

		/**
		*	\brief \e Cast operator to \c Hour with proper conversion.
		*
		*	\return \c Hour converted from \c Tick.
		*/
		SA_ENGINE_API operator Hour() const noexcept;
	};

	/**
	*	\brief _tk \b literal operator.
	*
	*	\param[in] _lit		Operand to convert as Tick.
	*
	*	\return \c Tick without time-conversion.
	*/
	SA_ENGINE_API Tick operator""_tk(uint64 _lit) noexcept;

	/**
	*	\brief _sec \b literal operator.
	*
	*	\param[in] _lit		Operand to convert as Tick.
	*
	*	\return \c Tick without time-conversion.
	*/
	SA_ENGINE_API Tick operator""_tk(long double _lit) noexcept;


	/** \} */
}

#endif // GUARD
