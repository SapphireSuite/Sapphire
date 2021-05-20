// Copyright 2020 Sapphire development team.All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_LOG_LEVEL_GUARD
#define SAPPHIRE_CORE_LOG_LEVEL_GUARD

#include <Core/Types/Char.hpp>

#include <Core/Misc/Flags.hpp>

#include <Core/Support/EngineAPI.hpp>

namespace Sa
{
	/**
	*	\file LogLevel.hpp
	*
	*	\brief Log level enum used by Sapphire's Debug class.
	*
	*	\ingroup Debug
	*	\{
	*/

	/**
	*	\brief All levels of logging.
	*/
	enum class LogLvlFlag : uint8
	{
		/// Normal level
		Normal = (1 << 0),

		/// Info level
		Infos = (1 << 1),

		/// Warning level
		Warning = (1 << 2),

		/// Error level
		Error = (1 << 3),

		/// Assert Success level
		AssertSuccess = (1 << 4),

		///Assert  Failed level
		AssertFailed = (1 << 5),
	};

	/**
	*	\brief Alias of \e Flags<LogLvlFlag>
	*/
	using LogLvlFlags = Flags<LogLvlFlag, ThreadMode::Safe>;

	/**
	*	\brief \e Getter of Log level name from flag.
	*
	*	\param _flag	Flag to get the name.
	*
	*	\return			Name of the enum.
	*/
	SA_ENGINE_API const wchar* GetLogLevelName(LogLvlFlag _flag) noexcept;

	/** \} */
}

#endif // GUARD
