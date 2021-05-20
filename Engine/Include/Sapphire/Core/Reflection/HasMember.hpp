// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_HAS_MEMBER_GUARD
#define SAPPHIRE_CORE_HAS_MEMBER_GUARD

#include <Core/Types/Conditions/IsSame.hpp>
#include <Core/Types/Conditions/IsFunction.hpp>

namespace Sa
{
	/**
	*	\file HasMember.hpp
	*
	*	\brief \b Generation of \e compile-time checker if type has member with name and type.
	*
	*	\ingroup Reflection
	*	\{
	*/


	namespace Internal
	{
		class HasMemberBase
		{
		protected:
			/// SFINAE success type.
			using Yes = char;

			/// SFINAE failure type.
			using No = char[2];

			/// template deduction helper.
			template<typename C, C> struct CheckT;
		};
	}


	/**
	*	\def SA_DEFINE_HAS_MEMBER(_name)
	*
	*	\brief \b Generate classes HM_name and HMOT_name to allow \e compile-time check.
	*
	*	HasMember use SFINAE to perform check without throwing errors.
	*	See documentation of SFINAE here: https://en.cppreference.com/w/cpp/language/sfinae
	*	See HasMember inspired source here: https://stackoverflow.com/questions/1005476/how-to-detect-whether-there-is-a-specific-member-variable-in-class
	*/
	#define SA_DEFINE_HAS_MEMBER(_name)																	\
	template<typename T>																				\
	class HM_ ## _name : Sa::Internal::HasMemberBase													\
	{																									\
		struct Fallback { int _name; };																	\
		struct Derived : T, Fallback { };																\
																										\
		template <typename C>																			\
		static Yes& SFINAE(CheckT<int Fallback::*, &C::_name>*);										\
																										\
		template <typename C>																			\
		static No& SFINAE(...);																			\
																										\
	public:																								\
		static constexpr bool const value = sizeof(SFINAE<Derived>(0)) == sizeof(No);					\
																										\
		template <typename MemT>																		\
		static constexpr bool OfType()																	\
		{																								\
			if constexpr (value)																		\
			{																							\
				if constexpr(IsFunction<MemT>::value)													\
					return IsSame<decltype(&T::_name), MemT>::value;									\
				else																					\
					return IsSame<decltype(T::_name), MemT>::value;										\
			}																							\
																										\
			return false;																				\
		}																								\
	};																									\
																										\
	template<typename T, typename MemT>																	\
	struct HMOT_##_name : public StaticConstant<bool, HM_ ## _name<T>::template OfType<MemT>()>{};


	namespace Doc
	{
		/**
		*	\brief HasMember_Name \e Compile-time check if T as a <b>public member</b> called \b Name.
		*
		*	This class is only for documentation purpose.
		*
		*	\tparam T The type to inspect.
		*/
		template <typename T>
		class HM_Name : Internal::HasMemberBase
		{
			/// Introduce Name member.
			struct Fallback
			{
				/// Dummy variable with name Name for SFINAE deduction.
				int Name;
			};

			/// Merge T and Fallback members.
			struct Derived : T, Fallback { };

			template<typename C, C> struct ChT;

			/**
			*	\brief SFINAE success expression.
			*
			*	Try to access member Name through Fallback class.
			*	Success means C hasn't member called Name.
			*
			*	\tparam C The type to try SFINAE.
			*
			*	\param[in] _c The argument to try tempalte deduction from.
			*
			*	\return char array of static size 1.
			*/
			template <typename C>
			static Yes& SFINAE(ChT<int Fallback::*, &C::Name>* _c);

			/**
			*	\brief SFINAE failure expression.
			*
			*	\tparam C The type to try SFINAE.
			*
			*	\param ... Arguments which failed ChT template deduction.
			*
			*	\return char array of static size 2.
			*/
			template <typename C>
			static No& SFINAE(...);

		public:

			/// true if T as a public member called Name (ie: SFINAE failure), otherwise false.
			static constexpr bool value = sizeof(SFINAE<Derived>(0)) == sizeof(No);

			/**
			*	\brief \e Compile-time check if T as a <b>public member</b> called \b Name of type \b MemT.
			*
			*	Use HMOT_Name as helper access class.
			*
			*	\tparam MemT The type to check the member with.
			*
			*	\return true if T as a public member called Name of type MemT, otherwise false.
			*/
			template <typename MemT>
			static constexpr bool OfType()
			{
				if constexpr (value)
				{
					if constexpr (IsFunction<MemT>::value)
						return IsSame<decltype(&T::Name), MemT>::value;
					else
						return IsSame<decltype(T::Name), MemT>::value;
				}

				return false;
			}
		};

		/**
		*	\brief HasMemberOfType_Name \e Compile-time check if T as a <b>public member</b> called \b Name of type \b MemT.
		*
		*	Helper template class of \e HM_Name<T>::OfType<MemberT>()
		*	This class is only for documentation purpose.
		*
		*	\tparam T The type to inspect
		*	\tparam MemT The type to check with.
		*/
		template<typename T, typename MemT>
		struct HMOT_Name : public StaticConstant<bool, HM_Name<T>::template OfType<MemT>()> {};
	}


	/** \} */
}

#endif // GUARD