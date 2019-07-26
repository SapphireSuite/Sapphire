// Copyright 2020 Sapphire development team. All Rights Reserved.

#ifndef SAPPHIRE_CORE_RAW_TYPE_GUARD
#define SAPPHIRE_CORE_RAW_TYPE_GUARD

#include <Core/Types/Extractions/RemoveRef.hpp>
#include <Core/Types/Extractions/RemoveConstVol.hpp>
#include <Core/Types/Extractions/RemovePtr.hpp>

namespace Sa
{
	/**
	*	\file Core/Types/Extractions/RawType.hpp
	*
	*	\brief \b Definition of Sapphire's \b Raw Type template type.
	*
	*	\ingroup Types
	*	\{
	*/


	/**
	*	\brief Extract the <b>raw type</b> of \c T.
	*
	*	Get the raw type \c T with removed reference, rvalue, const, volatile,
	*	const-volatile, pointer and array type.
	*	Use \e RawType<T> as helper class instead.
	*
	*	\tparam T1	Type to extract from.
	*	\tparam T2	Previous extracted iteration.
	*/
	template <typename T1, typename T2 = RemovePtr<RemoveConstVol<RemoveRef<T1>>>>
	struct RawTypeTemplate
	{
		/// Current extracted iteration.
		using Type = typename RawTypeTemplate<T2>::Type;
	};

	/// \cond Internal
	/**
	*	\brief Extract the <b>raw type</b> of \c T.
	*
	*	Get the raw type \c T with removed reference, rvalue, const, volatile,
	*	const-volatile, pointer and array type.
	*
	*	\tparam T	Type to extract from.
	*
	*	\implements RawTypeTemplate
	*/
	template <typename T>
	struct RawTypeTemplate<T, T>
	{
		/// Extracted type.
		using Type = T;
	};
	/// \endcond Internal

	/**
	*	\brief Helper template alias of \e RawTypeTemplate<T>::Type.
	*
	*	\tparam T	Type to extract from.
	*/
	template <typename T>
	using RawType = typename RawTypeTemplate<T>::Type;


	/** \} */
}

#endif // GUARD
