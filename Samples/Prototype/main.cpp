// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <string>
#include <iostream>

#include <Sapphire/Core/Time/Chrono.hpp>
#include <Sapphire/Maths/Misc/Maths.hpp>
#include <Sapphire/Maths/Misc/Degree.hpp>
#include <Sapphire/Maths/Misc/Radian.hpp>
#include <Sapphire/Maths/Space/Quaternion.hpp>
#include <Sapphire/Maths/Space/Matrix4.hpp>

#include <Sapphire/Window/GLFWWindow.hpp>
#include <Sapphire/Rendering/Vulkan/Model/VkMesh.hpp>
#include <Sapphire/Rendering/Framework/Model/UniformBufferObject.hpp>
#include <Sapphire/Rendering/Vulkan/VkRenderInstance.hpp>
#include <Sapphire/Rendering/Vulkan/Primitives/Pipeline/VkShader.hpp>
#include <Sapphire/Rendering/Vulkan/Primitives/Pipeline/VkRenderPipeline.hpp>
using namespace Sa;

#define LOG(_str) std::cout << _str << std::endl;

int main()
{
	LOG("=== Start ===");


	// Create.
	VkRenderInstance instance;
	instance.Create();

	GLFWWindow window;
	window.Create(800u, 600u);

	VkRenderSurface& surface = const_cast<VkRenderSurface&>(static_cast<const VkRenderSurface&>(instance.CreateRenderSurface(window)));

	const std::vector<Vertex> vertices =
	{
		{ { -0.5f, -0.5f, 0.0f }, Vec3f::Forward, { 1.0f, 0.0f, 0.0f } },
		{ { 0.5f, -0.5f, 0.0f }, Vec3f::Forward, { 0.0f, 1.0f, 0.0f } },
		{ { 0.5f, 0.5f, 0.0f }, Vec3f::Forward, { 0.0f, 0.0f, 1.0f } },
		{ { -0.5f, 0.5f, 0.0f }, Vec3f::Forward, { 1.0f, 1.0f, 1.0f } },
	};
	
	const std::vector<uint32> indices = {0, 1, 2, 2, 3, 0};

	VkMesh mesh;
	mesh.Create(instance, vertices, indices);

	VkShader vertShader;
	vertShader.Create(instance, ShaderType::Vertex, L"../../Bin/Shaders/default_vert.spv");

	VkShader fragShader;
	fragShader.Create(instance, ShaderType::Fragment, L"../../Bin/Shaders/default_frag.spv");

	VkRenderPipeline pipeline;
	pipeline.Create(instance, surface, { &vertShader, &fragShader }, surface.GetViewport());

	Chrono chrono;
	float time = 0.0f;

	// Main Loop
	while (!window.ShouldClose())
	{
		float deltaTime = chrono.Restart();
		time += deltaTime * 0.00001f;

		instance.Update();

		VkRenderFrame frame = surface.GetSwapChain().Update(instance.GetDevice());

		// Update Uniform Buffer.
		UniformBufferObject ubo;
		ubo.modelMat = (Quatf(time, Vec3f::Forward)).Matrix();
		ubo.projMat.At(0, 0) = -1;			// 2/(r - l)
		ubo.projMat.At(1, 1) = -1;			// 2/(t - b)
		ubo.projMat.At(2, 2) = -2 / 10;		//  -2(f - n)
		ubo.projMat.At(0, 3) = 0;			// -(r + l)/(r - l)
		ubo.projMat.At(1, 3) = 0;			// -(t + b)/(t - b)
		ubo.projMat.At(2, 3) = 0;			// -(f + n)/(f - n)

		void* data;
		vkMapMemory(instance.GetDevice(), frame.uniformBuffer, 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(instance.GetDevice(), frame.uniformBuffer);


		const VkCommandBufferBeginInfo commandBufferBeginInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			nullptr,
			VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
		};

		SA_VK_ASSERT(vkBeginCommandBuffer(frame.graphicsCommandBuffer, &commandBufferBeginInfo),
			LibCommandFailed, Rendering, L"Failed to begin command buffer!");



		const VkClearValue clearValue{ VkClearColorValue{ 0.0f, 0.0f, 0.07f, 1.0f } };

		const VkRenderPassBeginInfo renderPassBeginInfo
		{
			VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,							// sType.
			nullptr,															// pNext.
			surface.GetRenderPass(),											// renderPass.
			frame.frameBuffer,													// framebuffer
			VkRect2D{ VkOffset2D{}, surface.GetImageExtent() },					// renderArea.
			1,																	// clearValueCount.
			&clearValue															// pClearValues.
		};

		vkCmdBeginRenderPass(frame.graphicsCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(frame.graphicsCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

		vkCmdBindDescriptorSets(frame.graphicsCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline, 0, 1, &frame.descriptorSet, 0, nullptr);

		mesh.Draw(frame);

		vkCmdEndRenderPass(frame.graphicsCommandBuffer);

		SA_VK_ASSERT(vkEndCommandBuffer(frame.graphicsCommandBuffer),
			LibCommandFailed, Rendering, L"Failed to end command buffer!");
	}



	// Destroy.
	pipeline.Destroy(instance);

	vertShader.Destroy(instance);
	fragShader.Destroy(instance);

	instance.DestroyRenderSurface(window);
	
	window.Destroy();

	instance.Destroy();


	LOG("\n=== End ===");

	return 0;
}
