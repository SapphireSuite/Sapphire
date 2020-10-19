// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_CAMERA_UNIFORM_BUFFER_GUARD
#define SAPPHIRE_RENDERING_CAMERA_UNIFORM_BUFFER_GUARD

#include <Maths/Space/Matrix4.hpp>

namespace Sa
{
	struct CameraUniformBuffer
	{
		// Camera inverse transformation matrix.
		Mat4f viewInvMat = Mat4f::Identity;

		// Camera projection matrix.
		Mat4f projMat = Mat4f::Identity;

		// Camera position.
		Vec3f viewPosition;
	};
}

#endif // GUARD
