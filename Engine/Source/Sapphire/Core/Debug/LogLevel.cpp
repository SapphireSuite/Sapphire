// Copyright 2020 Sapphire development team.All Rights Reserved.

#include <Core/Debug/LogLevel.hpp>

namespace Sa
{
	const wchar* GetLogLevelName(LogLvlFlag _flag) noexcept
	{
		if (static_cast<uint16>(_flag) == 1)
			return L"Normal";
		else if ((static_cast<uint16>(_flag) >> 1) == 1)
			return L"Infos";
		else if ((static_cast<uint16>(_flag) >> 2) == 1)
			return L"Warning";
		else if ((static_cast<uint16>(_flag) >> 3) == 1)
			return L"Error";
		else if ((static_cast<uint16>(_flag) >> 4) == 1)
			return L"AssertSuccess";
		else if ((static_cast<uint16>(_flag) >> 5) == 1)
			return L"AssertFailed";

		return L"Unknown";
	}
}
