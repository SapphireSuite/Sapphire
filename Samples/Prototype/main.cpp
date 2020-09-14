// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <string>
#include <iostream>

#include <Sapphire/Window/GLFWWindow.hpp>
#include <Sapphire/Rendering/Vulkan/VkRenderInstance.hpp>
using namespace Sa;

#define LOG(_str) std::cout << _str << std::endl;

int main()
{
	LOG("=== Start ===");


	// Create.
	VkRenderInstance renderInstance;
	renderInstance.Create();

	GLFWWindow window;
	window.Create(800u, 600u);
	window.CreateRenderSurface(renderInstance);


	// Main Loop
	while (!window.ShouldClose())
	{
	}



	// Destroy.
	window.DestroyRenderSurface(renderInstance);
	window.Destroy();

	renderInstance.Destroy();


	LOG("\n=== End ===");

	return 0;
}
