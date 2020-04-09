// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Core/Debug/Log.hpp>

#include <Core/Time/Time.hpp>

#include <Core/Algorithms/Move.hpp>

namespace Sa
{
	Log::Log(
		const wchar* _file,
		const char8* _function,
		uint32 _line,
		const wchar* _str,
		LogLvlFlag _level,
		const LogChannel& _channel
	) noexcept :
		file{ _file },
		function{ _function },
		line{ _line },
		str{ _str },
		level{ _level },
		channel{ _channel },
		date{ Time::Date() }
	{
	}

	Log::Log(Log&& _other) :
		file{ _other.file },
		function{ _other.function },
		line{ _other.line },
		str{ _other.str },
		level{ _other.level },
		channel{ _other.channel },
		date{ Move(_other.date) }
	{
	}

	Log::Log(const Log& _other) :
		file{ _other.file },
		function{ _other.function },
		line{ _other.line },
		str{ _other.str },
		level{ _other.level },
		channel{ _other.channel },
		date{ _other.date }
	{
	}

	void Log::Output(std::wostream& _stream) const noexcept
	{
		// Output date.
		_stream << '[' << date.hour << ':' << date.minute << ':' << date.second << ']';

		// Output message.
		_stream << '\t' << str << '\n';

		// Output level and channel.
		_stream << '{' << GetLogLevelName(level) << " - " << channel.name << '}';

		// Output location.
		_stream << '\t' << file << ':' << line << " - " << function << '\n';
	}

	std::wostream& Log::operator>>(std::wostream& _stream) const noexcept
	{
		Output(_stream);

		return _stream;
	}

	std::wostream& operator<<(std::wostream& _stream, const Log& _log) noexcept
	{
		_log.Output(_stream);

		return _stream;
	}
}
