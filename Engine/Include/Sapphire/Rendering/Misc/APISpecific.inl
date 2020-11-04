// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
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

		/*
		*	Vulkan coordinate system: Left handed rotation Z 180
		*
		*		------ x
		*	  / |
		*	 /	|
		*	z	y
		*/

	template <typename T>
	Vec3<T> API_ConvertCoordinateSystem(const Vec3<T>& _position) noexcept
	{
		Vec3<T> result = _position;

#if SA_RENDERING_API == SA_VULKAN

		// Invert Y axis.
		result.y *= -1;

#endif

		return result;
	}

	template <typename T>
	Vec3<T>&& API_ConvertCoordinateSystem(Vec3<T>&& _position) noexcept
	{
#if SA_RENDERING_API == SA_VULKAN

		// Invert Y axis.
		_position.y *= -1;

#endif

		return Move(_position);
	}


	template <typename T>
	Mat4<T>& API_ConvertCoordinateSystem(Mat4<T>& _matrix) noexcept
	{
#if SA_RENDERING_API == SA_VULKAN

#if SA_MATRIX_ROW_MAJOR

		_matrix.Transpose();

#endif

		// Invert Y axis.
		_matrix.e13 *= -1;

#endif

		return _matrix;
	}

	template <typename T>
	Mat4<T> API_ConvertCoordinateSystem(const Mat4<T>& _matrix) noexcept
	{
		Mat4<T> result = _matrix;

		API_ConvertCoordinateSystem(result);

		return result;
	}
}