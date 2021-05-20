// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Core/Debug/Exceptions/Exception_OutOfRange.hpp>

#include <Core/Algorithms/Move.hpp>

namespace Sa
{
	Exception<ExceptCode::OutOfRange>::Exception(
		const wchar* _file,
		const char8* _function,
		uint32 _line,
		const wchar* _title,
		LogLvlFlag _level,
		const LogChannel& _channel,
		uint32 _currIndex,
		uint32 _minBound,
		uint32 _maxBound
	) noexcept :
		ExceptionBase(_file, _function, _line, _title, ExceptCode::OutOfRange, _level, _channel),
		currIndex{ _currIndex },
		minBound{ _minBound },
		maxBound{ _maxBound }
	{
	}

	Exception<ExceptCode::OutOfRange>::Exception(Exception&& _other) noexcept :
		ExceptionBase(Move(_other)),
		currIndex{ _other.currIndex },
		minBound{ _other.minBound },
		maxBound{ _other.maxBound }

	{
	}

	Exception<ExceptCode::OutOfRange>::Exception(const Exception& _other) noexcept :
		ExceptionBase(_other),
		currIndex{ _other.currIndex },
		minBound{ _other.minBound },
		maxBound{ _other.maxBound }
	{
	}


	void Exception<ExceptCode::OutOfRange>::OutputDetails(std::wostream& _stream) const noexcept
	{
		_stream << "Index [" << currIndex << "] is out of range [" << minBound << ',' << maxBound << ']';
	}
}