// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Core/Misc/Random.hpp>

namespace Sa
{
	std::random_device RandEngine::sDefaultEngine;

	RandEngine::RandEngine(uint32 _seed) noexcept : mHandle{ _seed }
	{
	}


	bool Random<bool>::Value(RandEngine* _en) noexcept
	{
		std::uniform_int_distribution<int16> distribution(0, 1);
		
		return _en ? distribution(_en->mHandle) : distribution(RandEngine::sDefaultEngine);
	}
}
