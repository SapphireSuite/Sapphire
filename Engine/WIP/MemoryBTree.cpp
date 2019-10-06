#include <Sapphire/Core/Debug/Debug.hpp>

#include <Sapphire/Core/Memory/MemoryBTree.hpp>

namespace Sa
{
	/// \cond Internal
	namespace Internal
	{
		MemoryBTree::MemoryBTree(MemoryMeta* root) : mRoot{ root }
		{
		}


		void MemoryBTree::Add(MemoryMeta* meta) noexcept
		{
			meta->SetIsFree(true);

			if (!mRoot)
			{
				mRoot = meta;
				return;
			}

			uint32 metaSize = meta->Size();

			MemoryMeta* it = mRoot;
			uint64 itSize = it->Size();

			while (true)
			{
				MemoryMeta* next = metaSize < itSize ? it->LowFree() : it->HighFree();

				// Find location to add.
				if (next == nullptr)
					break;

				it = next;
				itSize = it->Size();
			}

			// Add.
			if (metaSize < itSize)
				it->SetLowFree(meta);
			else
				it->SetHighFree(meta);

			Balance(it);
		}

		MemoryMeta* MemoryBTree::Pop(MemoryMeta* meta) noexcept
		{
			uint64 metaSize = meta->Size();
			MetaWeight metaWeight = meta->Weight();
			MemoryMeta* parent = meta->PrevFree();

			if (metaWeight == MetaWeight::Full)
			{
				MemoryMeta* low = meta->LowFree();
				MetaWeight lowWeight = low->Weight();

				MemoryMeta* high = meta->HighFree();
				MetaWeight highWeight = high->Weight();


				if (lowWeight == MetaWeight::Full || highWeight == MetaWeight::Full)	// Almost one child has 2 children.
				{
					MemoryMeta* big = lowWeight < highWeight ? low : high;
					MemoryMeta* small = lowWeight < highWeight ? high : low;


					// Tree root ?
					if (parent == nullptr)
					{
						mRoot = small;
						mRoot->SetPrevFree(nullptr);
					}
					else
						parent->SetHighFree(small);
					
					Balance(small);

					AddSubTree(big);
				}
				else																	// One chil is bigger than the other: Set + balance parent.
				{
					MemoryMeta* big = highWeight == MetaWeight::Full ? high : low;
					MemoryMeta* small = highWeight == MetaWeight::Full ? low : high;

					// Tree root ?
					if (parent == nullptr)
					{
						mRoot = small;
						mRoot->SetPrevFree(nullptr);
					}
					else if(metaSize < parent->Size())
						parent->SetLowFree(small);
					else
						parent->SetHighFree(small);

					small->SetLowFree(big);
					Balance(small);
				}
			}
			else																		// One or no children.
			{
				if (parent == nullptr)
				{
					if (metaWeight == MetaWeight::One)
					{
						mRoot = meta->LowFree() ? meta->LowFree() : meta->HighFree();
						mRoot->SetPrevFree(nullptr);
						Balance(mRoot);
					}
					else
						mRoot = nullptr;
				}
				else if (metaSize < parent->Size()) // Set parent new children + balance.
				{
					parent->SetLowFree(nullptr);
					Balance(parent);
				}
				else
				{
					parent->SetHighFree(nullptr);
					Balance(parent);
				}
			}

			meta->SetIsFree(false);
			return meta;
		}

		MemoryMeta* MemoryBTree::Pop(uint64 size) noexcept
		{
			MemoryMeta* meta = Find(size);

			return meta ? Pop(meta) : nullptr;
		}

		void MemoryBTree::AddSubTree(MemoryMeta* meta) noexcept
		{
			MemoryMeta* child = meta->LowFree();

			if (child)
				AddSubTree(child);

			child = meta->HighFree();

			if (child)
				AddSubTree(child);

			Add(meta);
		}

		MemoryMeta* MemoryBTree::Find(uint64 size) const noexcept
		{
			if (mRoot == nullptr)
				return nullptr;

			MemoryMeta* it = mRoot;
			uint64 itSize = it->Size();

			while (size != itSize)
			{
				MemoryMeta* next = size < itSize ? it->LowFree() : it->HighFree();

				if (next == nullptr)
				{
					it = size < itSize ? it : it->PrevFree();
					break;
				}

				it = next;
				itSize = it->Size();
			}

			return it;
		}

		MemoryMeta* MemoryBTree::Swap(MemoryMeta* meta) noexcept
		{
			MemoryMeta* parent = meta->PrevFree();

#if SA_CORE_MEMORY_SAFE
			SA_ASSERT(meta->PrevFree(), "MemoryBTree::Swap(MemoryMeta*): MemoryMeta must have a parent!", LogChanFlag::Memory);
#endif

			uint64 metaSize = meta->Size();
			MemoryMeta* grandParent = parent->PrevFree();

			// Set grandParent new child.
			if (grandParent == nullptr)
				mRoot = meta;
			else if (grandParent->Size() < metaSize)
				grandParent->SetHighFree(meta);
			else
				grandParent->SetLowFree(meta);

			// Swap parent and child.
			if (parent->Size() < metaSize)
			{
				parent->SetHighFree(meta->LowFree());
				meta->SetLowFree(parent);
			}
			else
			{
				parent->SetLowFree(meta->HighFree());
				meta->SetHighFree(parent);
			}

			return meta;
		}

		void MemoryBTree::Balance(MemoryMeta* meta) noexcept
		{
			MemoryMeta* low = meta->LowFree();
			MemoryMeta* high = meta->HighFree();
			MetaWeight metaWeight = meta->Weight();

			if (high && metaWeight < high->Weight())
				meta = Swap(high);
			else if (low && metaWeight < low->Weight())
				meta = Swap(low);

			MemoryMeta* parent = meta->PrevFree();

			// Weight may have change du to previous swap.
			if(parent && parent->Weight() <= meta->Weight())
				Swap(meta);
		}
	}
	///\endcond Internal
}