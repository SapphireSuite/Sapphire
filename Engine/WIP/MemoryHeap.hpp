#pragma once

#ifndef SAPPHIRE_CORE_MEMORY_HEAP_GUARD
#define SAPPHIRE_CORE_MEMORY_HEAP_GUARD

#include <Sapphire/Core/Thread/Mutex.hpp>
#include <Sapphire/Core/Thread/ThreadMode.hpp>

#include <Sapphire/Core/Memory/MemoryHeapBase.hpp>

namespace Sa
{
	/**
	*	\file MemoryHeap.hpp
	*
	*	\brief \b Definition of Sapphire's <b>Memory Heap</b> class.
	*
	*	\ingroup Memory
	*	\{
	*/


	/**
	*	\brief <b>Memory management</b> class which provide optimized allocation, reallocation and free of <b>raw memory</b>.
	*
	*	\tparam thMode The thread mode.
	*/
	template <ThreadMode thMode = ThreadMode::Unsafe>
	class MemoryHeap;


	/**
	*	\brief MemoryHeap thread-unsafe implementation.
	*/
	template <>
	class MemoryHeap<ThreadMode::Unsafe> final : public MemoryHeapBase
	{
	public:
		using MemoryHeapBase::MemoryHeapBase;

		SA_CORE_API void* Alloc(uint64 size) override;

		SA_CORE_API void* Realloc(void* ptr, uint64 size) override;

		SA_CORE_API void Free(void* ptr) override;
	};


	/**
	*	\brief MemoryHeap thread-safe implementation.
	*/
	template <>
	class MemoryHeap<ThreadMode::Safe> final : public MemoryHeapBase
	{
		/// Mutex to ensure thread-safe utilization.
		Mutex mMutex;

	public:
		using MemoryHeapBase::MemoryHeapBase;

		SA_CORE_API void* Alloc(uint64 size) override;

		SA_CORE_API void* Realloc(void* ptr, uint64 size) override;

		SA_CORE_API void Free(void* ptr) override;
	};


	/** \} */
}

#endif // GUARD