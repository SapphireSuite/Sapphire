#pragma once

#ifndef SAPPHIRE_CORE_MEMORY_META_GUARD
#define SAPPHIRE_CORE_MEMORY_META_GUARD

#include <Sapphire/Core/Types/Int.hpp>

namespace Sa
{
	/// \cond Internal
	/**
	*	\file MemoryMeta.hpp
	*
	*	\brief \b Definition of Sapphire's Internal <b>Memory Meta</b> class.
	*
	*	\ingroup Memory
	*	\{
	*/


	namespace Internal
	{
		/**
		*	\brief Meta weight values.
		*
		*	Weight is +1 for each child of the current Meta.
		*/
		enum class MetaWeight : uint8
		{
			/// No child
			None = 0,

			/// One child
			One = 1,

			/// Full (ie 2 children) 
			Full = 2
		};


		/**
		*	\brief \b Meta-data which contain infos on the current held memory block in heap.
		*/
		class MemoryMeta
		{
			/// The bit-flag in meta mask to check a block \e free-ness.
			static constexpr const uint64 sAllocFlag = uint64(1) << 63;

			/**
			*	\brief Mask containing 1 bit for <b>block free-ness</b> and the \b size of the held block.
			*
			*	Ex: 1xxx xxxx...:
			*	1 reprents the first bit to check is Alloc (0) or Free (1).
			*	xxx xxxx... represents the other bits, containing the size of the held block.
			*/
			uint64 mMask = 0u;

		public:

			///	Pointer to the previous meta (heap based).
			MemoryMeta* prev = nullptr;

		public:
			/// The minimal size of a meta (ie holding non free block).
			static constexpr const uint32 metaSize = sizeof(uint64) + sizeof(MemoryMeta*);

			/// The minimal block size. Reserve space for free meta behaviour.
			static constexpr const uint32 minBlockSize = 3 * sizeof(MemoryMeta*);


			/**
			*	\brief \b Value constructor.
			*
			*	\param[in] size The size of the block.
			*	\param[in] isFree Is holding a free block.
			*/
			MemoryMeta(uint64 size, bool isFree = false) noexcept;


			/**
			*	\e Getter of the <b>starting address</b> of the held block.
			*
			*	\tparam T The return type (auto cast).
			*
			*	\return The starting address of the held block as T.
			*/
			template <typename T = uint64>
			T Data() const noexcept
			{
				return reinterpret_cast<T>(reinterpret_cast<uint64>(this) + metaSize);
			}

			/**
			*	\brief \e Getter of the \b size of the held block.
			*
			*	\return The size of the held block.
			*/
			uint64 Size() const noexcept;

			/**
			*	\brief \e Setter of the \b size of the held block.
			*
			*	\param[in] newSize The new size of the block.
			*/
			void SetSize(uint64 newSize) noexcept;

			/**
			*	\brief \e Getter of the next block (heap based).
			*/
			MemoryMeta* Next() const noexcept;

			/**
			*	\e Getter of the <b>free-ness bit-flag</b> of the held block.
			*
			*	\return true if the block is free, otherwise false.
			*/
			bool IsFree() const noexcept;

			/**
			*	\e Setter of the <b>free-ness bit-flag</b> of the held block.
			*
			*	\param[in] isFree value of the bit-flag.
			*/
			void SetIsFree(bool isFree) noexcept;

		private:
			/* Free-meta only fields and properties (holding free block) */

			/**
			*	\brief Pointer to the prev meta holding a free block (binary tree based).
			*
			*	This data is available only while this meta is holding a <b>free block</b>.
			*/
			MemoryMeta* mPrevFree = nullptr;

			/**
			*	\brief Pointer to the low-sized meta holding a free block (binary tree based).
			*
			*	This data is available only while this meta is holding a <b>free block</b>.
			*/
			MemoryMeta* mLowFree = nullptr;

			/**
			*	\brief Pointer to the high-sized meta holding a free block (binary tree based).
			*
			*	This data is available only while this meta is holding a <b>free block</b>.
			*/
			MemoryMeta* mHighFree = nullptr;

		public:

			/**
			*	\e Getter of the <b>prev free-meta</b> (binary tree).
			*
			*	Available only while holding a free block, either throw an exeption.
			*/
			MemoryMeta* PrevFree() const;

			/**
			*	\e Setter of the <b>prev free-meta</b> (binary tree).
			*
			*	Available only while holding a free block, either throw an exeption.
			*/
			void SetPrevFree(MemoryMeta* newLowFree);

			/**
			*	\e Getter of the <b>low-sized free-meta</b> (binary tree).
			*
			*	Available only while holding a free block, either throw an exeption.
			*/
			MemoryMeta* LowFree() const;

			/**
			*	\e Setter of the <b>low-sized free-meta</b> (binary tree).
			*
			*	Available only while holding a free block, either throw an exeption.
			*/
			void SetLowFree(MemoryMeta* newLowFree);

			/**
			*	\e Getter of the <b>high-sized free-meta</b> (binary tree).
			*
			*	Available only while holding a free block, either throw an exeption.
			*/
			MemoryMeta* HighFree() const;

			/**
			*	\e Setter of the <b>high-sized free-meta</b> (binary tree).
			*
			*	Available only while holding a free block, either throw an exeption.
			*/
			void SetHighFree(MemoryMeta* newHighFree);

			/**
			*	\e Getter of the weight of the meta node in the binary tree.
			*
			*	Weight is +1 for each non-null free meta child (mLowFree and mHighFree).
			*	Available only while holding a free block, either throw an exeption.
			*
			*	\return The weight of the meta.
			*/
			MetaWeight Weight() const noexcept;


			/**
			*	\brief \e Reset free-meta properties to null.
			*/
			void Reset() noexcept;
		};
	}


	/** \} */
	/// \endcond Internal
}

#endif // GUARD