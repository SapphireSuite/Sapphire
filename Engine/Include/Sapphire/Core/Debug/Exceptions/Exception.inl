// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template <ExceptCode TCode>
	Exception<TCode>::Exception(
		const wchar* _file,
		const char8* _function,
		uint32 _line,
		const wchar* _title,
		LogLvlFlag _level,
		const LogChannel& _channel,
		const wchar* _details
	) noexcept :
		ExceptionBase(_file, _function, _line, _title, TCode, _level, _channel),
		details{ _details }
	{
	}

	template <ExceptCode TCode>
	Exception<TCode>::Exception(Exception&& _other) noexcept :
		ExceptionBase(Move(_other)),
		details{ _other.details }
	{
	}

	template <ExceptCode TCode>
	Exception<TCode>::Exception(const Exception& _other) noexcept :
		ExceptionBase(_other),
		details{ _other.details }
	{
	}


	template <ExceptCode TCode>
	void Exception<TCode>::OutputDetails(std::wostream& _stream) const noexcept
	{
		_stream << details;
	}
}
