// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <string>
#include <iostream>

#include <Sapphire/Rendering/Window/GLFWWindow.hpp>
using namespace Sa;

#define LOG(_str) std::cout << _str << std::endl;

int main()
{
	LOG("=== Start ===");


	GLFWWindow window;
	window.Create(800u, 600u);

	while (!window.ShouldClose())
	{
	}

	window.Destroy();


	LOG("\n=== End ===");

	return 0;
}
