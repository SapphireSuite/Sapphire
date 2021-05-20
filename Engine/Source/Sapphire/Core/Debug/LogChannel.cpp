// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Core/Debug/LogChannel.hpp>

#include <Core/Debug/Debug.hpp>

namespace Sa
{
	LogChannel::LogChannel(const wchar* _name) noexcept : name{ _name }
	{
	}

#if SA_LOGGING

	RAII<LogChannel>::RAII(const wchar* _name) noexcept : mHandle{ Debug::RegisterChannel(_name) }
	{
	}

	RAII<LogChannel>::~RAII() noexcept
	{
		Debug::UnRegisterChannel(mHandle.name);
	}

#endif
}
