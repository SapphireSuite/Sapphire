// Copyright 2020 Sapphire development team.All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_MILLI_SECOND_GUARD
#define SAPPHIRE_CORE_MILLI_SECOND_GUARD

#include <Core/Types/Int.hpp>
#include <Core/Support/EngineAPI.hpp>

namespace Sa
{
	class Tick;
	class Second;
	class Minute;
	class Hour;

	/**
	*	\file MilliSecond.hpp
	*
	*	\brief \b Definition of Sapphire's \b MilliSecond type.
	*
	*	\ingroup Time
	*	\{
	*/

	
	/**
	*	\brief \e Time-relative class which provide \b conversions with other \e time-relative classes.
	*/
	class MilliSecond
	{
		/// The handled time.
		float mTime = 0.0f;

	public:
		/// Constant to convert MilliSeconds to Ticks.
		static constexpr float ToTicks = 1000.0f;

		/// Constant to convert MilliSeconds to Seconds.
		static constexpr float ToSeconds = 1.0f / 1000.0f;

		/// Constant to convert MilliSeconds to Minutes.
		static constexpr float ToMinutes = 1.0f / 60000.0f;

		/// Constant to convert MilliSeconds to Hours.
		static constexpr float ToHours = 1.0f / 3600000.0f;

		/**
		*	\brief \b Default contructor.
		*/
		MilliSecond() = default;

		/**
		*	\brief \e Value contructor without conversion.
		*
		*	\param[in] _time	Time milliseconds as \c float.
		*/
		SA_ENGINE_API MilliSecond(float _time) noexcept;

		/**
		*	\brief \e Value contructor without conversion.
		*
		*	\param[in] _time	Time milliseconds as \c uint64.
		*/
		SA_ENGINE_API MilliSecond(uint64 _time) noexcept;


		/**
		*	\brief \e Value contructor with conversion.
		*
		*	\param[in] _time	Time in Tick.
		*/
		SA_ENGINE_API explicit MilliSecond(Tick _time) noexcept;

		/**
		*	\brief \e Value contructor with conversion.
		*
		*	\param[in] _time	Time in Second.
		*/
		SA_ENGINE_API explicit MilliSecond(Second _time) noexcept;

		/**
		*	\brief \e Value contructor with conversion.
		*
		*	\param[in] _time	Time in Minute.
		*/
		SA_ENGINE_API explicit MilliSecond(Minute _time) noexcept;

		/**
		*	\brief \e Value contructor with conversion.
		*
		*	\param[in] _time	Time in Hour.
		*/
		SA_ENGINE_API explicit MilliSecond(Hour _time) noexcept;


		/**
		*	\brief Access the handled value.
		*
		*	\return \c float without conversion.
		*/
		SA_ENGINE_API operator float() const noexcept;

		/**
		*	\brief \e Cast operator to \c Tick with proper conversion.
		*
		*	\return \c Tick converted from \c MilliSecond.
		*/
		SA_ENGINE_API operator Tick() const noexcept;

		/**
		*	\brief \e Cast operator to \c Second with proper conversion.
		*
		*	\return \c Second converted from \c MilliSecond.
		*/
		SA_ENGINE_API operator Second() const noexcept;

		/**
		*	\brief \e Cast operator to \c Minute with proper conversion.
		*
		*	\return \c Minute converted from \c MilliSecond.
		*/
		SA_ENGINE_API operator Minute() const noexcept;

		/**
		*	\brief \e Cast operator to \c Hour with proper conversion.
		*
		*	\return \c Hour converted from \c MilliSecond.
		*/
		SA_ENGINE_API operator Hour() const noexcept;
	};

	/**
	*	\brief _ms \b literal operator.
	*
	*	\param[in] _lit		Operand to convert as MilliSecond.
	*
	*	\return \c MilliSecond without time-conversion.
	*/
	SA_ENGINE_API MilliSecond operator""_ms(uint64 _lit) noexcept;

	/**
	*	\brief _ms \b literal operator.
	*
	*	\param[in] _lit		Operand to convert as MilliSecond.
	*
	*	\return \c MilliSecond without time-conversion.
	*/
	SA_ENGINE_API MilliSecond operator""_ms(long double _lit) noexcept;


	/** \} */
}

#endif // GUARD
