// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_FLAGS_GUARD
#define SAPPHIRE_CORE_FLAGS_GUARD

#include <Core/Misc/Flags_Internal.hpp>

namespace Sa
{
	/**
	*	\file Flags.hpp
	*
	*	\brief \b Definition of Sapphire's \b Flags type.
	*
	*	\ingroup Misc
	*	\{
	*/


	/**
	*	\brief \e Template class for \b bit-flags enum management.
	*
	*	\tparam Enum	Enum type.
	*	\tparam thMode	Thread-mode.
	*/
	template <typename Enum, ThreadMode thMode = ThreadMode::Unsafe>
	class Flags : protected Internal::Flags<Enum, thMode>
	{
		using Internal::Flags<Enum, thMode>::mBits;
		/// The inherited size for the Enum type.
		using TypeBits = typename Internal::Flags<Enum, thMode>::TypeBits;

	public:

		/**
		*	\brief \b Default constructor.
		*/
		Flags() = default;

		/**
		*	\brief \e Value constructor
		*
		*	\param[in] _value	Enum value to assign to the handle.
		*/
		constexpr Flags(Enum _value) noexcept;

		/**
		*	\brief \e Value constructor
		*
		*	\param[in] _flags	Bits value to assign to the handle.
		*/
		constexpr Flags(TypeBits _flags) noexcept;

		/**
		*	\brief \b Default \e move constructor.
		*/
		Flags(Flags&&) = default;

		/**
		*	\brief \b Default \e copy constructor.
		*/
		Flags(const Flags&) = default;


		/**
		*	\brief \e Getter of bool if the value is currently set.
		*
		*	\param[in] _value	Value to check with.
		*
		*	\return true if value is set, otherwise false.
		*/
		constexpr bool IsSet(Enum _value) const noexcept;


		/**
		*	\brief Add value to the current bit flags.
		*
		*	\param[in] _value	Value to add.
		*/
		void Add(Enum _value) noexcept;
		
		/**
		*	\brief Remove value to the current bit flags.
		*
		*	\param[in] _value	Value to remove.
		*/
		void Remove(Enum _value) noexcept;


		/**
		*	\brief \e Create a Flags with value lhs \b and rhs.
		*
		*	\param[in] _lhs		Left operand.
		*	\param[in] _rhs		Right operand.
		*
		*	\return Created Flags.
		*/
		static constexpr Flags EnumAND(Enum _lhs, Enum _rhs) noexcept;

		/**
		*	\brief \e Create a Flags with value lhs \b or rhs.
		*
		*	\param[in] _lhs		Left operand.
		*	\param[in] _rhs		Right operand.
		*
		*	\return Created Flags.
		*/
		static constexpr Flags EnumOR(Enum _lhs, Enum _rhs) noexcept;

		/**
		*	\brief \e Create a Flags with value lhs \b xor rhs.
		*
		*	\param[in] _lhs		Left operand.
		*	\param[in] _rhs		Right operand.
		*
		*	\return Created Flags.
		*/
		static constexpr Flags EnumXOR(Enum _lhs, Enum _rhs) noexcept;


		/**
		*	\brief Perform \b and operation between bit flags and Enum.
		*
		*	\param[in] _rhs		Right operand.
		*
		*	\return result of and operation.
		*/
		constexpr Flags operator&(Enum _rhs) const noexcept;

		/**
		*	\brief Perform \b and operation between 2 Flags.
		*
		*	\param[in] _rhs		Right operand.
		*
		*	\return result of and operation.
		*/
		constexpr Flags operator&(const Flags& _rhs) const noexcept;

		/**
		*	\brief Perform \b or operation between bit flags and Enum.
		*
		*	\param[in] _rhs		Right operand.
		*
		*	\return result of or operation.
		*/
		constexpr Flags operator|(Enum _rhs) const noexcept;

		/**
		*	\brief Perform \b or operation between 2 Flags.
		*
		*	\param[in] _rhs		Right operand.
		*
		*	\return result of or operation.
		*/
		constexpr Flags operator|(const Flags& _rhs) const noexcept;

		/**
		*	\brief Perform \b xor operation between bit flags and Enum.
		*
		*	\param[in] _rhs		Right operand.
		*
		*	\return result of xor operation.
		*/
		constexpr Flags operator^(Enum _rhs) const noexcept;

		/**
		*	\brief Perform \b xor operation between 2 Flags.
		*
		*	\param[in] _rhs		Right operand.
		*
		*	\return result of xor operation.
		*/
		constexpr Flags operator^(const Flags& _rhs) const noexcept;

		/**
		*	\brief Perform operator \b not on current bit flags.
		*
		*	\return result of not operation.
		*/
		constexpr Flags operator~() const noexcept;


		/**
		*	\brief \e Value operator= with enum.
		*
		*	\param[in] _rhs		Right operand.
		*
		*	\return this instance.
		*/
		Flags& operator=(Enum _rhs) noexcept;

		/**
		*	\brief \b Default \e move operator=.
		*
		*	\return this instance.
		*/
		Flags& operator=(Flags&&) = default;

		/**
		*	\brief \b Default \e copy operator=.
		*
		*	\return this instance.
		*/
		Flags& operator=(const Flags&) = default;

		/**
		*	\brief Perform \b and and \b assign operations with Enum.
		*
		*	\param[in] _rhs		Right operand.
		*
		*	\return this instance.
		*/
		Flags& operator&=(Enum _rhs) noexcept;

		/**
		*	\brief Perform \b and and \b assign operations with Flags.
		*
		*	\param[in] _rhs		Right operand.
		*
		*	\return this instance.
		*/
		Flags& operator&=(const Flags& _rhs) noexcept;

		/**
		*	\brief Perform \b or and \b assign operations with Enum.
		*
		*	\param[in] _rhs		Right operand.
		*
		*	\return this instance.
		*/
		Flags& operator|=(Enum _rhs) noexcept;

		/**
		*	\brief Perform \b or and \b assign operations with Flags.
		*
		*	\param[in] _rhs		Right operand.
		*
		*	\return this instance.
		*/
		Flags& operator|=(const Flags& _rhs) noexcept;

		/**
		*	\brief Perform \b xor and \b assign operations with Enum.
		*
		*	\param[in] _rhs		Right operand.
		*
		*	\return this instance.
		*/
		Flags& operator^=(Enum _rhs) noexcept;

		/**
		*	\brief Perform \b xor and \b assign operations with Flags.
		*
		*	\param[in] _rhs		Right operand.
		*
		*	\return this instance.
		*/
		Flags& operator^=(const Flags& _rhs) noexcept;


		/**
		*	\brief Perform \b equal operation between bit flags and enum.
		*
		*	\param[in] _rhs		Right operand.
		*
		*	\return result of equal operation.
		*/
		constexpr bool operator==(Enum _rhs) const noexcept;

		/**
		*	\brief Perform \b equal operation between 2 Flags.
		*
		*	\param[in] _rhs		Right operand.
		*
		*	\return result of equal operation.
		*/
		constexpr bool operator==(const Flags& _rhs) const noexcept;

		/**
		*	\brief Perform <b>not equal</b> operation between bit flags and enum.
		*
		*	\param[in] _rhs		Right operand.
		*
		*	\return result of not equal operation.
		*/
		constexpr bool operator!=(Enum _rhs) const noexcept;

		/**
		*	\brief Perform <b>not equal</b> operation between 2 Flags.
		*
		*	\param[in] _rhs		Right operand.
		*
		*	\return result of not equal operation.
		*/
		constexpr bool operator!=(const Flags& _rhs) const noexcept;


		/**
		*	\brief \e Cast operator to TypeBits to get bit flags.
		*
		*	\return bit flags.
		*/
		constexpr operator TypeBits() const noexcept;

		/**
		*	\brief \e Cast operator to bool.
		*
		*	Return the result of operation bit-flags != 0;
		*
		*	\return true if almost one flag is set, otherwise false.
		*/
		constexpr operator bool() const noexcept;
	};


	/**
	*	\brief Perform \b and operation between Enum and bit flags.
	*
	*	\tparam Enum		Enum type.
	*	\tparam thMode		Thread-mode.
	*
	*	\param[in] _lhs		Left operand.
	*	\param[in] _rhs		Right operand.
	*
	*	\return result of and operation.
	*/
	template <typename Enum, ThreadMode thMode>
	constexpr Flags<Enum, thMode> operator&(Enum _lhs, Flags<Enum, thMode> _rhs) noexcept;

	/**
	*	\brief Perform \b or operation between Enum and bit flags.
	*
	*	\tparam Enum		Enum type.
	*	\tparam thMode		Thread-mode.
	*
	*	\param[in] _lhs		Left operand.
	*	\param[in] _rhs		Right operand.
	*
	*	\return result of or operation.
	*/
	template <typename Enum, ThreadMode thMode>
	constexpr Flags<Enum, thMode> operator|(Enum _lhs, Flags<Enum, thMode> _rhs) noexcept;

	/**
	*	\brief Perform \b xor operation between Enum and bit flags.
	*
	*	\tparam Enum		Enum type.
	*	\tparam thMode		Thread-mode.
	*
	*	\param[in] _lhs		Left operand.
	*	\param[in] _rhs		Right operand.
	*
	*	\return result of xor operation.
	*/
	template <typename Enum, ThreadMode thMode>
	constexpr Flags<Enum, thMode> operator^(Enum _lhs, Flags<Enum, thMode> _rhs) noexcept;


	/**
	*	\brief Perform \b and operation between 2 Enum.
	*
	*	Call Flags::EnumAND().
	*
	*	\tparam Enum		Enum type.
	*	\tparam thMode		Thread-mode.
	*
	*	\param[in] _lhs		Left operand.
	*	\param[in] _rhs		Right operand.
	*
	*	\return result of and operation.
	*/
	template <typename Enum, ThreadMode thMode = ThreadMode::Unsafe>
	constexpr Flags<Enum, thMode> operator&(Enum _lhs, Enum _rhs) noexcept;

	/**
	*	\brief Perform \b or operation between 2 Enum.
	*
	*	Call Flags::EnumOR().
	*
	*	\tparam Enum		Enum type.
	*	\tparam thMode		Thread-mode.
	*
	*	\param[in] _lhs		Left operand.
	*	\param[in] _rhs		Right operand.
	*
	*	\return result of or operation.
	*/
	template <typename Enum, ThreadMode thMode = ThreadMode::Unsafe>
	constexpr Flags<Enum, thMode> operator|(Enum _lhs, Enum _rhs) noexcept;

	/**
	*	\brief Perform \b xor operation between 2 Enum.
	*
	*	Call Flags::EnumXOR().
	*
	*	\tparam Enum		Enum type.
	*	\tparam thMode		Thread-mode.
	*
	*	\param[in] _lhs		Left operand.
	*	\param[in] _rhs		Right operand.
	*
	*	\return result of xor operation.
	*/
	template <typename Enum, ThreadMode thMode = ThreadMode::Unsafe>
	constexpr Flags<Enum, thMode> operator^(Enum _lhs, Enum _rhs) noexcept;


	/** \} */
}

#include <Core/Misc/Flags.inl>

#endif
