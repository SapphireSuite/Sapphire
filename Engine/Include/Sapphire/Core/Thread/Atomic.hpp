// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_ATOMIC_GUARD
#define SAPPHIRE_CORE_ATOMIC_GUARD

#include <atomic>

#include <Core/Types/Int.hpp>

namespace Sa
{
	/**
	*	\file Atomic.hpp
	*
	*	\brief \b Definition of Sapphire's \b Atomic type.
	*
	*	\ingroup Thread
	*	\{
	*/

	/**
	*	\brief Handle of the \e std::atomic standard class.
	*
	*	Use std:: implementation and Sapphire's norm.
	*	See more documentation: https://en.cppreference.com/w/cpp/atomic/atomic
	*
	*	\tparam T	Type of atomic handle.
	*/
	template <typename T>
	class Atomic
	{
		/// The handled std::atomic.
		std::atomic<T> mHandle;

	public:

		/**
		*	\brief \b Default constructor.
		*/
		Atomic() = default;

		/**
		*	\brief \e Value constructor
		*
		*	\param[in] _value	Value to assign to the handle.
		*/
		Atomic(T _value) noexcept;

		/**
		*	\brief \b Deleted \b move constructor.
		*/
		Atomic(Atomic&&) = delete;

		/**
		*	\brief \b Deleted \e copy constructor.
		*/
		Atomic(const Atomic&) = delete;

		
		/**
		*	\brief \e Getter of the handled value.
		*
		*	Wrap the load() function.
		*
		*	\return the handled value.
		*/
		T Get() const noexcept;

		/**
		*	\brief \e Setter of the handled value.
		*
		*	Wrap the store function.
		*
		*	\param[in] _value	New value to set.
		*/
		void Set(T _value) noexcept;

		/**
		*	\brief Perform a Get and Set operation.
		*
		*	\param[in] _value	New value to set.
		*
		*	\return The previous handled value.
		*/
		T Exchange(T _value);


		/**
		*	\brief Check whether the atomic operations on object of this type are lock-free.
		*
		*	\return true if the operation on this type are lock-free, otherwise false.
		*/
		static bool IsLockFree() noexcept;


		/**
		*	\brief Assign handle to value.
		*
		*	\param[in] _rhs		Value to assign.
		*
		*	\return this instance.
		*/
		Atomic& operator=(T _rhs) noexcept;

		/**
		*	\brief \b Deleted \e move operator=.
		*
		*	\return this instance.
		*/
		Atomic& operator=(Atomic&&) = delete;

		/**
		*	\brief \b Deleted \e copy operator=.
		*
		*	\return this instance.
		*/
		Atomic& operator=(const Atomic&) = delete;


		/**
		*	\brief Perform \e pre-increment value.
		*
		*	\return handled value.
		*/
		T operator++() noexcept;

		/**
		*	\brief Perform \e post-increment value.
		*
		*	\return handled value.
		*/
		T operator++(int32) noexcept;

		/**
		*	\brief Perform \e pre-decrement value.
		*
		*	\return handled value.
		*/
		T operator--() noexcept;

		/**
		*	\brief Perform \e post-decrement value.
		*
		*	\return handled value.
		*/
		T operator--(int32) noexcept;


		/**
		*	\brief Perform \b atomic \e addition.
		*
		*	\param[in] _rhs		Value to \e add.
		*
		*	\return handled value.
		*/
		T operator+=(T _rhs) noexcept;

		/**
		*	\brief Perform \b atomic \e substraction.
		*
		*	\param[in] _rhs		Value to \e substract.
		*
		*	\return handled value.
		*/
		T operator-=(T _rhs) noexcept;

		/**
		*	\brief Perform \b atomic <em>bitwise and</em>.
		*
		*	\param[in] _rhs		Value to perform <em>bitwise and</em> with.
		*
		*	\return handled value.
		*/
		T operator&=(T _rhs) noexcept;

		/**
		*	\brief Perform \b atomic <em>bitwise or</em>.
		*
		*	\param[in] _rhs		Value to perform <em>bitwise or</em> with.
		*
		*	\return handled value.
		*/
		T operator|=(T _rhs) noexcept;

		/**
		*	\brief Perform \b atomic <em>bitwise exclusive or</em>.
		*
		*	\param[in] _rhs		Value to perform <em>bitwise exclusive or</em> with.
		*
		*	\return handled value.
		*/
		T operator^=(T _rhs) noexcept;


		/**
		*	\brief \e Cast operator to \c T.
		*
		*	\return The handled value of T.
		*/
		operator T() const noexcept;
	};


	/** \} */
}

#include <Core/Thread/Atomic.inl>

#endif // GUARD
