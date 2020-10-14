// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_UNIFORM_BUFFERS_GUARD
#define SAPPHIRE_RENDERING_UNIFORM_BUFFERS_GUARD

#include <Maths/Space/Matrix4.hpp>

namespace Sa
{
	struct DefaultUniformBuffer
	{
		// Model transformation matrix.
		Mat4f modelMat = Mat4f::Identity;

		// Material UV tilling.
		float uvTilling = 1.0f;

		// Material UV offset.
		float uvOffset = 0.0f;
	};
}

#endif // GUARD
