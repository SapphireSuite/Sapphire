// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_FLAGS_INTERNAL_GUARD
#define SAPPHIRE_CORE_FLAGS_INTERNAL_GUARD

#include <Core/Types/Int.hpp>

#include <Core/Thread/Atomic.hpp>
#include <Core/Thread/ThreadMode.hpp>

namespace Sa
{
	/// \cond Internal
	/**
	*	\file Flags_Internal.hpp
	*
	*	\brief \b Definition of Sapphire's <b>Internal Flags</b> type.
	*
	*	\ingroup Misc
	*	\{
	*/


	namespace Internal
	{
		/**
		*	\brief \e Internal Flags implementation <b>ThreadMode dependant</b>.
		*/
		template <typename Enum, ThreadMode thMode>
		class Flags;

		/**
		*	\brief \e Internal Flags \b thread-unsafe implementation.
		*/
		template <typename Enum>
		class Flags<Enum, ThreadMode::Unsafe>
		{
		protected:
			/// The minimal bits size for the Enum type.
			using TypeBits = UIntOfSize<sizeof(Enum)>;

			/// bit-flags value.
			TypeBits mBits = 0;

			/**
			*	\brief \b Default constructor.
			*/
			Flags() = default;

			/**
			*	\brief \b Value constructor.
			*
			*	\param[in] _bits	Bits value to assign to the handle.
			*/
			constexpr Flags(TypeBits _bits) noexcept;
		};

		/**
		*	\brief \e Internal Flags \b thread-safe implementation.
		*/
		template <typename Enum>
		class Flags<Enum, ThreadMode::Safe>
		{
		protected:
			/// The minimal bits size for the Enum type.
			using TypeBits = UIntOfSize<sizeof(Enum)>;

			/// bit-flags value.
			Atomic<TypeBits> mBits = 0;

			/**
			*	\brief \b Default constructor.
			*/
			Flags() = default;

			/**
			*	\brief \b Value constructor.
			*
			*	\param[in] _bits	Bits value to assign to the handle.
			*/
			Flags(TypeBits _bits) noexcept;
		};
	}


	/** \} */
	/// \endcond Internal
}

#include <Core/Misc/Flags_Internal.inl>

#endif // GUARD
