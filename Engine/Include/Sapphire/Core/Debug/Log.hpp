// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_LOG_GUARD
#define SAPPHIRE_CORE_LOG_GUARD

#include <ostream>

#include <Core/Time/DateTime.hpp>

#include <Core/Debug/LogLevel.hpp>
#include <Core/Debug/LogChannel.hpp>

namespace Sa
{
	/**
	*	\file Log.hpp
	*
	*	\brief Sapphire's Log class.
	*
	*	\ingroup Debug
	*	\{
	*/

	/**
	*	\brief Sapphire's Log class.
	*
	*	Contains log infos.
	*/
	class Log
	{
	public:
		/// Log's file name.
		const wchar* file = nullptr;

		/// Log's function name.
		const char8* function = nullptr;

		/// Log's line number.
		const uint32 line = 0u;

		/// Log's string message.
		const wchar* str = nullptr;

		/// Level of Log.
		const LogLvlFlag level;

		/// Log's channel.
		const LogChannel& channel;

		/// Date time of Log.
		const DateTime date;

		/**
		*	\brief \e Value Constructor.
		*
		*	\param[in] _file		File of the Log.
		*	\param[in] _function	Function of the Log.
		*	\param[in] _line		Line of the Log.
		*	\param[in] _str			String of the Log.
		*	\param[in] _level		Level of the Log.
		*	\param[in] _channel		Channel of the Log.
		*/
		SA_ENGINE_API Log(
			const wchar* _file,
			const char8* _function,
			uint32 _line,
			const wchar* _str,
			LogLvlFlag _level,
			const LogChannel& _channel
		) noexcept;

		/**
		*	\brief \e Move constructor.
		*
		*	\param[in] _other	Other to construct from.
		*/
		SA_ENGINE_API Log(Log&& _other);

		/**
		*	\brief \e Copy constructor.
		*
		*	\param[in] _other	Other to construct from.
		*/
		SA_ENGINE_API Log(const Log& _other);

		/**
		*	\brief \e Output log into stream.
		*
		*	\param[in,out] _stream		Stream to output in.
		*/
		SA_ENGINE_API virtual void Output(std::wostream& _stream) const noexcept;

		/**
		*	\brief \e Output this log into stream.
		*
		*	\param[in,out] _stream		Stream to output in.
		*
		*	\return stream \e _stream parameter.
		*/
		SA_ENGINE_API std::wostream& operator>>(std::wostream& _stream) const noexcept;
	};


	/**
	*	\brief \e Output log into stream.
	*
	*	\param[in,out] _stream		Stream to output in.
	*	\param[in,out] _log			Log to output.
	*
	*	\return stream \e _stream parameter.
	*/
	SA_ENGINE_API std::wostream& operator<<(std::wostream& _stream, const Log& _log) noexcept;

	/** \} */
}

#endif // GUARD
