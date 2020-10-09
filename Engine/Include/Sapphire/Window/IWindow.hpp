// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_WINDOW_IWINDOW_GUARD
#define SAPPHIRE_WINDOW_IWINDOW_GUARD

#include <Core/Types/Int.hpp>
#include <Core/Types/IInterface.hpp>
#include <Core/Types/Variadics/Event.hpp>

#include <Window/Config.hpp>

#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

struct VkSurfaceKHR_T;

#endif

namespace Sa
{
	class IRenderInstance;

	class IWindow : public IInterface
	{
	protected:
#if SA_RENDERING_API == SA_VULKAN

		friend class VkRenderInstance;
		virtual VkSurfaceKHR_T* CreateRenderSurface(const IRenderInstance& _instance) const = 0;

#endif

	public:
		//mutable Event<void(const IWindow&, uint32, uint32)> onResizeEvent;

		virtual void Create(uint32 _width, uint32 _height, const char* _name = "Window") = 0;
		virtual void Destroy() = 0;

		virtual void Update() = 0;

		virtual bool ShouldClose() const = 0;


		SA_ENGINE_API static void Init();
		SA_ENGINE_API static void UnInit();

		static void GetRequiredExtensions(std::vector<const char*>& _extensions) noexcept;
	};
}

#endif // GUARD
