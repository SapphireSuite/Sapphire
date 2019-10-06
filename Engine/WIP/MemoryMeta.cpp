#include <Sapphire/Core/Debug/Debug.hpp>

#include <Sapphire/Core/Utils/Algorithms.hpp>

#include <Sapphire/Core/Memory/MemoryMeta.hpp>

namespace Sa
{
	/// \cond Internal
	namespace Internal
	{
		MemoryMeta::MemoryMeta(uint64 size, bool isFree) noexcept : mMask{ isFree ? size | sAllocFlag : size }
		{
			if(isFree)
				Reset();
		}


		uint64 MemoryMeta::Size() const noexcept
		{
			return mMask & ~sAllocFlag;
		}

		void MemoryMeta::SetSize(uint64 newSize) noexcept
		{
			mMask = IsFree() ? newSize | sAllocFlag : newSize;
		}


		MemoryMeta* MemoryMeta::Next() const noexcept
		{
			return reinterpret_cast<MemoryMeta*>(Data() + Size());
		}

		bool MemoryMeta::IsFree() const noexcept
		{
			return mMask & sAllocFlag;
		}

		void MemoryMeta::SetIsFree(bool isFree) noexcept
		{
			mMask = isFree ? mMask | sAllocFlag : Size();
		}


		MemoryMeta* MemoryMeta::PrevFree() const
		{
#if SA_CORE_MEMORY_SAFE
			SA_ASSERT(IsFree(), "MemoryMeta::PrevFree(): MemoryMeta is not a free-meta (holding a free block)!", LogChanFlag::Memory);
#endif

			return mPrevFree;
		}

		void  MemoryMeta::SetPrevFree(MemoryMeta* newLowFree)
		{
#if SA_CORE_MEMORY_SAFE
			SA_ASSERT(IsFree(), "MemoryMeta::SetPrevFree(MemoryMeta*): MemoryMeta is not a free-meta (holding a free block)!", LogChanFlag::Memory);
#endif

			mPrevFree = newLowFree;
		}

		MemoryMeta* MemoryMeta::LowFree() const
		{
#if SA_CORE_MEMORY_SAFE
			SA_ASSERT(IsFree(), "MemoryMeta::LowFree(): MemoryMeta is not a free-meta (holding a free block)!", LogChanFlag::Memory);
#endif

			return mLowFree;
		}

		void MemoryMeta::SetLowFree(MemoryMeta* newLowFree)
		{
#if SA_CORE_MEMORY_SAFE
			SA_ASSERT(IsFree(), "MemoryMeta::SetLowFree(MemoryMeta*): MemoryMeta is not a free-meta (holding a free block)!", LogChanFlag::Memory);
#endif

			mLowFree = newLowFree;

			if (mLowFree)
				mLowFree->mPrevFree = this;
		}

		MemoryMeta* MemoryMeta::HighFree() const
		{
#if SA_CORE_MEMORY_SAFE
			SA_ASSERT(IsFree(), "MemoryMeta::HighFree(): MemoryMeta is not a free-meta (holding a free block)!", LogChanFlag::Memory);
#endif

			return mHighFree;
		}

		void MemoryMeta::SetHighFree(MemoryMeta* newHighFree)
		{
#if SA_CORE_MEMORY_SAFE
			SA_ASSERT(IsFree(), "MemoryMeta::SetHighFree(MemoryMeta*): MemoryMeta is not a free-meta (holding a free block)!", LogChanFlag::Memory);
#endif

			mHighFree = newHighFree;

			if (mHighFree)
				mHighFree->mPrevFree = this;
		}

		MetaWeight MemoryMeta::Weight() const noexcept
		{
			return (MetaWeight)((mLowFree != nullptr) + (mHighFree != nullptr));
		}


		void MemoryMeta::Reset() noexcept
		{
			MemReset(Data<char*>(), MemoryMeta::minBlockSize);
		}
	}
	/// \endcond Internal
}