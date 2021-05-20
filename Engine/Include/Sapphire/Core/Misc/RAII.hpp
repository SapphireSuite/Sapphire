// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_RAII_GUARD
#define SAPPHIRE_CORE_RAII_GUARD

#include <Core/Support/EngineAPI.hpp>

namespace Sa
{
	/**
	*	\file RAII.hpp
	*
	*	\brief \b Definition of Sapphire's \b RAII type.
	*
	*	\ingroup Misc
	*	\{
	*/


	/// \cond Internal
	/**
	*	\brief RAII Base class which remove move or copy operations.
	*/
	class RAIIBase
	{
	public:
		/**
		*	\brief \b Default constructor.
		*/
		RAIIBase() = default;

		/**
		*	\brief \b Deleted \e move constructor.
		*/
		RAIIBase(RAIIBase&&) = delete;

		/**
		*	\brief \b Deleted \e copy constructor.
		*/
		RAIIBase(const RAIIBase&) = delete;


		/**
		*	\brief \b Deleted \e move operator.
		*
		*	\return this instance.
		*/
		RAIIBase& operator=(RAIIBase&&) = delete;

		/**
		*	\brief \b Deleted \e copy operator.
		*
		*	\return this instance.
		*/
		RAIIBase& operator=(const RAIIBase&) = delete;
	};
	/// \endcond Internal

	/**
	*	\brief Template class for RAII definitions.
	*
	*	This is only a forward declaration.
	*	The final class must be defined by the user by creating
	*	a template specialization of RAII class which inherit from RAIIBase.
	*
	*	\implements RAIIBase
	*/
	template <typename T>
	class RAII;

	/**
	*	\brief RAII specialization for bool.
	*
	*	\implements RAII
	*/
	template <>
	class RAII<bool> final : RAIIBase
	{
		/// The handled bool.
		bool& mHandle;

	public:
		/**
		*	\brief \e Value constructor
		*
		*	\param[in, out] _handle		Bool to handle.
		*	\param[in] _value			Value to assign to the handle.
		*/
		SA_ENGINE_API RAII(bool& _handle, bool _value = true) noexcept;

		/**
		*	\brief \b Reset the handle to !value.
		*/
		SA_ENGINE_API ~RAII() noexcept;
	};


	/** \} */
}

#endif // GUARD
