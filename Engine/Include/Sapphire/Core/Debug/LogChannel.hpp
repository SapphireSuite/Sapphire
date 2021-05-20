// Copyright 2020 Sapphire development team.All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_LOG_CHANNEL_GUARD
#define SAPPHIRE_CORE_LOG_CHANNEL_GUARD

#include <Core/Config.hpp>

#include <Core/Types/Char.hpp>

#include <Core/Misc/RAII.hpp>
#include <Core/Misc/Macro.hpp>

namespace Sa
{
	/**
	*	\file LogChannel.hpp
	*
	*	\brief Log channel used by Sapphire's Debug class.
	*
	*	\ingroup Debug
	*	\{
	*/

	/**
	*	\brief Sapphire's Debug Log channel class.
	*
	*	Paired use with RAII class.
	*/
	class LogChannel
	{
	public:
		/// Log channel name.
		const wchar* name = nullptr;
		
		/// Whether logging is enabled.
		bool enableLog = true;

		/// Whether assertion is enabled.
		bool enableAssert = true;

		/**
		*	\brief \e value constructor
		*
		*	\param _name	Name of the channel.
		*/
		SA_ENGINE_API LogChannel(const wchar* _name) noexcept;
	};

#if SA_LOGGING

	/**
	*	\brief RAII specialization for LogChannel.
	*	
	*	Register LogChannel on construct, UnRegister LogChannel on destruct.
	*
	*	\implements RAII
	*/
	template <>
	class RAII<LogChannel> final : RAIIBase
	{
		/// The handled log channel
		LogChannel& mHandle;

	public:
		/**
		*	\brief \e value constructor
		*
		*	\param[in] _name	Name of the LogChannel to handle.
		*/
		SA_ENGINE_API RAII(const wchar* _name) noexcept;

		/**
		*	\brief \b Destructor.
		*
		*	UnRegister mHandle Channel.
		*/
		SA_ENGINE_API ~RAII() noexcept;
	};

	#define SA_LOG_CHAN_DEFINE(_chan) Sa::RAII<Sa::LogChannel> __RAII_LogChannel_ ##_chan(SA_WSTR(_chan));

#else

	#define SA_LOG_CHAN_DEFINE(...)

#endif

	/**
	*	\def SA_LOG_CHAN_DEFINE(_chan)
	*
	*	\brief Sapphire define LogChannel macro.
	*
	*	Helper macro to define a new log channel.
	*	Must be used at the top of a .cpp file.
	*/

	/** \} */
}

#endif // GUARD
