#include <new>

#include <Sapphire/Core/Debug/Debug.hpp>

#include <Sapphire/Core/Memory/MemoryAlgorithms.hpp>
#include <Sapphire/Core/Memory/MemoryMeta.hpp>
#include <Sapphire/Core/Memory/MemoryBTree.hpp>
#include <Sapphire/Core/Memory/MemoryHeapSegment.hpp>

namespace Sa
{
	/// \cond Internal
	namespace Internal
	{
		MemoryHeapSegment::MemoryHeapSegment(uint64 size, uint32 count) :
			startAddr{ Init(size, count) },
			endAddr{ startAddr + size }
		{
		}

		MemoryHeapSegment::~MemoryHeapSegment() noexcept
		{
			::operator delete(reinterpret_cast<void*>(startAddr));

			if (mSubHeapSegment != nullptr)
				delete mSubHeapSegment;
		}


		uint64 MemoryHeapSegment::Init(uint64& size, uint32 count)
		{
			// Compute total heap size.
			size = size < MemoryMeta::metaSize + MemoryMeta::minBlockSize ? MemoryMeta::metaSize + MemoryMeta::minBlockSize : Align8(size);
			size *= count;

			void* ptr = ::operator new(size);

			SA_ASSERT(ptr, "MemoryHeapSegment::Init(uint64, uint32): Allocation Failed!", LogChanFlag::Memory);

			return reinterpret_cast<uint64>(ptr);
		}


		MemoryHeapSegment* MemoryHeapSegment::SubHeapSegment() const noexcept
		{
			return mSubHeapSegment;
		}

		bool MemoryHeapSegment::IsInHeapSegment(uint64 address) const noexcept
		{
			return startAddr + MemoryMeta::metaSize <= address && address <= endAddr - MemoryMeta::minBlockSize;
		}

		bool MemoryHeapSegment::IsInHeapSegment(MemoryMeta* meta) const noexcept
		{
			return startAddr <= reinterpret_cast<uint64>(meta) && reinterpret_cast<uint64>(meta) <= endAddr - MemoryMeta::minBlockSize - MemoryMeta::metaSize;
		}

		
		MemoryHeapSegment* MemoryHeapSegment::CreateSubHeapSegment(uint64 size)
		{
			if (mSubHeapSegment == nullptr)
			{
				mSubHeapSegment = new MemoryHeapSegment(size, 1u);
				return mSubHeapSegment;
			}

			return mSubHeapSegment->CreateSubHeapSegment(size);
		}
	}
	/// \endcond Internal
}