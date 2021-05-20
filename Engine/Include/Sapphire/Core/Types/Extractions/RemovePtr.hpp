// Copyright 2020 Sapphire development team. All Rights Reserved.

#ifndef SAPPHIRE_CORE_REMOVE_PTR_GUARD
#define SAPPHIRE_CORE_REMOVE_PTR_GUARD

#include <Core/Types/Int.hpp>

namespace Sa
{
	/**
	*	\file Core/Types/Extractions/RemovePtr.hpp
	*
	*	\brief \b Definition of Sapphire's \b Remove Ptr template type.
	*
	*	\ingroup Types
	*	\{
	*/


	/**
	*	\brief Extract the \b non-pointer and \b non-array type of \c T.
	*
	*	Get the type \c T with removed pointer and array.
	*
	*	\tparam T	Type to extract from.
	*/
	template <typename T>
	struct RemovePtrTemplate
	{
		/// Extracted type.
		using Type = T;
	};

	/// \cond Internal
	/**
	*	\brief Extract the \b non-pointer type of \c T.
	*
	*	Get the type \c T with removed pointer.
	*
	*	\tparam T	Type to extract from.
	*
	*	\implements RemovePtrTemplate
	*/
	template <typename T>
	struct RemovePtrTemplate<T*>
	{
		/// Extracted type.
		using Type = T;
	};

	/**
	*	\brief Extract the \b non-array type of \c T.
	*
	*	Get the type \c T with removed array.
	*
	*	\tparam T	Type to extract from.
	*
	*	\implements RemovePtrTemplate
	*/
	template <typename T>
	struct RemovePtrTemplate<T[]>
	{
		/// Extracted type.
		using Type = T;
	};

	/**
	*	\brief Extract the \b non-sized-array type of \c T.
	*
	*	Get the type \c T with removed sized-array.
	*
	*	\tparam T	Type to extract from.
	*
	*	\implements RemovePtrTemplate
	*/
	template <typename T, uint32 size>
	struct RemovePtrTemplate<T[size]>
	{
		/// Extracted type.
		using Type = T;
	};
	/// \endcond Internal

	/**
	*	\brief Helper template alias of \e RemovePtrTemplate<T>::Type.
	*
	*	\tparam T	Type to extract from.
	*/
	template <typename T>
	using RemovePtr = typename RemovePtrTemplate<T>::Type;


	/** \} */
}

#endif // GUARD
