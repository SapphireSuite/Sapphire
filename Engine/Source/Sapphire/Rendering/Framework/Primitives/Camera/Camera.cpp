// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Camera/Camera.hpp>

namespace Sa
{
	// TODO: CLEAN LATER.
	Camera* Camera::mainCamera = nullptr;

	Camera::Camera(const ImageViewExtent& _viewport, const ImageViewExtent& _scissor) noexcept :
		mViewport{ _viewport },
		mScissor{ _scissor }
	{
	}

	Camera::Camera(const ImageViewExtent& _viewportScissor) noexcept :
		mViewport{ _viewportScissor },
		mScissor{ _viewportScissor }
	{
	}

	const ImageViewExtent& Camera::GetViewport() const
	{
		return mViewport;
	}

	const ImageViewExtent& Camera::GetScissor() const
	{
		return mScissor;
	}

	void Camera::SetViewport(ImageViewExtent _viewport)
	{
		if (mViewport == _viewport)
			return;

		mViewport = _viewport;

		onViewportChange(*this, mViewport);
	}

	void Camera::SetScissor(ImageViewExtent _scissor)
	{
		if (mScissor == _scissor)
			return;

		mScissor = _scissor;

		onScissorChange(*this, mScissor);
	}


#if SA_RENDERING_API == SA_VULKAN

	VkViewport Camera::GetVkViewport() const
	{
		return VkViewport
		{
			static_cast<float>(mViewport.offset.x),								// x.
			static_cast<float>(mViewport.offset.y),								// y.
			static_cast<float>(mViewport.extent.width),							// width.
			static_cast<float>(mViewport.extent.height),						// height.
			0.0f,																// minDepth.
			1.0f,																// maxDepth.
		};
	}

	VkRect2D Camera::GetVkScissor() const
	{
		return VkRect2D
		{
			VkOffset2D															// offset.
			{
				static_cast<int32>(mScissor.offset.x),
				static_cast<int32>(mScissor.offset.y)
			},

			mScissor.extent, 													// extent.
		};
	}

#endif
}
