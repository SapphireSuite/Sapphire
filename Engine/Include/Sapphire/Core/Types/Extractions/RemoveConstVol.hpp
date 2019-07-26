// Copyright 2020 Sapphire development team. All Rights Reserved.

#ifndef SAPPHIRE_CORE_REMOVE_CONST_VOL_GUARD
#define SAPPHIRE_CORE_REMOVE_CONST_VOL_GUARD

namespace Sa
{
	/**
	*	\file Core/Types/Extractions/RemoveConstVol.hpp
	*
	*	\brief \b Definition of Sapphire's \b Remove Const Volatile template type.
	*
	*	\ingroup Types
	*	\{
	*/


	/**
	*	\brief Extract the \b non-const, \b non-volatile and \b non-const-volatile type of \c T.
	*
	*	Get the type \c T with removed const, volatile and const volatile.
	*	Use \e RemoveConstVol<T> as helper class.
	*
	*	\tparam T	Type to extract from.
	*/
	template <typename T>
	struct RemoveConstVolTemplate
	{
		/// Extracted type.
		using Type = T;
	};

	/// \cond Internal
	/**
	*	\brief Extract the \b non-const type of \c T.
	*
	*	Get the type \c T with removed const.
	*
	*	\tparam T	Type to extract from.
	*
	*	\implements RemoveConstVolTemplate
	*/
	template <typename T>
	struct RemoveConstVolTemplate<const T>
	{
		/// Extracted type.
		using Type = T;
	};

	/**
	*	\brief Extract the \b non-volatile type of \c T.
	*
	*	Get the type \c T with removed volatile.
	*
	*	\tparam T	Type to extract from.
	*
	*	\implements RemoveConstVolTemplate
	*/
	template <typename T>
	struct RemoveConstVolTemplate<volatile T>
	{
		/// Extracted type.
		using Type = T;
	};

	/**
	*	\brief Extract the \b non-const-volatile type of \c T.
	*
	*	Get the type \c T with removed const volatile.
	*
	*	\tparam T	Type to extract from.
	*
	*	\implements RemoveConstVolTemplate
	*/
	template <typename T>
	struct RemoveConstVolTemplate<const volatile T>
	{
		/// Extracted type.
		using Type = T;
	};
	/// \endcond Internal

	/**
	*	\brief Helper template alias of \e RemoveConstVolTemplate<T>::Type.
	*
	*	\tparam T	Type to extract from.
	*/
	template <typename T>
	using RemoveConstVol = typename RemoveConstVolTemplate<T>::Type;


	/** \} */
}

#endif // GUARD
