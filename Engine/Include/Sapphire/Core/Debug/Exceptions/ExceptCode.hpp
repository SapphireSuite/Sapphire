#pragma once

#ifndef SAPPHIRE_CORE_EXCEPTION_CODE_GUARD
#define SAPPHIRE_CORE_EXCEPTION_CODE_GUARD

#include <Core/Types/Int.hpp>

namespace Sa
{
	/**
	*	\file ExceptCode.hpp
	*
	*	\brief Sapphire's Exception codes definitions.
	*
	*	\ingroup Debug
	*	\{
	*/

	/**
	*	\brief Exception codes enum class.
	*
	*	Reference all exception codes.
	*/
	enum class ExceptCode : uint16
	{
		/// Default error code.
		Unknown = 0,

		/// Feature is not supported by the hardware device.
		NotSupported = 1,

		/// Parameter of method is invalid.
		InvalidParam,


		/** Memory Errors: 100 */

		/// Nullptr access
		Nullptr = 100,

		/// Out of range access
		OutOfRange = 101,


		/** Maths Errors: 200 */

		/// Try to divide by 0.
		DivisionBy0 = 200,

		/// Operation on non-normalized object.
		NonNormalized = 201,


		/** Window Errors: 300 */

		/// Window initialization.
		WindowInit = 300,

		/// Window creation.
		WindowCreate = 301,
	};


	/** \} */
}

#endif // GUARD
