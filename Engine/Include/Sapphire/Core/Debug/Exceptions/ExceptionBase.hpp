// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_EXCEPTION_BASE_GUARD
#define SAPPHIRE_CORE_EXCEPTION_BASE_GUARD

#include <Core/Debug/Log.hpp>
#include <Core/Debug/Exceptions/ExceptCode.hpp>

namespace Sa
{
	/**
	*	\file ExceptionBase.hpp
	*
	*	\brief Sapphire's Exception base definitions.
	*
	*	\ingroup Debug
	*	\{
	*/


	/**
	*	\brief Sapphire's Exception base class.
	*
	*	Contains exception base infos to catch after throw.
	*/
	class ExceptionBase : public Log
	{
	protected:
		/**
		*	\brief \e Output exception details into stream.
		*
		*	Implemented in children.
		*
		*	\param[in,out] _stream		Stream to output in.
		*/
		virtual void OutputDetails(std::wostream& _stream) const noexcept = 0;

	public:
		/// The code of the exception.
		const ExceptCode code = ExceptCode::Unknown;

		/**
		*	\brief \e Value Constructor.
		*
		*	\param[in] _file		File of the exception.
		*	\param[in] _function	Function of the exception.
		*	\param[in] _line		Line of the exception.
		*	\param[in] _str			String of the exception.
		*	\param[in] _code		Code of the exception.
		*	\param[in] _level		Level of the exception.
		*	\param[in] _channel		Channel of the exception.
		*/
		SA_ENGINE_API ExceptionBase(
			const wchar* _file,
			const char8* _function,
			uint32 _line,
			const wchar* _str,
			ExceptCode _code,
			LogLvlFlag _level,
			const LogChannel& _channel
		) noexcept;

		/**
		*	\brief \e Move constructor.
		*
		*	\param[in] _other	Other to construct from.
		*/
		SA_ENGINE_API ExceptionBase(ExceptionBase&& _other) noexcept;

		/**
		*	\brief \e Copy constructor.
		*
		*	\param[in] _other	Other to construct from.
		*/
		SA_ENGINE_API ExceptionBase(const ExceptionBase& _other) noexcept;

		/**
		*	\brief \e Output exception into stream.
		*
		*	\param[in,out] _stream		Stream to output in.
		*/
		SA_ENGINE_API void Output(std::wostream& _stream) const noexcept override;
	};


	/** \} */
}

#endif // GUARD
