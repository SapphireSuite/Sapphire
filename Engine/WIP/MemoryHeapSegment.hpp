#pragma once

#ifndef SAPPHIRE_CORE_MEMORY_SUB_HEAP_GUARD
#define SAPPHIRE_CORE_MEMORY_SUB_HEAP_GUARD

#include <Sapphire/Core/Config.hpp>

#include <Sapphire/Core/Types/Int.hpp>

namespace Sa
{
	/// \cond Internal
	/**
	*	\file MemoryHeapSegment.hpp
	*
	*	\brief \b Definition of Sapphire's Internal <b>Memory Sub-Heap</b> class.
	*
	*	\ingroup Memory
	*	\{
	*/

	namespace Internal
	{
		class MemoryBTree;

		/**
		*/
		class MemoryHeapSegment
		{
			/**
			*	\brief sub-memory heap segment.
			*
			*	This is used when this heap segment is full but still needs to allocate.
			*	Then a sub-heap segment is created.
			*/
			MemoryHeapSegment* mSubHeapSegment = nullptr;


			/**
			*	\brief Init the current MemoryHeapSegment.
			*
			*	Called by constructor.
			*
			*	\param[in] size The size of the heap.
			*	\param[in] count The size multiplier.
			*
			*	\return the starting address of the heap (init mStartAddr).
			*/
			uint64 Init(uint64& size, uint32 count);

		public:
			/// The starting address of the heap.
			const uint64 startAddr = 0u;

			/// The ending address of the heap.
			const uint64 endAddr = 0u;

			/**
			*	\brief \b Create a memory sub-heap of size * count (octet).
			*
			*	\param[in] size The size of the heap.
			*	\param[in] count The size multiplier.
			*/
			MemoryHeapSegment(uint64 size, uint32 count);

			/**
			*	\brief \b Deleted \e move constructor.
			*/
			MemoryHeapSegment(MemoryHeapSegment&&) = delete;

			/**
			*	\brief \b Deleted \e copy constructor.
			*/
			MemoryHeapSegment(const MemoryHeapSegment&) = delete;

			/**
			*	\brief Destructor free the allocated Memory.
			*/
			SA_CORE_API ~MemoryHeapSegment() noexcept;


			/**
			*	\e Getter of the sub-heap segment.
			*
			*	\return the sub-heap segment.
			*/
			MemoryHeapSegment* SubHeapSegment() const noexcept;

			/**
			*	\brief Check if an address is in heap segment range.
			*
			*	\return true if the address is in heap, otherwise false.
			*/
			bool IsInHeapSegment(uint64 address) const noexcept;

			/**
			*	\brief Check if a meta is in heap segment range.
			*
			*	\return true if the meta is in heap, otherwise false.
			*/
			bool IsInHeapSegment(MemoryMeta* meta) const noexcept;


			/**
			*	\brief Create a sub-heap segment of size and return a new free-meta.
			*
			*	\param[in] size The size of the new sub-heap Segment.
			*
			*	\return The new free-meta created.
			*/
			MemoryHeapSegment* CreateSubHeapSegment(uint64 size);


			/**
			*	\brief \b Deleted \e move operator=.
			*
			*	\return this instance.
			*/
			MemoryHeapSegment& operator=(MemoryHeapSegment&&) = delete;

			/**
			*	\brief \b Deleted \e copy operator=.
			*
			*	\return this instance.
			*/
			MemoryHeapSegment& operator=(const MemoryHeapSegment&) = delete;
		};
	}


	/** \} */
	/// \endcond Internal
}

#endif // GUARD