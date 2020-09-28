// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_API_SPECIFIC_GUARD
#define SAPPHIRE_RENDERING_API_SPECIFIC_GUARD

#include <Maths/Space/Vector3.hpp>

namespace Sa
{
	/**
	*	Convert coordinates from Sapphire to API or from API to Sapphire.
	*
	*	\param[in] _position	Position coordinates to convert.
	*
	*	\result Converted coordinates.
	*/
	template <typename T>
	Vec3<T> API_ConvertCoordinates(const Vec3<T>& _position) noexcept;
}

#include <Rendering/Misc/APISpecific.inl>

#endif // GUARD
