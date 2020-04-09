// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Core/Debug/Exceptions/ExceptionBase.hpp>

#include <Core/Algorithms/Move.hpp>

namespace Sa
{
	ExceptionBase::ExceptionBase(
		const wchar* _file,
		const char8* _function,
		uint32 _line,
		const wchar* _str,
		ExceptCode _code,
		LogLvlFlag _level,
		const LogChannel& _channel
	) noexcept :
		Log(_file, _function, _line, _str, _level, _channel),
		code{ _code }
	{
	}

	ExceptionBase::ExceptionBase(ExceptionBase&& _other) noexcept :
		Log(Move(_other)),
		code{ _other.code }
	{
	}
	ExceptionBase::ExceptionBase(const ExceptionBase& _other) noexcept :
		Log(_other),
		code{ _other.code }
	{
	}

	void ExceptionBase::Output(std::wostream& _stream) const noexcept
	{
		// Output date.
		_stream << '[' << date.hour << ':' << date.minute << ':' << date.second << "]\t";

		// Output title.
		_stream << 'E' << static_cast<uint16>(code) << '\t' << str << ": ";

		// Output delails
		OutputDetails(_stream);

		_stream << '\n';

		// Output level and channel.
		_stream << '{' << GetLogLevelName(level) << " - " << channel.name << '}';

		// Output location.
		_stream << '\t' << file << ':' << line << " - " << function << '\n';
	}
}
