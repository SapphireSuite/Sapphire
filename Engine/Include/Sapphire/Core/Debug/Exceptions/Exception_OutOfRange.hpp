// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_EXCEPTION_OUT_OF_RANGE_GUARD
#define SAPPHIRE_CORE_EXCEPTION_OUT_OF_RANGE_GUARD

#include <Core/Debug/Exceptions/Exception.hpp>

namespace Sa
{
	/**
	*	\file Exception_OutOfRange.hpp
	*
	*	\brief Sapphire's Exception out of range definitions.
	*
	*	\ingroup Debug
	*	\{
	*/


	/**
	*	\brief Sapphire's Exception out of range class specialization.
	*
	*	Contains exception out of range infos to catch after throw.
	*/
	template <>
	class Exception<ExceptCode::OutOfRange> : public ExceptionBase
	{
	protected:
		SA_ENGINE_API void OutputDetails(std::wostream& _stream) const noexcept override final;

	public:
		/// The current index.
		const uint32 currIndex = 0;

		/// The min bound allowed by the range.
		const uint32 minBound = 0;

		/// The max bound allowed by the range.
		const uint32 maxBound = 0;

		/**
		*	\brief \e Value Constructor.
		*
		*	\param[in] _file		File of the Log.
		*	\param[in] _function	Function of the Log.
		*	\param[in] _line		Line of the Log.
		*	\param[in] _title		Title name of the exception.
		*	\param[in] _level		Level of the exception.
		*	\param[in] _channel		Channel of the exception.
		*	\param[in] _currIndex	Current index.
		*	\param[in] _minBound	Min bound of range.
		*	\param[in] _maxBound	Max bound of range.
		*/
		SA_ENGINE_API Exception(
			const wchar* _file,
			const char8* _function,
			uint32 _line,
			const wchar* _title,
			LogLvlFlag _level,
			const LogChannel& _channel,
			uint32 _currIndex = 0u,
			uint32 _minBound = 0u,
			uint32 _maxBound = 0u
			) noexcept;

		/**
		*	\brief \e Move constructor.
		*
		*	\param[in] _other	Other to construct from.
		*/
		SA_ENGINE_API Exception(Exception&& _other) noexcept;

		/**
		*	\brief \e Copy constructor.
		*
		*	\param[in] _other	Other to construct from.
		*/
		SA_ENGINE_API Exception(const Exception& _other) noexcept;
	};


	/** \} */
}

#endif // GUARD
