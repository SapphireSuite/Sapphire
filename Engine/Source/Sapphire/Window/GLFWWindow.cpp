// Copyright 2020 Sapphire development team. All Rights Reserved.

// TODO: Clean later.
#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>
#include <Rendering/Vulkan/System/VkRenderSurface.hpp>

#else

#include <GLFW/glfw3.h>

#endif


#include <Window/GLFWWindow.hpp>

#include <Core/Debug/Debug.hpp>


#if SA_WINDOW_API == SA_GLFW

namespace Sa
{
	uint32 GLFWWindow::sInitCount = 0u;

	// TODO: Clean later:
	static double oldX = 0.0f;
	static double oldY = 0.0f;
	static double mouseX = 0.0f;
	static double mouseY = 0.0f;
	static float dx = 0.0f;
	static float dy = 0.0f;

	void GLFWWindow::Init()
	{
		SA_ASSERT(glfwInit(), Init, Window, L"GLFW init failed!");
	}

	void GLFWWindow::UnInit()
	{
		glfwTerminate();
	}

	void GLFWWindow::Create(uint32 _width, uint32 _height, const char* _name)
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
		glfwSetInputMode(mHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwGetCursorPos(mHandle, &mouseX, &mouseY);
		oldX = mouseX;
		oldY = mouseY;
	}

	void GLFWWindow::Destroy()
	{
		SA_ASSERT(mHandle, Nullptr, Window, L"Window handle nulltpr! Try to destroy a non-created window, call Create() first.");

		onResizeEvent.Clear();

		glfwDestroyWindow(mHandle);

		// UnInit GLFW lib.
		if (--sInitCount == 0)
			UnInit();

		mHandle = nullptr;
	}

	void GLFWWindow::Update()
	{
		static uint32 sUpdateCount = 0u;

		sUpdateCount = (sUpdateCount + 1) % sInitCount;
		
		if(sUpdateCount == 0)
			glfwPollEvents();
	}

	bool GLFWWindow::ShouldClose() const
	{
		return glfwWindowShouldClose(mHandle);
	}

	void GLFWWindow::ResizeCallback(GLFWwindow* _handle, int32 _width, int32 _height)
	{
		SA_ASSERT(_handle, Nullptr, Window, L"Window handle nulltpr!");
		
		// Window minimized.
		while (_width == 0 || _height == 0)
			glfwWaitEvents();

		// Resize callback event.
		IWindow* window = reinterpret_cast<IWindow*>(glfwGetWindowUserPointer(_handle));

		window->onResizeEvent(*window, static_cast<uint32>(_width), static_cast<uint32>(_height));
	}

	GLFWWindow::operator GLFWwindow*() const
	{
		return mHandle;
	}


	void GLFWWindow::TEST(TransffPRS& _camTr, Vec3f& _lightPos, float _deltaTime)
	{
		if (glfwGetKey(mHandle, GLFW_KEY_D) == GLFW_PRESS)
			_camTr.position += _deltaTime * _camTr.RightVector();
		if (glfwGetKey(mHandle, GLFW_KEY_A) == GLFW_PRESS)
			_camTr.position -= _deltaTime * _camTr.RightVector();
		if (glfwGetKey(mHandle, GLFW_KEY_Q) == GLFW_PRESS)
			_camTr.position -= _deltaTime * _camTr.UpVector();
		if (glfwGetKey(mHandle, GLFW_KEY_E) == GLFW_PRESS)
			_camTr.position += _deltaTime * _camTr.UpVector();
		if (glfwGetKey(mHandle, GLFW_KEY_W) == GLFW_PRESS)
			_camTr.position -= _deltaTime * _camTr.ForwardVector();
		if (glfwGetKey(mHandle, GLFW_KEY_S) == GLFW_PRESS)
			_camTr.position += _deltaTime * _camTr.ForwardVector();

		glfwGetCursorPos(mHandle, &mouseX, &mouseY);

		if (mouseX != oldX || mouseY != oldY)
		{
			dx -= static_cast<float>(mouseX - oldX) * _deltaTime * 10.0f * Maths::DegToRad;
			dy += static_cast<float>(mouseY - oldY) * _deltaTime * 10.0f * Maths::DegToRad;

			oldX = mouseX;
			oldY = mouseY;

			dx = dx > Maths::Pi ? dx - Maths::Pi : dx < -Maths::Pi ? dx + Maths::Pi : dx;
			dy = dy > Maths::Pi ? dy - Maths::Pi : dy < -Maths::Pi ? dy + Maths::Pi : dy;

			_camTr.rotation = Quatf(cos(dx), 0, sin(dx), 0) * Quatf(cos(dy), sin(dy), 0, 0);
		}



		if (glfwGetKey(mHandle, GLFW_KEY_J) == GLFW_PRESS)
			_lightPos -= _deltaTime * Vec3f::Right;
		if (glfwGetKey(mHandle, GLFW_KEY_L) == GLFW_PRESS)
			_lightPos += _deltaTime * Vec3f::Right;
		if (glfwGetKey(mHandle, GLFW_KEY_U) == GLFW_PRESS)
			_lightPos -= _deltaTime * Vec3f::Up;
		if (glfwGetKey(mHandle, GLFW_KEY_O) == GLFW_PRESS)
			_lightPos += _deltaTime * Vec3f::Up;
		if (glfwGetKey(mHandle, GLFW_KEY_I) == GLFW_PRESS)
			_lightPos -= _deltaTime * Vec3f::Forward;
		if (glfwGetKey(mHandle, GLFW_KEY_K) == GLFW_PRESS)
			_lightPos += _deltaTime * Vec3f::Forward;
	}
}

#endif
