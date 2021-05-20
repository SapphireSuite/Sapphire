// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Core/Misc/RAII.hpp>

namespace Sa
{
	RAII<bool>::RAII(bool& _handle, bool _value) noexcept : mHandle{ _handle }
	{
		mHandle = _value;
	}

	RAII<bool>::~RAII() noexcept
	{
		mHandle = !mHandle;
	}
}
