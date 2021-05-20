// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_DEBUG_GUARD
#define SAPPHIRE_CORE_DEBUG_GUARD

#include <vector>

#include <Core/Config.hpp>

#include <Collections/Exceptions>

namespace Sa
{
	/**
	*	\file Debug.hpp
	*
	*	\brief Sapphire's Debug class.
	*
	*	\ingroup Debug
	*	\{
	*/


	/**
	*	\brief Static Debug class.
	*
	*	Static Debug class with level and chan masks.
	*	Enable or disable level and channel to stop logging.
	*/
	class Debug
	{
#if SA_LOGGING || SA_ASSERTION

		/// Registered channels for logging.
		SA_ENGINE_API static std::vector<LogChannel> channels;

		/**
		*	\brief Log internal implementation.
		*
		*	\param[in] _log		Log to output.
		*/
		SA_ENGINE_API static void Log_Internal(const Sa::Log& _log) noexcept;

	public:

		/// Allow logging for enabled levels.
		SA_ENGINE_API static LogLvlFlags levelMask;

		/**
		*	\brief \e Getter of file name from file path.
		*
		*	\param[in] _filePath	Full path to the file.
		*
		*	\returns pointer to the file name in _filePath.
		*/
		SA_ENGINE_API static const wchar* GetFileNameFromPath(const wchar* _filePath) noexcept;

		/**
		*	\brief \e Get LogChannel by name.
		*
		*	\param[in] _name	Name of the LogChannel.
		*
		*	\return First LogChannel with name _name.
		*/
		SA_ENGINE_API static LogChannel& GetChannel(const wchar* _name);

		/**
		*	\brief Register a log channel into channels.
		*
		*	\param[in] _chan	Channel to register.
		*
		*	\return Registered channel.
		*/
		SA_ENGINE_API static LogChannel& RegisterChannel(const wchar* _chan) noexcept;

		/**
		*	\brief UnRegister a log channel from channels.
		*
		*	\param[in] _chan	Channel to unregister.
		*/
		SA_ENGINE_API static void UnRegisterChannel(const wchar* _chan) noexcept;


		/**
		*	\brief Log str in chan with lvl.
		*
		*	Log only if both chan and lvl are enabled.
		*	Use SA_LOG as helper call.
		*
		*	\param[in] _log		Log to output.
		*/
		SA_ENGINE_API static void Log(const Sa::Log& _log) noexcept;

#endif

#if SA_ASSERTION

	public:
		/**
		*	\brief Throw assertion with str in chan on flase predicate.
		*
		*	The channel is automatically set on assertion failure.
		*	Use SA_ASSERT as helper call.
		*
		*	This must me a template Exception type to allow catch of typed exception.
		*
		*	\param[in] _exception	Exception to assert.
		*/
		template <ExceptCode code>
		static void Assert(const Exception<code>& _exception);

#endif
	};


/// \cond Internal

#if SA_GNU || SA_CLANG && !SA_MSVC

	#define __SA_FUNC_NAME __PRETTY_FUNCTION__

#elif SA_MSVC

	#define __SA_FUNC_NAME __FUNCSIG__

#else

	#define __SA_FUNC_NAME __FUNCTION__

#endif

/// \endcond Internal


#if SA_LOGGING

	#define SA_LOG_LVL_ENABLE(_lvl, _enable)\
	{\
		if(_enable)\
			Sa::Debug::levelMask.Add(Sa::LogLvlFlag::_lvl);\
		else\
			Sa::Debug::levelMask.Remove(Sa::LogLvlFlag::_lvl);\
	}

	#define SA_LOG_CHAN_ENABLE_LOG(_chan, _enable) { Sa::Debug::GetChannel(L"" # _chan).enableLog = _enable; }

	#define SA_LOG_CHAN_ENABLE_ASSERT(_chan, _enable) { Sa::Debug::GetChannel(L"" # _chan).enableAssert = _enable; }


	/// \cond Internal

	#define __SA_CREATE_LOG(_str, _lvl, _chan) Sa::Log(\
		Sa::Debug::GetFileNameFromPath(SA_WIDE(__FILE__)),\
		__SA_FUNC_NAME,\
		__LINE__,\
		SA_WIDE(_str),\
		Sa::LogLvlFlag::_lvl,\
		Sa::Debug::GetChannel(SA_WSTR(_chan))\
	)

	#define __SA_SELECT_LOG_MACRO(_1, _2, _3, _name, ...) _name

	#define __SA_LOG1(_str)					{ Sa::Debug::Log(__SA_CREATE_LOG(_str, Normal, Default)); }
	#define __SA_LOG2(_str, _lvl)			{ Sa::Debug::Log(__SA_CREATE_LOG(_str, _lvl, Default)); }
	#define __SA_LOG3(_str, _lvl, _chan)	{ Sa::Debug::Log(__SA_CREATE_LOG(_str, _lvl, _chan)); }

	/// \endcond Internal

	#define SA_LOG(...) __SA_SELECT_LOG_MACRO(__VA_ARGS__, __SA_LOG3, __SA_LOG2, __SA_LOG1)(__VA_ARGS__)

#else
	/// \cond Internal

	#define __SA_CREATE_LOG(...)

	/// \endcond Internal

	#define SA_LOG(...) {}

	#define SA_LOG_LVL_ENABLE(...) {}

	#define SA_LOG_CHAN_ENABLE_LOG(...) {}

	#define SA_LOG_CHAN_ENABLE_ASSERT(...) {}

#endif

#if SA_ASSERTION

	/// \cond Internal

	#define __SA_CREATE_EXCEPTION(_predicate, _code, _chan, ...) Sa::Exception<Sa::ExceptCode::_code>(\
		Sa::Debug::GetFileNameFromPath(SA_WIDE(__FILE__)),\
		__SA_FUNC_NAME,\
		__LINE__,\
		SA_WSTR(_code),\
		(_predicate) ? LogLvlFlag::AssertSuccess : LogLvlFlag::AssertFailed,\
		SA_WSTR(_chan),\
		##__VA_ARGS__\
	)

	/// \endcond Internal

	#define SA_ASSERT(_predicate, _code, _chan, ...) { Sa::Debug::Assert(__SA_CREATE_EXCEPTION(_predicate, _code, _chan, ##__VA_ARGS__)); }

#else

	/// \cond Internal

	#define __SA_CREATE_EXCEPTION(...)

	/// \endcond Internal

	#define SA_ASSERT(...) {}

#endif


	/**
	*	\def SA_LOG_LVL_ENABLE(_lvl, _enable)
	*
	*	\brief Sapphire enable / disable log level macro.
	*
	*	Helper macro to use Debug::levelMask.
	*
	*	\param[in] _lvl			LogLevel to set new enable state.
	*	\param[in] _enable		New enable state of the LogLevel.
	*/

	/**
	*	\def SA_LOG_CHAN_ENABLE_LOG(_chan, _enable)
	*
	*	\brief Sapphire enable / disable logging in log channel macro.
	*
	*	Helper macro to set channel.enableLog.
	*
	*	\param[in] _chan		Channel to set new enable state.
	*	\param[in] _enable		New enable state of the channel.
	*/

	/**
	*	\def SA_LOG_CHAN_ENABLE_ASSERT(_chan, _enable)
	*
	*	\brief Sapphire enable / disable assertion in log channel macro.
	*
	*	Helper macro to set channel.enableAssert.
	*
	*	\param[in] _chan		Channel to set new enable state.
	*	\param[in] _enable		New enable state of the channel.
	*/

	/**
	*	\def SA_LOG(_str, _lvl = Normal, _chan = Default)
	*
	*	\brief Sapphire Log macro.
	*
	*	Helper macro to use Debug::Log.
	*
	*	\param[in] _str		String message of the log.
	*	\param[in] _lvl		Level of the log.
	*	\param[in] _chan	Channel of the log.
	*/

	/**
	*	\def SA_ASSERT(_predicate, _code = Unknown, _details = "", _chan = Default)
	*
	*	\brief Sapphire Assertion macro.
	*
	*	Helper macro to use Debug::Assert.
	*
	*	\param[in] _predicate		Throw exception on false.
	*	\param[in] _code			Code of the exception.
	*	\param[in] _details			Details of the exception.
	*	\param[in] _chan			Channel of the exception.
	*/


	/// \cond Internal

	/**
	*	\def __SA_WFUNC_NAME
	*
	*	\brief Sapphire wchar function name internal macro.
	*
	*	\e Getter of the <b> current scope </b> function name and signature as wchar*.
	*/

	/**
	*	\def __SA_CREATE_LOG(_str, _lvl, _chan)
	*
	*	\brief Sapphire create log internal macro.
	*
	*	Helper macro to create a Sa::Log.
	*
	*	\param[in] _str		String message of the log.
	*	\param[in] _lvl		Level of the log.
	*	\param[in] _chan	Channel of the log.
	*
	*	\return The created Sa::Log.
	*/

	/**
	*	\def __SA_CREATE_EXCEPTION(_predicate, _code, _chan, _details)
	*
	*	\brief Sapphire create exception internal macro.
	*
	*	Helper macro to create a Sa::Exception.
	*
	*	\param[in] _predicate		Whether to create the exception with AssertionSuccess or AssertionFailed level.
	*	\param[in] _code			Code of the exception.
	*	\param[in] _chan			Channel of the exception.
	*	\param[in] _details			Details of the exception.
	*
	*	\return The created Sa::Exception.
	*/

	/// \endcond Internal


	/** \} */
}

#include <Core/Debug/Debug.inl>

#endif // GUARD
