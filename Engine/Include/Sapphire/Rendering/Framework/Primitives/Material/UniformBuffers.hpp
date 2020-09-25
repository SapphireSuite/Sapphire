// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_UNIFORM_BUFFERS_GUARD
#define SAPPHIRE_RENDERING_UNIFORM_BUFFERS_GUARD

#include <Maths/Space/Matrix4.hpp>

namespace Sa
{
	struct StaticUniformBuffer
	{
		// Camera inverse transformation matrix.
		Mat4f viewInvMat = Mat4f::Identity;

		// Camera projection matrix.
		Mat4f projMat = Mat4f::Identity;
	};

	struct ObjectUniformBuffer
	{
		// Model transformation matrix.
		Mat4f modelMat = Mat4f::Identity;

		// Material UV tilling.
		float tillingUV = 1.0f;

		// Materuak UV offset.
		float offsetUV = 0.0f;
	};
}

#endif // GUARD
