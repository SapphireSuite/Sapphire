// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_WINDOW_GLFW_WINDOW_GUARD
#define SAPPHIRE_WINDOW_GLFW_WINDOW_GUARD

#include <Window/IWindow.hpp>


// TODO: REMOVE LATER.
#include <Maths/Space/Transform.hpp>


#if SA_WINDOW_API == SA_GLFW

namespace Sa
{
	class GLFWWindow : public IWindow
	{
		static uint32 sInitCount;

		struct GLFWwindow* mHandle = nullptr;

		static void ResizeCallback(GLFWwindow* _handle, int32 _width, int32 _height);

#if SA_RENDERING_API == SA_VULKAN

		VkSurfaceKHR_T* CreateRenderSurface(const IRenderInstance& _instance) const override final;

#endif

	public:
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Create(uint32 _width, uint32 _height, const char* _name = "Main Window") override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Destroy() override final;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Update() override final;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API bool ShouldClose() const override final;

		operator GLFWwindow*() const;

		// TODO: REMOVE LATER.
		SA_ENGINE_API void TEST(TransffPRS& _camTr, Vec3f& _lightPos, float _deltaTime);

		static void GetRequiredExtensions(std::vector<const char*>& _extensions) noexcept;

		static void Init();
		static void UnInit();
	};
}

#endif

#endif // GUARD