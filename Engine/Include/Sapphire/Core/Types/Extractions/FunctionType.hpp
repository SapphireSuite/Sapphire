// Copyright 2020 Sapphire development team. All Rights Reserved.

#ifndef SAPPHIRE_CORE_FUNCTION_TYPE_GUARD
#define SAPPHIRE_CORE_FUNCTION_TYPE_GUARD

namespace Sa
{
	/**
	*	\file Core/Types/Extractions/FunctionType.hpp
	*
	*	\brief \b Definition of Sapphire's \b Function-getter template type.
	*
	*	\ingroup Types
	*	\{
	*/


	/**
	*	\brief Extract the \b function, \b return and \b arguments types of \c T.
	*
	*	\tparam T	Type to extract from.
	*/
	template <typename T>
	struct FunctionType;

	/// \cond Internal
	/**
	*	\brief Extract the \b pointer-to-function, \b return and \b arguments types of \c T.
	*
	*	\tparam R		return type of the function type.
	*	\tparam Args	argument types of the function type.
	*/
	template <typename R, typename... Args>
	struct FunctionType<R(*)(Args...)>
	{
		/// \e Getter of the return type of the function type.
		using ReturnT = R;

		/// \e Getter of the argument types of the function type. Not available now.
		//using... ArgsT = Args;

		/// \e Getter of the function type.
		using FuncT = R(*)(Args...);
	};


	/**
	*	\brief Extract the \b pointer-to-member-function, \b return and \b arguments types of \c T.
	*
	*	\tparam R		return type of the function type.
	*	\tparam M		member type of the function type.
	*	\tparam Args	argument types of the function type.
	*/
	template <typename R, typename M, typename... Args>
	struct FunctionType<R(M::*)(Args...)>
	{
		/// \e Getter of the return type of the function type.
		using ReturnT = R;

		/// \e Getter of the member type of the function type.
		using MemberT = M;

		/// \e Getter of the argument types of the function type. Not available now.
		//using... ArgsT = Args;

		/// \e Getter of the member function type.
		using FuncT = R(M::*)(Args...);

		/// \e Getter of the raw function type.
		using RawFuncT = R(*)(Args...);
	};
	/// \endcond Internal


	/** \} */
}

#endif // GUARD
