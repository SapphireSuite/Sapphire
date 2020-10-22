// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Camera/CameraBuffer.hpp>

#include <Core/Algorithms/Move.hpp>

namespace Sa
{
	CameraBuffer::CameraBuffer(Mat4f&& _projMat, Mat4f&& _viewInvMat, Vec3f&& _viewPosition) noexcept :
		projMat{ Move(_projMat) },
		viewInfos
		{
			Move(_viewInvMat),
			Move(_viewPosition)
		}
	{
	}

	CameraBuffer::CameraBuffer(const Mat4f& _projMat, const Mat4f& _viewInvMat, const Vec3f& _viewPosition) noexcept :
		projMat{ _projMat },
		viewInfos
		{
			_viewInvMat,
			_viewPosition
		}
	{
	}
}
