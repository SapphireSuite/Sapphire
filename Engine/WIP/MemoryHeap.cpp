#include <Sapphire/Core/Debug/Debug.hpp>

#include <Sapphire/Core/Utils/Algorithms.hpp>

#include <Sapphire/Core/Memory/MemoryAlgorithms.hpp>
#include <Sapphire/Core/Memory/MemoryHeap.hpp>

namespace Sa
{
	void* MemoryHeap<ThreadMode::Unsafe>::Alloc(uint64 size)
	{
#if SA_CORE_MEMORY_SAFE
		SA_ASSERT(size != 0, "void* MemoryHeap<ThreadMode::Unsafe>::Alloc(uint64): Invalid argument: Alloc size == 0!", LogChanFlag::Memory);
#endif

		size = size < Internal::MemoryMeta::minBlockSize ? Internal::MemoryMeta::minBlockSize : Align8(size);

		Internal::MemoryMeta* meta = mBTree.Pop(size);

		if (meta == nullptr)
			meta = CreateSubHeapSegment(size * 2.0f);

		return  SplitBlock(meta, size)->Data<void*>();
	}

	void* MemoryHeap<ThreadMode::Unsafe>::Realloc(void* ptr, uint64 size)
	{
#if SA_CORE_MEMORY_SAFE
		SA_ASSERT(ptr, "void* MemoryHeap<ThreadMode::Unsafe>::Realloc(void*, uint64): Invalid ptr: instance nullptr!", LogChanFlag::Memory);
		SA_ASSERT(IsInHeap(reinterpret_cast<uint64>(ptr)), "void* MemoryHeap<ThreadMode::Unsafe>::Realloc(void*, uint64): Invalid ptr: not in heap!", LogChanFlag::Memory);
#endif

		Meta* meta = GetMetaFromPtr(ptr);
		size = size < Meta::minBlockSize ? Meta::minBlockSize : Align8(size);

		uint64 blockSize = meta->Size();

		// if enough space.
		if (blockSize >= size)
			return ptr;


		// Reallocate using next block.

		Meta* next = meta->Next();

		if (!IsInHeap(next))
			next = nullptr;

		if (next && next->IsFree())
		{
			Meta* nextNext = next->Next();

			uint64 nextSize = next->Size();
			next = mBTree.Pop(next);

			uint64 splitSize = blockSize + nextSize - size;
			if (splitSize >= Meta::minBlockSize) // totalSize - size < minSize, let empty space, else split.
			{
				Meta* newFreeMeta = new(meta->Data<char*>() + size) Meta(splitSize, true);
				newFreeMeta->prev = meta;
				mBTree.Add(newFreeMeta);

				if (IsInHeap(nextNext))
					nextNext->prev = newFreeMeta;
			}
			else if (IsInHeap(nextNext))
				nextNext->prev = meta;

			meta->SetSize(size);

			return meta->Data<void*>();
		}


		// Reallocate using prev block.

		Meta* prev = meta->prev;

		if (prev && prev->IsFree())
		{
			uint64 prevSize = prev->Size();
			prev = mBTree.Pop(prev);

			MemMove(prev->Data<char*>(), reinterpret_cast<char*>(ptr), blockSize);

			uint64 splitSize = blockSize + prevSize - size;
			meta = prev;

			if (splitSize >= Meta::minBlockSize) // totalSize - size < minSize, let empty space, else split.
			{
				Meta* newFreeMeta = new(meta->Data<char*>() + size) Meta(splitSize, true);
				newFreeMeta->prev = meta;
				mBTree.Add(newFreeMeta);

				if (next)
					next->prev = newFreeMeta;
			}
			else if (next)
				next->prev = prev;

			meta->SetSize(size);

			return meta->Data<void*>();
		}

		// else Alloc new, move and free old.
		void* newPtr = Alloc(size);

		MemMove(reinterpret_cast<char*>(newPtr), reinterpret_cast<char*>(ptr), blockSize);

		Free(ptr);

		return newPtr;
	}

	void MemoryHeap<ThreadMode::Unsafe>::Free(void* ptr)
	{
#if SA_CORE_MEMORY_SAFE
		SA_ASSERT(ptr, "MemoryHeap<ThreadMode::Unsafe>::Free(void*): Invalid ptr: instance nullptr!", LogChanFlag::Memory);
#endif

		Meta* meta = GetMetaFromPtr(ptr);

#if SA_CORE_MEMORY_SAFE
		SA_ASSERT(IsInHeap(meta), "MemoryHeap<ThreadMode::Unsafe>::Free(void*): Invalid ptr: not in heap!", LogChanFlag::Memory);
#endif
		MergeBlock(meta);
	}


	void* MemoryHeap<ThreadMode::Safe>::Alloc(uint64 size)
	{
#if SA_CORE_MEMORY_SAFE
		SA_ASSERT(size != 0, "void* MemoryHeap<ThreadMode::Safe>::Alloc(uint64): Invalid argument: Alloc size == 0!", LogChanFlag::Memory);
#endif

		size = size < Internal::MemoryMeta::minBlockSize ? Internal::MemoryMeta::minBlockSize : Align8(size);

		RAII<Mutex> locker(mMutex);

		Internal::MemoryMeta* meta = mBTree.Pop(size);

		if (meta == nullptr)
			meta = CreateSubHeapSegment(size * 2.0f);

		return  SplitBlock(meta, size)->Data<void*>();
	}

	void* MemoryHeap<ThreadMode::Safe>::Realloc(void* ptr, uint64 size)
	{
#if SA_CORE_MEMORY_SAFE
		SA_ASSERT(ptr, "void* MemoryHeap<ThreadMode::Safe>::Realloc(void*, uint64): Invalid ptr: instance nullptr!", LogChanFlag::Memory);
		SA_ASSERT(IsInHeap(reinterpret_cast<uint64>(ptr)), "void* MemoryHeap<ThreadMode::Safe>::Realloc(void*, uint64): Invalid ptr: not in heap!", LogChanFlag::Memory);
#endif

		Meta* meta = GetMetaFromPtr(ptr);
		size = size < Meta::minBlockSize ? Meta::minBlockSize : Align8(size);

		uint64 blockSize = meta->Size();

		// if enough space.
		if (blockSize >= size)
			return ptr;


		// Reallocate using next block.

		Meta* next = meta->Next();

		mMutex.Lock();

		if (!IsInHeap(next))
			next = nullptr;

		if (next && next->IsFree())
		{
			Meta* nextNext = next->Next();

			uint64 nextSize = next->Size();
			next = mBTree.Pop(next);

			uint64 splitSize = blockSize + nextSize - size;
			if (splitSize >= Meta::minBlockSize) // totalSize - size < minSize, let empty space, else split.
			{
				Meta* newFreeMeta = new(meta->Data<char*>() + size) Meta(splitSize, true);
				newFreeMeta->prev = meta;
				mBTree.Add(newFreeMeta);

				if (IsInHeap(nextNext))
					nextNext->prev = newFreeMeta;
			}
			else if (IsInHeap(nextNext))
				nextNext->prev = meta;

			meta->SetSize(size);

			mMutex.Unlock();

			return meta->Data<void*>();
		}


		// Reallocate using prev block.

		Meta* prev = meta->prev;

		if (prev && prev->IsFree())
		{
			uint64 prevSize = prev->Size();
			prev = mBTree.Pop(prev);

			MemMove(prev->Data<char*>(), reinterpret_cast<char*>(ptr), blockSize);

			uint64 splitSize = blockSize + prevSize - size;
			meta = prev;

			if (splitSize >= Meta::minBlockSize) // totalSize - size < minSize, let empty space, else split.
			{
				Meta* newFreeMeta = new(meta->Data<char*>() + size) Meta(splitSize, true);
				newFreeMeta->prev = meta;
				mBTree.Add(newFreeMeta);

				if (next)
					next->prev = newFreeMeta;
			}
			else if (next)
				next->prev = prev;

			meta->SetSize(size);

			mMutex.Unlock();

			return meta->Data<void*>();
		}

		mMutex.Unlock();

		// else Alloc new, move and free old.
		void* newPtr = Alloc(size);

		MemMove(reinterpret_cast<char*>(newPtr), reinterpret_cast<char*>(ptr), blockSize);

		Free(ptr);

		return newPtr;
	}

	void MemoryHeap<ThreadMode::Safe>::Free(void* ptr)
	{
#if SA_CORE_MEMORY_SAFE
		SA_ASSERT(ptr, "MemoryHeap<ThreadMode::Safe>::Free(void*): Invalid ptr: instance nullptr!", LogChanFlag::Memory);
#endif

		Meta* meta = GetMetaFromPtr(ptr);

#if SA_CORE_MEMORY_SAFE
		SA_ASSERT(IsInHeap(meta), "MemoryHeap<ThreadMode::Safe>::Free(void*): Invalid ptr: not in heap!", LogChanFlag::Memory);
#endif
		mMutex.Lock();

		MergeBlock(meta);

		mMutex.Unlock();
	}
}