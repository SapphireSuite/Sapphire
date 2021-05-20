// Copyright 2020 Sapphire development team. All Rights Reserved.

#ifndef SAPPHIRE_CORE_REMOVE_REF_GUARD
#define SAPPHIRE_CORE_REMOVE_REF_GUARD

namespace Sa
{
	/**
	*	\file Core/Types/Extractions/RemoveRef.hpp
	*
	*	\brief \b Definition of Sapphire's \b Remove Reference template type.
	*
	*	\ingroup Types
	*	\{
	*/


	/**
	*	\brief Extract the \b non-reference and \b non-rvalue type of \c T.
	*
	*	Get the type \c T with removed reference and rvalue.
	*	Use \e RemoveRef<T> as helper class.
	*
	*	\tparam T	Type to extract from.
	*/
	template <typename T>
	struct RemoveRefTemplate
	{
		/// Extracted type.
		using Type = T;
	};

	/// \cond Internal
	/**
	*	\brief Extract the \b non-reference type of \c T.
	*
	*	Get the type \c T with removed reference.
	*
	*	\tparam T	Type to extract from.
	*
	*	\implements RemoveRefTemplate
	*/
	template <typename T>
	struct RemoveRefTemplate<T&>
	{
		/// Extracted type.
		using Type = T;
	};

	/**
	*	\brief Extract the \b non-rvalue type of \c T.
	*
	*	Get the type \c T with removed rvalue.
	*
	*	\tparam T	Type to extract from.
	*
	*	\implements RemoveRefTemplate
	*/
	template <typename T>
	struct RemoveRefTemplate<T&&>
	{
		/// Extracted type.
		using Type = T;
	};
	/// \endcond Internal

	/**
	*	\brief Helper template alias of \e RemoveRefTemplate<T>::Type.
	*
	*	\tparam T	Type to extract from.
	*/
	template <typename T>
	using RemoveRef = typename RemoveRefTemplate<T>::Type;


	/** \} */
}

#endif // GUARD
