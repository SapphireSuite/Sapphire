#pragma once

#ifndef SAPPHIRE_CORE_MEMORY_BTREE_GUARD
#define SAPPHIRE_CORE_MEMORY_BTREE_GUARD

#include <Sapphire/Core/Memory/MemoryMeta.hpp>

namespace Sa
{
	/// \cond Internal
	/**
	*	\file MemoryBTree.hpp
	*
	*	\brief \b Definition of Sapphire's Internal <b>Memory Binary Tree</b> class.
	*
	*	\ingroup Memory
	*	\{
	*/


	namespace Internal
	{
		/**
		*	\brief \b Binary-Tree class to optimize \b free-meta operations.
		*/
		class MemoryBTree
		{
			/// The free-meta root of the tree.
			MemoryMeta* mRoot;


			/**
			*	\brief \b Add a sub-tree of free-meta to the tree.
			*
			*	\param[in] meta The root of the sub-tree.
			*/
			void AddSubTree(MemoryMeta* meta) noexcept;

			/**
			*	\brief \b Find a free-meta of size in the tree.
			*
			*	\param[in] size The free-meta of size to find.
			*
			*	\return The free-meta with equal or closest superior size.
			*/
			MemoryMeta* Find(uint64 size) const noexcept;

			/**
			*	\brief \b Swap free-meta with his parent to keep the binary tree \b balanced.
			*
			*	\param[in] meta The free meta to swap.
			*
			*	\return The free-meta initialized (same as parameter).
			*/
			MemoryMeta* Swap(MemoryMeta* meta) noexcept;

			/**
			*	\brief Check if the tree need to be balanced around the given meta (parent + children).
			*
			*	\param[in] meta The meta to balance around.
			*/
			void Balance(MemoryMeta* meta) noexcept;

		public:

			/**
			*	\brief \b Value constructor.
			*
			*	\param[in] root The root of the tree.
			*/
			MemoryBTree(MemoryMeta* root);


			/**
			*	\brief \b Add a free-meta to the tree.
			*
			*	\param[in] meta The free-meta to add.
			*/
			void Add(MemoryMeta* meta) noexcept;

			/**
			*	\brief \b Extract a free-meta from the tree.
			*
			*	\param[in] meta The free-meta to extract.
			*
			*	\return The meta initialized (same as parameter).
			*/
			MemoryMeta* Pop(MemoryMeta* meta) noexcept;

			/**
			*	\brief \b Pop a free-meta of size in the tree.
			*
			*	\param[in] size The free-meta of size to pop.
			*
			*	\return The free-meta with equal or closest superior size.
			*/
			MemoryMeta* Pop(uint64 size) noexcept;
		};
	}


	/** \} */
	/// \endcond Internal
}

#endif // GUARD