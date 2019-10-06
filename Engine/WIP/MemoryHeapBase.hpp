#pragma once

#ifndef SAPPHIRE_CORE_MEMORY_HEAP_BASE_GUARD
#define SAPPHIRE_CORE_MEMORY_HEAP_BASE_GUARD

#include <Sapphire/Core/Memory/MemoryBTree.hpp>
#include <Sapphire/Core/Memory/MemoryHeapSegment.hpp>

namespace Sa
{
	/**
	*	\file MemoryHeapBase.hpp
	*
	*	\brief \b Definition of Sapphire's <b>Memory Heap Base</b> class.
	*
	*	\ingroup Memory
	*	\{
	*/


	/**
	*	\brief <b>Memory management base</b> class which provide optimized allocation, reallocation and free of <b>raw memory</b>.
	*/
	class MemoryHeapBase
	{
	protected:

		/// Internal using.
		using Meta = Internal::MemoryMeta;

		/// Internal using.
		using BTree = Internal::MemoryBTree;

		/// Internal using.
		using HSegment = Internal::MemoryHeapSegment;

		/// The main heap segment.
		HSegment mMain;

		/// The free-meta binary tree.
		BTree mBTree;


		/// The default heap size.
		static constexpr const uint64 sDefaultMemoryHeapSize = 2048u;

		/**
		*	\brief \e Check if an address is in heap or sub-heap range.
		*
		*	\return true if the address is in heap, otherwise false.
		*/
		bool IsInHeap(uint64 address) const noexcept;

		/**
		*	\brief \e Check if a meta is in heap or sub-heap range.
		*
		*	\return true if the meta is in heap, otherwise false.
		*/
		bool IsInHeap(Meta* meta) const noexcept;


		/**
		*	\brief \e Create a sub-heap segment of size and return a new free-meta.
		*
		*	\param[in] size The size of the new sub-heap Segment.
		*
		*	\return The new free-meta created.
		*/
		Meta* CreateSubHeapSegment(uint64 size);

		/**
		*	\brief \e Split the block held by meta by size.
		*
		*	\param[in] meta The free meta with block to split.
		*	\param[in] size The size of split.
		*
		*	\return the reset meta.
		*/
		Meta* SplitBlock(Meta* meta, uint64 size) noexcept;

		/**
		*	\brief \e Merge free blocks around meta.
		*
		*	\param[in] meta The meta to merge around.
		*
		*	\return The new free meta merged.
		*/
		Meta* MergeBlock(Meta* meta) noexcept;


		/**
		*	\brief \e Getter of a meta from an allocated block.
		*/
		static Meta* GetMetaFromPtr(const void* ptr) noexcept;

	public:
		/**
		*	\brief \b Create a memory heap of size * count (octet).
		*
		*	\param[in] size The size of the heap.
		*	\param[in] count The size multiplier.
		*/
		SA_CORE_API MemoryHeapBase(uint64 size = sDefaultMemoryHeapSize, uint32 count = 1u);

		/**
		*	\brief \b Deleted \e move constructor.
		*/
		MemoryHeapBase(MemoryHeapBase&&) = delete;

		/**
		*	\brief \b Deleted \e copy constructor.
		*/
		MemoryHeapBase(const MemoryHeapBase&) = delete;


		/**
		*	\brief \e Allocate a <b>raw memory</b> block of size (octet).
		*
		*	\param[in] size The size of memory to allcate (octet).
		*
		*	\return Pointer to allocated raw memory.
		*/
		virtual void* Alloc(uint64 size) = 0;

		/**
		*	\brief \e Reallocate a <b>raw memory</b> block of size (octet).
		*
		*	\param[in] ptr The ptr to reallocate.
		*	\param[in] size The size of memory to reallocate (octet).
		*
		*	\return Pointer to reallocated raw memory.
		*/
		virtual void* Realloc(void* ptr, uint64 size) = 0;

		/**
		*	\brief \e Free a <b>raw memory</b> block allocated from this heap.
		*
		*	\param[in] ptr The ptr to free.
		*/
		virtual void Free(void* ptr) = 0;


		/**
		*	\e Getter of ptr's \b capacity.
		*
		*	Undefined behaviour if pointer hasn't been allocated by a MemoryHeap.
		*
		*	\return The ptr's capacity.
		*/
		SA_CORE_API static uint64 Capacity(const void* ptr) noexcept;


		/**
		*	\brief \b Deleted \e move operator=.
		*
		*	\return this instance.
		*/
		MemoryHeapBase& operator=(MemoryHeapBase&&) = delete;

		/**
		*	\brief \b Deleted \e copy operator=.
		*
		*	\return this instance.
		*/
		MemoryHeapBase& operator=(const MemoryHeapBase&) = delete;
	};


	/** \} */
}

#endif // GUARD