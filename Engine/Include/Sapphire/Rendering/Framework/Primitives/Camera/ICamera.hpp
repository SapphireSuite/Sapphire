// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_ICAMERA_GUARD
#define SAPPHIRE_RENDERING_ICAMERA_GUARD

#include <Core/Types/Variadics/Event.hpp>
#include <Core/Types/IInterface.hpp>

#include <Rendering/Image/ImageViewExtent.hpp>

namespace Sa
{
	class IRenderInstance;
	class IRenderSurface;

	class ICamera : public IInterface
	{
		ImageViewExtent mViewport;
		ImageViewExtent mScissor;

	public:
		Event<void(ICamera&, ImageViewExtent)> onViewportChange;
		Event<void(ICamera&, ImageViewExtent)> onScissorChange;

		ICamera() = default;
		SA_ENGINE_API ICamera(const ImageViewExtent& _viewport, const ImageViewExtent& _scissor) noexcept;
		SA_ENGINE_API ICamera(const ImageViewExtent& _viewportScissor) noexcept;
		virtual ~ICamera() = default;

		const ImageViewExtent& GetViewport() const;
		const ImageViewExtent& GetScissor() const;

		void SetViewport(ImageViewExtent _viewport);
		void SetScissor(ImageViewExtent _scissor);

		virtual void Create(const IRenderInstance& _instance, const IRenderSurface& _surface) = 0;
		virtual void Destroy(const IRenderInstance& _instance) = 0;

#if SA_RENDERING_API == SA_VULKAN

		VkViewport GetVkViewport() const;

		VkRect2D GetVkScissor() const;

#endif
	};
}

#endif // GUARD
