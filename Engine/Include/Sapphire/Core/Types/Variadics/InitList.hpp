// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_INIT_LIST_GUARD
#define SAPPHIRE_CORE_INIT_LIST_GUARD

#include <initializer_list>

namespace Sa
{
	/**
	*	\file Core/Types/Variadics/InitList.hpp
	*
	*	\brief \b Definition of Sapphire's \b initializer-list type.
	*
	*	\ingroup Types
	*	\{
	*/


	/**
	*	\brief using for \b std::initializer_list.
	*
	*	std::initializer_list<T> is a part of C++11, we can't implement it by ourself.
	*	Sources: https://stackoverflow.com/questions/18164353/implementation-of-stdinitializer-list
	*/
	template <typename T>
	using InitList = std::initializer_list<T>;


	/** \} */
}

#endif // GUARD
