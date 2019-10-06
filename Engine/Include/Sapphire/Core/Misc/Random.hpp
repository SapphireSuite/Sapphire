// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_RANDOM_GUARD
#define SAPPHIRE_CORE_RANDOM_GUARD

#include <random>

#include <Core/Support/EngineAPI.hpp>

#include <Core/Types/Variadics/Limits.hpp>
#include <Core/Types/Conditions/IsIntegral.hpp>
#include <Core/Types/Conditions/IsFloatingPoint.hpp>

namespace Sa
{
	/**
	*	\file Random.hpp
	*
	*	\brief \b Definition of Sapphire's <b>random generator</b> classes.
	*
	*	\ingroup Misc
	*	\{
	*/


	/**
	*	\brief Handle the \e std::default_random_engine class
	*
	*	Use std:: implementation and Sapphire's norm.
	*	See more documentation: http://www.cplusplus.com/reference/random/default_random_engine/
	*/
	class RandEngine
	{
		/// The handled std::default_random_engine.
		std::default_random_engine mHandle;

		/// The default unseeded hardware-dependant random implementation.
		SA_ENGINE_API static std::random_device sDefaultEngine;

		/// Only Random implementation can access to handle and sDefaultEngine.
		template <typename T>
		friend struct Random;

	public:

		/**
		*	\brief \e Value constructor.
		*
		*	\param[in] _seed	Seed to initialize the random engine.
		*/
		SA_ENGINE_API RandEngine(uint32 _seed = 0) noexcept;

		/**
		*	\b Default \e move constructor.
		*/
		RandEngine(RandEngine&&) = default;

		/**
		*	\b Default \e copy constructor.
		*/
		RandEngine(const RandEngine&) = default;


		/**
		*	\brief \b Default \e move operator=.
		*
		*	\return this instance.
		*/
		RandEngine& operator=(RandEngine&&) = default;

		/**
		*	\brief \b Default \e copy operator=.
		*
		*	\return this instance.
		*/
		RandEngine& operator=(const RandEngine&) = default;
	};


	/**
	*	\brief \e Generate a random value of typ T in range [min;max[.
	*
	*	random values range: min <= rand < max
	*
	*	\tparam T	Type of the random.
	*/
	template <typename T>
	struct Random
	{
		/**
		*	\brief \e Generate a \b random T-value in range [min;max[.
		*
		*	random values range: min <= rand < max
		*
		*	\param[in] _min		Included min.
		*	\param[in] _max		Excluded max.
		*	\param[in] _en		Seeded Engine (or null for unseeded).
		*
		*	\return Generated random in range [min;max[.
		*/
		static T Value(const T& _min = Limits<T>::min, const T& _max = Limits<T>::max + 1, RandEngine* _en = nullptr) noexcept;
	};

	/// \cond Internal
	/**
	*	\brief Random \e specialization for \c bool.
	*
	*	\implements Random
	*/
	template <>
	struct Random<bool>
	{
		/**
		*	\brief \e Generate a \b random bool value.
		*
		*	\param[in] _en	Seeded Engine (or null for unseeded).
		*
		*	\return Generated random bool (true or false).
		*/
		SA_ENGINE_API static bool Value(RandEngine* _en = nullptr) noexcept;
	};
	/// \endcond Internal


	/** \} */
}

#include <Core/Misc/Random.inl>

#endif // GUARD
