// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_UNIFORM_OBJECT_GUARD
#define SAPPHIRE_RENDERING_UNIFORM_OBJECT_GUARD

#include <Maths/Space/Matrix4.hpp>

namespace Sa
{
	class UniformBufferObject
	{
	public:
		Mat4f modelMat = Mat4f::Identity;
		Mat4f viewMat = Mat4f::Identity;
		Mat4f projMat = Mat4f::Identity;
	};
}

#endif // GUARD
