// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_CAMERA_GPU_GUARD
#define SAPPHIRE_RENDERING_CAMERA_GPU_GUARD

#include <Maths/Space/Matrix4.hpp>

namespace Sa
{
	struct Camera_GPU
	{
		// Camera projection matrix.
		Mat4f projMat = Mat4f::Identity;

		struct ViewInfos
		{
			// Camera inverse transformation matrix.
			Mat4f viewInvMat = Mat4f::Identity;

			// Camera position.
			Vec3f viewPosition;

		} viewInfos;

		bool bEnabled = true;

		Camera_GPU() = default;
		Camera_GPU(Mat4f&& _projMat, Mat4f&& _viewInvMat, Vec3f&& _viewPosition) noexcept;
		Camera_GPU(const Mat4f& _projMat, const Mat4f& _viewInvMat, const Vec3f& _viewPosition) noexcept;
	};

	static constexpr uint32 __cameraGPUProjMatOffset = offsetof(Camera_GPU, projMat);
	static constexpr uint32 __cameraGPUViewOffset = offsetof(Camera_GPU, viewInfos);
}

#endif // GUARD
