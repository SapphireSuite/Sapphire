// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

#else

#include <GLFW/glfw3.h>

#endif

// include window after GLFW lib.
#include <Window/GLFW/System/GLFWWindow.hpp>

#include <Collections/Debug>


#if SA_WINDOW_API == SA_GLFW

namespace Sa::GLFW
{
	static uint32 sInitCount = 0u;

	void Window::Create(uint32 _width, uint32 _height, const char* _name)
	{
		// Init GLFW lib.
		if (sInitCount++ == 0)
			Init();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		mHandle = glfwCreateWindow(_width, _height, _name, nullptr, nullptr);
		
		SA_ASSERT(mHandle, CreationFailed, Window, L"GLFW create window failed!");

		glfwSetWindowUserPointer(mHandle, this);
		glfwSetWindowSizeCallback(mHandle, ResizeCallback);


		// TODO: Clean later:
		auto lambda = [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GLFW_TRUE);
		};

		glfwSetKeyCallback(mHandle, lambda);
		glfwSetInputMode(mHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // TODO: CLEAN LATER.
	}

	void Window::Destroy()
	{
		SA_ASSERT(mHandle, Nullptr, Window, L"Window handle nulltpr! Try to destroy a non-created window, call Create() first.");

		//onResizeEvent.Clear();

		glfwDestroyWindow(mHandle);

		// UnInit GLFW lib.
		if (--sInitCount == 0)
			UnInit();

		mHandle = nullptr;
	}

	void Window::Update()
	{
		glfwPollEvents();
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(mHandle);
	}

#if SA_RENDERING_API == SA_VULKAN

	VkSurfaceKHR_T* Window::CreateRenderSurface(const IRenderInstance& _instance) const
	{
		VkSurfaceKHR vkSurface;
		VkResult res = glfwCreateWindowSurface(
			_instance.As<Vk::RenderInstance>(),
			mHandle,
			nullptr,
			&vkSurface
		);

		SA_VK_ASSERT(res, CreationFailed, Window, L"Failed to create window surface!");

		return vkSurface;
	}

#endif

	void Window::ResizeCallback(GLFWwindow* _handle, int32 _width, int32 _height)
	{
		SA_ASSERT(_handle, Nullptr, Window, L"Window handle nulltpr!");

		// Resize callback event.
		IWindow* window = reinterpret_cast<IWindow*>(glfwGetWindowUserPointer(_handle));

		window->onResizeEvent(*window, static_cast<uint32>(_width), static_cast<uint32>(_height));
	}


	void Window::Init()
	{
		SA_ASSERT(glfwInit(), Init, Window, L"GLFW init failed!");
	}

	void Window::UnInit()
	{
		glfwTerminate();
	}

	void Window::GetRequiredExtensions(std::vector<const char*>& _extensions) noexcept
	{
		// Query extensions.
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = nullptr;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		_extensions.reserve(glfwExtensionCount);
		_extensions.insert(_extensions.end(), glfwExtensions, glfwExtensions + glfwExtensionCount);
	}


	Window::operator GLFWwindow* () const
	{
		return mHandle;
	}
}

#endif
