// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_UNIFORM_BUFFERS_GUARD
#define SAPPHIRE_RENDERING_UNIFORM_BUFFERS_GUARD

#include <Maths/Space/Matrix4.hpp>

namespace Sa
{
	struct StaticUniformBuffer
	{
		Mat4f viewMat = Mat4f::Identity;
		Mat4f projMat = Mat4f::Identity;
	};

	struct ObjectUniformBuffer
	{
		Mat4f modelMat = Mat4f::Identity;
	};
}

#endif // GUARD
