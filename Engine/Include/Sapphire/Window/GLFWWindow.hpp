// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_WINDOW_GLFW_WINDOW_GUARD
#define SAPPHIRE_WINDOW_GLFW_WINDOW_GUARD

#include <Window/IWindow.hpp>

#if SA_WINDOW_API == SA_GLFW

namespace Sa
{
	class GLFWWindow : public IWindow
	{
		static uint32 sInitCount;

		struct GLFWwindow* mHandle = nullptr;

		void Init();
		void UnInit();

		static void ResizeCallback(GLFWwindow* _handle, int32 _width, int32 _height);

	public:
		SA_ENGINE_API void Create(uint32 _width, uint32 _height, const char* _name = "Main Window") override final;
		SA_ENGINE_API void Destroy() override final;

		SA_ENGINE_API bool ShouldClose() const override final;

		operator GLFWwindow*() const;
	};
}

#endif

#endif // GUARD