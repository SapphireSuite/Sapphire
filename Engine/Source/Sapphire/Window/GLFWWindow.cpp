// Copyright 2020 Sapphire development team. All Rights Reserved.

// TODO: Clean later.
#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include <Rendering/Vulkan/VkRenderInstance.hpp>
#include <Rendering/Vulkan/VkRenderSurface.hpp>

#else

#include <GLFW/glfw3.h>

#endif


#include <Window/GLFWWindow.hpp>

#include <Core/Debug/Debug.hpp>


#if SA_WINDOW_API == SA_GLFW

namespace Sa
{
	uint32 GLFWWindow::sInitCount = 0u;


	void GLFWWindow::Create(uint32 _width, uint32 _height, const char* _name)
	{
		// Init GLFW lib.
		if (sInitCount++ == 0)
		{
			bool res = glfwInit();
			SA_ASSERT(res, Init, Window, L"GLFW init failed!")
		}


		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		mHandle = glfwCreateWindow(_width, _height, _name, nullptr, nullptr);
		
		SA_ASSERT(mHandle, CreationFailed, Window, L"GLFW create window failed!")
	}

	void GLFWWindow::Destroy()
	{
		SA_ASSERT(mHandle, Nullptr, Window, L"Window handle nulltpr! Try to destroy a non-created window, call Create() first.")

		glfwDestroyWindow(mHandle);

		// UnInit GLFW lib.
		if (--sInitCount == 0)
			glfwTerminate();

		mHandle = nullptr;
	}

	bool GLFWWindow::ShouldClose() const
	{
		return glfwWindowShouldClose(mHandle);
	}

	GLFWWindow::operator GLFWwindow*() const
	{
		return mHandle;
	}
}

#endif
