// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_API_SPECIFIC_GUARD
#define SAPPHIRE_RENDERING_API_SPECIFIC_GUARD

#include <Core/Algorithms/Move.hpp>

#include <Maths/Space/Vector3.hpp>
#include <Maths/Space/Matrix4.hpp>

namespace Sa
{
	/**
	*	Convert coordinates from Sapphire's to API's system or from API's to Sapphire's.
	*
	*	\param[in] _position	Position coordinates to convert.
	*
	*	\result Converted coordinates.
	*/
	template <typename T>
	Vec3<T> API_ConvertCoordinateSystem(const Vec3<T>& _position) noexcept;

	/**
	*	Convert coordinates from Sapphire's to API's system or from API's to Sapphire's.
	*
	*	\param[in] _position	Position coordinates to convert.
	*
	*	\result Converted coordinates.
	*/
	template <typename T>
	Vec3<T>&& API_ConvertCoordinateSystem(Vec3<T>&& _position) noexcept;


	/**
	*	Convert coordinates from Sapphire's to API's system or from API's to Sapphire's.
	*
	*	\param[in] _matrix	Transformation matrix to convert.
	*
	*	\result Converted transformed matrix.
	*/
	template <typename T>
	Mat4<T> API_ConvertCoordinateSystem(const Mat4<T>& _matrix) noexcept;

	/**
	*	Convert coordinates from Sapphire's to API's system or from API's to Sapphire's.
	*
	*	\param[in] _matrix	Transformation matrix to convert.
	*
	*	\result Converted transformed matrix.
	*/
	template <typename T>
	Mat4<T>&& API_ConvertCoordinateSystem(Mat4<T>&& _matrix) noexcept;
}

#include <Rendering/Misc/APISpecific.inl>

#endif // GUARD
