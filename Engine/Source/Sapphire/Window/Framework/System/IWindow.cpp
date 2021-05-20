// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Window/Framework/System/IWindow.hpp>

#include <Window/GLFW/System/GLFWWindow.hpp>

namespace Sa
{
	void IWindow::Init()
	{
#if SA_WINDOW_API == SA_GLFW

		GLFW::Window::Init();

#endif
	}

	void IWindow::UnInit()
	{
#if SA_WINDOW_API == SA_GLFW

		GLFW::Window::UnInit();

#endif
	}

	void IWindow::GetRequiredExtensions(std::vector<const char*>& _extensions) noexcept
	{
#if SA_WINDOW_API == SA_GLFW

		GLFW::Window::GetRequiredExtensions(_extensions);

#endif
	}
}
