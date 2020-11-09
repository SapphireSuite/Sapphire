// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <string>
#include <iostream>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>
#include <Window/GLFW/System/GLFWWindow.hpp>
using namespace Sa;

#define LOG(_str) std::cout << _str << std::endl;


int main()
{
	LOG("=== Start ===\n");


	LOG("=== Init ===");
	IRenderInstance::Init();
	IWindow::Init();


	LOG("=== Create ===");
	Vk::RenderInstance instance;
	instance.Create();
	
	GLFW::Window window;
	window.Create(1200u, 800u);

	IRenderSurface& surface = instance.CreateRenderSurface(window);


	LOG("=== Loop ===");
	while (!window.ShouldClose())
	{
		window.Update();
	}

	
	LOG("=== Destroy ===");
	instance.DestroyRenderSurface(surface);

	window.Destroy();

	instance.Destroy();


	LOG("=== UnInit ===");
	IWindow::UnInit();
	IRenderInstance::UnInit();


	LOG("\n=== End ===");

	return 0;
}
