// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_MUTEX_GUARD
#define SAPPHIRE_CORE_MUTEX_GUARD

#include <mutex>

#include <Core/Support/EngineAPI.hpp>

#include <Core/Misc/RAII.hpp>

namespace Sa
{
	/**
	*	\file Mutex.hpp
	*
	*	\brief \b Definition of Sapphire's \b Mutex type.
	*
	*	\ingroup Thread
	*	\{
	*/


	/**
	*	\brief Handle of the \e std::mutex standard class.
	*
	*	Use std:: implementation and Sapphire's norm.
	*	See more documentation: https://en.cppreference.com/w/cpp/thread/mutex
	*/
	class Mutex
	{
		/// The handled std::mutex.
		std::mutex mHandle;

	public:

		/**
		*	\brief \b Default constructor.
		*/
		Mutex() = default;

		/**
		*	\brief \b Deleted \e move constructor
		*/
		Mutex(Mutex&&) = delete;

		/**
		*	\brief \b Deleted \e copy constructor.
		*/
		Mutex(const Mutex&) = delete;


		/**
		*	\brief \b Locks the mutex, \b blocks if the mutex is not available.
		*/
		SA_ENGINE_API void Lock();

		/**
		*	\brief \b Tries to \b lock the mutex.
		*
		*	\return true if the lock was acquired successfully, otherwise false.
		*/
		SA_ENGINE_API bool TryLock();

		/**
		*	\brief \b Unlock the mutex.
		*/
		SA_ENGINE_API void Unlock();


		/**
		*	\brief \b Deleted \e move operator=.
		*
		*	\return this instance
		*/
		Mutex& operator=(Mutex&&) = delete;

		/**
		*	\brief \b Deleted \e copy operator=.
		*
		*	\return this instance
		*/
		Mutex& operator=(const Mutex&) = delete;
	};


	/**
	*	\brief RAII specialization for Mutex.
	*
	*	\implements RAII
	*/
	template <>
	class RAII<Mutex> final : RAIIBase
	{
		/// The handled mutex.
		Mutex& mHandle;

	public:

		/**
		*	\brief Value constructor: \b Lock() the handled mutex.
		*
		*	\param[in, out] _mutex		Mutex to handle.
		*/
		SA_ENGINE_API RAII(Mutex& _mutex);

		/**
		*	\brief Destructor: \b Unlock() the handled mutex.
		*/
		SA_ENGINE_API ~RAII();
	};


	/** \} */
}

#endif // GUARD
