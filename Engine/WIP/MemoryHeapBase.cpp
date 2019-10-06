#include <Sapphire/Core/Debug/Debug.hpp>

#include <Sapphire/Core/Memory/MemoryHeapBase.hpp>

namespace Sa
{
	MemoryHeapBase::MemoryHeapBase(uint64 size, uint32 count) :
		mMain{ size + Meta::metaSize, count }, // Add first meta size
		mBTree{ new(reinterpret_cast<void*>(mMain.startAddr)) Meta(size, true) }
	{
	}


	bool MemoryHeapBase::IsInHeap(uint64 address) const noexcept
	{
		if (mMain.IsInHeapSegment(address))
			return true;

		for (const HSegment* currSeg = mMain.SubHeapSegment(); currSeg != nullptr; currSeg = currSeg->SubHeapSegment())
		{
			if (currSeg->IsInHeapSegment(address))
				return true;
		}

		return false;
	}

	bool MemoryHeapBase::IsInHeap(Meta* meta) const noexcept
	{
		// private function: ptr null check in parent call.

		if (mMain.IsInHeapSegment(meta))
			return true;

		for (const HSegment* currSeg = mMain.SubHeapSegment(); currSeg != nullptr; currSeg = currSeg->SubHeapSegment())
		{
			if (currSeg->IsInHeapSegment(meta))
				return true;
		}

		return false;
	}


	MemoryHeapBase::Meta* MemoryHeapBase::CreateSubHeapSegment(uint64 size)
	{
		SA_LOG("MemoryHeapBase: MemoryHeapBase is full, creating sub-heap segment.", LogLvlFlag::Warning, LogChanFlag::Memory);

		if (size < sDefaultMemoryHeapSize)
			size = sDefaultMemoryHeapSize;

		HSegment* subHeapSegment = mMain.CreateSubHeapSegment(size + Meta::metaSize); // Add first meta size

		Meta* meta = new(reinterpret_cast<void*>(subHeapSegment->startAddr)) Meta(size, true);
		mBTree.Add(meta);

		return meta;
	}

	MemoryHeapBase::Meta* MemoryHeapBase::SplitBlock(Meta* meta, uint64 size) noexcept
	{
		// private function: meta IsFree checked in parent call.

		uint64 prevSize = meta->Size();

		// if block too small, don't split. Let empty space for realloc.
		if (prevSize - size < Meta::metaSize + Meta::minBlockSize)
			return meta;

		meta->SetSize(size);

		Meta* freeMeta = new(meta->Data<char*>() + size) Meta(prevSize - Meta::metaSize - size, true);

		// Init new free meta.
		freeMeta->prev = meta;

		Meta* next = freeMeta->Next();

		if (IsInHeap(next))
			next->prev = freeMeta;
		//

		mBTree.Add(freeMeta);

		return meta;
	}

	MemoryHeapBase::Meta* MemoryHeapBase::MergeBlock(Meta* meta) noexcept
	{
		// private function: meta null checked in parent call.

		Meta* prev = meta->prev;
		Meta* next = meta->Next();

		if (!IsInHeap(next))
			next = nullptr;

		if (prev && prev->IsFree())
		{
			uint64 newSize = prev->Size() + Meta::metaSize + meta->Size();

			meta = mBTree.Pop(prev);
			meta->SetSize(newSize);

			if (next)
				next->prev = meta;
		}

		if (next && next->IsFree())
		{
			Meta* nextNext = next->Next();

			if (IsInHeap(nextNext))
				nextNext->prev = meta;

			uint64 newSize = meta->Size() + Meta::metaSize + next->Size();

			mBTree.Pop(next);
			meta->Reset();
			meta->SetSize(newSize);
		}

		mBTree.Add(meta);
		return meta;
	}


	MemoryHeapBase::Meta* MemoryHeapBase::GetMetaFromPtr(const void* ptr) noexcept
	{
		// private function: ptr null check in parent call.

		return reinterpret_cast<Meta*>(reinterpret_cast<uint64>(ptr) - Meta::metaSize);
	}


	uint64 MemoryHeapBase::Capacity(const void* ptr) noexcept
	{
#if SA_CORE_MEMORY_SAFE
		SA_ASSERT(ptr, "MemoryHeapBase::GetCapacity(const void*): Invalid ptr: instance nullptr!", LogChanFlag::Memory);
#endif

		// Get size of block.
		return GetMetaFromPtr(ptr)->Size();
	}
}