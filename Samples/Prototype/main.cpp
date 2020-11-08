// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <string>
#include <iostream>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>
using namespace Sa;

#define LOG(_str) std::cout << _str << std::endl;


int main()
{
	LOG("=== Start ===\n");


	LOG("=== Init ===");
	IRenderInstance::Init();


	LOG("=== Create ===");
	Vk::RenderInstance instance;
	instance.Create();
	
	
	LOG("=== Destroy ===");
	instance.Destroy();


	LOG("=== UnInit ===");
	IRenderInstance::UnInit();


	LOG("\n=== End ===");

	return 0;
}
