// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <string>
#include <iostream>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>
#include <Rendering/Vulkan/System/VkRenderPass.hpp>
#include <Rendering/Vulkan/System/Surface/VkRenderSurface.hpp>
#include <Rendering/Vulkan/Buffers/VkFrameBuffer.hpp>
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

	// Main RenderPass.
	Vk::RenderPass mainRP;
	const RenderPassDescriptor mainRPDesc = RenderPassDescriptor::CreateDefaultPBRDeferred(&surface);
	mainRP.Create(instance, mainRPDesc);
	const uint32 mainRPID = surface.AddRenderPass(instance, mainRP, mainRPDesc);

	// UI RenderPass.
	Vk::RenderPass UIRP;
	const RenderPassDescriptor UIRPDesc = RenderPassDescriptor::CreateDefaultForward(&surface);
	UIRP.Create(instance, UIRPDesc);
	const uint32 UIRPID = surface.AddRenderPass(instance, UIRP, UIRPDesc);


	LOG("=== Loop ===");
	while (!window.ShouldClose())
	{
		window.Update();

		// Begin Surface.
		surface.Begin(instance);


		// Main framebuffer.
		IFrameBuffer& mainFB = surface.GetFrameBuffer(mainRPID);

		mainFB.Begin();

		mainFB.NextSubpass();

		mainFB.End();


		// UI framebuffer.
		IFrameBuffer& UIFB = surface.GetFrameBuffer(UIRPID);

		UIFB.Begin();

		UIFB.End();


		// End Surface.
		surface.End(instance);
	}

	
	LOG("=== Destroy ===");
	vkDeviceWaitIdle(instance.device);

	surface.RemoveRenderPass(instance, UIRPID);
	surface.RemoveRenderPass(instance, mainRPID);

	UIRP.Destroy(instance);
	mainRP.Destroy(instance);

	instance.DestroyRenderSurface(surface);

	window.Destroy();

	instance.Destroy();


	LOG("=== UnInit ===");
	IWindow::UnInit();
	IRenderInstance::UnInit();


	LOG("\n=== End ===");

	return 0;
}
