// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template <typename T>
	Vec3<T> API_ConvertCoordinates(const Vec3<T>& _position) noexcept
	{
		/*
		*	Sapphire coordinate system: Right handed no rotation.
		*
		*		y
		*		|
		*		|
		*		------- x
		*	   /
		*	  /
		*	 z
		*/

		Vec3<T> result = _position;

#if SA_RENDERING_API == SA_VULKAN

		/*
		*	Vulkan coordinate system: Left handed rotation Z 180
		*
		*		------ x
		*	  / |
		*	 /	|
		*	z	y
		*/

		result.y *= -1;

#endif

		return result;
	}
}