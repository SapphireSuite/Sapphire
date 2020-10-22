// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_CAMERA_BUFFER_GUARD
#define SAPPHIRE_RENDERING_CAMERA_BUFFER_GUARD

#include <Maths/Space/Matrix4.hpp>

namespace Sa
{
	struct CameraBuffer
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

		CameraBuffer() = default;
		CameraBuffer(Mat4f&& _projMat, Mat4f&& _viewInvMat, Vec3f&& _viewPosition) noexcept;
		CameraBuffer(const Mat4f& _projMat, const Mat4f& _viewInvMat, const Vec3f& _viewPosition) noexcept;
	};

	static constexpr uint32 __cameraBufferProjMatOffset = offsetof(CameraBuffer, projMat);
	static constexpr uint32 __cameraBufferViewOffset = offsetof(CameraBuffer, viewInfos);
}

#endif // GUARD
