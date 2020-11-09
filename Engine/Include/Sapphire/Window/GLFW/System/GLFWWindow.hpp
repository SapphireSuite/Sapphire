// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_WINDOW_GLFW_WINDOW_GUARD
#define SAPPHIRE_WINDOW_GLFW_WINDOW_GUARD

#include <Window/Framework/System/IWindow.hpp>

#if SA_WINDOW_API == SA_GLFW

struct GLFWwindow;

namespace Sa::GLFW
{
	class SA_ENGINE_API Window : public IWindow
	{
		GLFWwindow* mHandle = nullptr;

		static void ResizeCallback(GLFWwindow* _handle, int32 _width, int32 _height);

	public:
		void Create(uint32 _width, uint32 _height, const char* _name = "Main Window") override final;
		void Destroy() override final;

		void Update() override final;

		bool ShouldClose() const override final;

#if SA_RENDERING_API == SA_VULKAN

		VkSurfaceKHR_T* CreateRenderSurface(const IRenderInstance& _instance) const override final;

#endif

		static void GetRequiredExtensions(std::vector<const char*>& _extensions) noexcept;

		static void Init();
		static void UnInit();

		operator GLFWwindow*() const;
	};
}

#endif

#endif // GUARD