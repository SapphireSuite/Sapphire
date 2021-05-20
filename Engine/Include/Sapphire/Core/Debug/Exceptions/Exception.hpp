// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_EXCEPTION_GUARD
#define SAPPHIRE_CORE_EXCEPTION_GUARD

#include <Core/Debug/Exceptions/ExceptionBase.hpp>
#include <Core/Debug/Exceptions/ExceptCode.hpp>

namespace Sa
{
	/**
	*	\file Exception.hpp
	*
	*	\brief Sapphire's Exception definitions.
	*
	*	\ingroup Debug
	*	\{
	*/


	/**
	*	\brief Sapphire's Exception class.
	*
	*	Contains exception infos to catch after throw.
	*
	*	\tparam TCode	Code of the exception. Used for template specialization.
	*/
	template <ExceptCode TCode>
	class Exception : public ExceptionBase
	{
	protected:
		/**
		*	\brief \e Output exception details into stream.
		*
		*	\param[in,out] _stream		Stream to output in.
		*/
		void OutputDetails(std::wostream& _stream) const noexcept override final;

	public:
		/// Additionnal details of the exception.
		const wchar* details = nullptr;

		/**
		*	\brief \e Value Constructor.
		*
		*	\param[in] _file		File of the Log.
		*	\param[in] _function	Function of the Log.
		*	\param[in] _line		Line of the Log.
		*	\param[in] _title		Title name of the exception.
		*	\param[in] _level		Level of the exception.
		*	\param[in] _channel		Channel of the exception.
		*	\param[in] _details		Additionnal details of the exception.
		*/
		Exception(
			const wchar* _file,
			const char8* _function,
			uint32 _line,
			const wchar* _title,
			LogLvlFlag _level,
			const LogChannel& _channel,
			const wchar* _details = L""
		) noexcept;

		/**
		*	\brief \e Move constructor.
		*
		*	\param[in] _other	Other to construct from.
		*/
		Exception(Exception&& _other) noexcept;

		/**
		*	\brief \e Copy constructor.
		*
		*	\param[in] _other	Other to construct from.
		*/
		Exception(const Exception& _other) noexcept;
	};


	/** \} */
}

#include <Core/Debug/Exceptions/Exception.inl>

#endif // GUARD
