// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_CAMERA_GUARD
#define SAPPHIRE_RENDERING_CAMERA_GUARD

#include <Core/Types/Variadics/Event.hpp>

#include <Rendering/Image/ImageViewExtent.hpp>

namespace Sa
{
	class Camera
	{
		ImageViewExtent mViewport;
		ImageViewExtent mScissor;

	public:
		Event<void(Camera&, ImageViewExtent)> onViewportChange;
		Event<void(Camera&, ImageViewExtent)> onScissorChange;

		Camera() = default;
		SA_ENGINE_API Camera(const ImageViewExtent& _viewport, const ImageViewExtent& _scissor) noexcept;
		SA_ENGINE_API Camera(const ImageViewExtent& _viewportScissor) noexcept;

		const ImageViewExtent& GetViewport() const;
		const ImageViewExtent& GetScissor() const;

		void SetViewport(ImageViewExtent _viewport);
		void SetScissor(ImageViewExtent _scissor);

#if SA_RENDERING_API == SA_VULKAN

		VkViewport GetVkViewport() const;

		VkRect2D GetVkScissor() const;

#endif
	};
}

#endif // GUARD
