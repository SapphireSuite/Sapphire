// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Window/IWindow.hpp>

#include <Window/GLFWWindow.hpp>

namespace Sa
{
	void IWindow::Init()
	{
#if SA_WINDOW_API == SA_GLFW

		GLFWWindow::Init();

#endif
	}

	void IWindow::UnInit()
	{
#if SA_WINDOW_API == SA_GLFW

		GLFWWindow::UnInit();

#endif
	}

	void IWindow::GetRequiredExtensions(std::vector<const char*>& _extensions) noexcept
	{
#if SA_WINDOW_API == SA_GLFW

		GLFWWindow::GetRequiredExtensions(_extensions);

#endif
	}
}
