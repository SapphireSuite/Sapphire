// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_THREAD_GUARD
#define SAPPHIRE_CORE_THREAD_GUARD

#include <thread>

#include <Core/Support/EngineAPI.hpp>

#include <Core/Types/Int.hpp>

#include <Core/Algorithms/Move.hpp>
#include <Core/Algorithms/Forward.hpp>

namespace Sa
{
	/**
	*	\file Thread.hpp
	*
	*	\brief \b Definition of Sapphire's \b Thread type.
	*
	*	\ingroup Thread
	*	\{
	*/


	/**
	*	\brief Handle of the \e std::thread standard class.
	*
	*	Use std:: implementation and Sapphire's norm.
	*	See more documentation: https://en.cppreference.com/w/cpp/thread/thread
	*/
	class Thread
	{
		/// The handled std::thread.
		std::thread mHandle;

	public:

		/**
		*	\brief \b Default constructor.
		*/
		Thread() = default;

		/**
		*	\brief \e Value constructor.
		*
		*	\tparam Function	Function declaration type.
		*	\tparam Args		Arguments type of the function.
		*
		*	\param[in] _func	Function to be executed by the thread.
		*	\param[in] _args	Arguments forwarded to the function.
		*/
		template <typename Function, typename... Args>
		Thread(Function&& _func, Args&&... _args);

		/**
		*	\brief \b Default \e move constructor.
		*/
		Thread(Thread&&) = default;

		/**
		*	\brief \b Deleted \e copy constructor.
		*/
		Thread(const Thread&) = delete;

		/**
		*	\brief Destructor force call Join().
		*/
		SA_ENGINE_API ~Thread() noexcept;


		/**
		*	\brief \e Getter of this thread ID.
		*
		*	\return This thread ID.
		*/
		SA_ENGINE_API uint32 GetID() const noexcept;

		/**
		*	\brief \e Getter if this thread is currently running a function.
		*
		*	\return bool is currently running.
		*/
		SA_ENGINE_API bool IsRunning() const noexcept;


		/**
		*	\brief Blocks the current thread until this finished its execution.
		*
		*	Performe std:: joinable() check before calling join().
		*/
		SA_ENGINE_API void Join() noexcept;
		
		/**
		*	\brief Separates the thread from this handle.
		*
		*	The execution will continue independently and can't be Join() anymore.
		*	This will become empty.
		*/
		SA_ENGINE_API void Detatch() noexcept;

		/**
		*	\brief \e Getter of the number of concurrent threads supported by the implementation.
		*
		*	\return hint of max number of threads supported.
		*/
		SA_ENGINE_API static uint32 HardwareConcurrency();

		/**
		*	\brief Allow other threads to run.
		*
		*	Hint to the implementation to reschedule the execution of
		*	threads to allow other threads to run.
		*	See more documentation: https://en.cppreference.com/w/cpp/thread/yield
		*/
		SA_ENGINE_API static void Yield();


		/**
		*	\brief \b Default \e move operator=.
		*
		*	\return this instance.
		*/
		Thread& operator=(Thread&&) = default;

		/**
		*	\brief \b Deleted \e copy operator=.
		*
		*	\return this instance.
		*/
		Thread& operator=(const Thread&) = delete;
	};


	/** \} */
}

#include <Core/Thread/Thread.inl>

#endif // GUARD
