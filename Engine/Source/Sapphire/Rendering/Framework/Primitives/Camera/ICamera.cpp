// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Camera/ICamera.hpp>

namespace Sa
{
	// TODO: CLEAN LATER.
	ICamera* ICamera::mainCamera = nullptr;

	ICamera::ICamera(const ImageViewExtent& _viewport, const ImageViewExtent& _scissor) noexcept :
		mViewport{ _viewport },
		mScissor{ _scissor }
	{
	}

	ICamera::ICamera(const ImageViewExtent& _viewportScissor) noexcept :
		mViewport{ _viewportScissor },
		mScissor{ _viewportScissor }
	{
	}

	const ImageViewExtent& ICamera::GetViewport() const
	{
		return mViewport;
	}

	const ImageViewExtent& ICamera::GetScissor() const
	{
		return mScissor;
	}

	void ICamera::SetViewport(ImageViewExtent _viewport)
	{
		if (mViewport == _viewport)
			return;

		mViewport = _viewport;

		onViewportChange(*this, mViewport);
	}

	void ICamera::SetScissor(ImageViewExtent _scissor)
	{
		if (mScissor == _scissor)
			return;

		mScissor = _scissor;

		onScissorChange(*this, mScissor);
	}


#if SA_RENDERING_API == SA_VULKAN

	VkViewport ICamera::GetVkViewport() const
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

	VkRect2D ICamera::GetVkScissor() const
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
