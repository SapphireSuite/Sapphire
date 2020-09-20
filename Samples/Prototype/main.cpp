// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <string>
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "../../Engine/Libs/stblib/tiny_obj_loader.h"

#include <Sapphire/Core/Time/Chrono.hpp>
#include <Sapphire/Maths/Space/Transform.hpp>

#include <Sapphire/Window/GLFWWindow.hpp>

#include <Sapphire/Rendering/Vulkan/VkRenderInstance.hpp>
#include <Sapphire/Rendering/Framework/UniformBufferObject.hpp>
#include <Sapphire/Rendering/Framework/Primitives/IRenderMaterial.hpp>
#include <Sapphire/Rendering/Framework/Primitives/Pipeline/PipelineCreateInfos.hpp>

#include <Sapphire/Sdk/Asset/AssetManager.hpp>
#include <Sapphire/Sdk/Model.hpp>

using namespace Sa;

#define LOG(_str) std::cout << _str << std::endl;


int main()
{
	LOG("=== Start ===");


	// === Create ===
	VkRenderInstance instance;
	instance.Create();

	GLFWWindow window;
	window.Create(800u, 800u);

	VkRenderSurface& surface = const_cast<VkRenderSurface&>(static_cast<const VkRenderSurface&>(instance.CreateRenderSurface(window)));


	AssetManager assetMgr;

	// Load assets..
	const IShader* unlitVertShader = assetMgr.Shader(instance, "../../Bin/Shaders/default_unlit_vert.spv");
	const IShader* unlitFragShader = assetMgr.Shader(instance, "../../Bin/Shaders/default_unlit_frag.spv");

	const ITexture* magikarpBodyTexture = assetMgr.Texture(instance, "../../Engine/Resources/Models/Magikarp/Body.png");
	const ITexture* magikarpEyesTexture = assetMgr.Texture(instance, "../../Engine/Resources/Models/Magikarp/Eyes.png");

	Model Magikarp = assetMgr.Model(instance, "../../Engine/Resources/Models/Magikarp/Magikarp.obj");

	// Create Materials.
	PipelineCreateInfos bodyPipelineInfos
	{
		surface,
		surface.GetViewport(),

		unlitVertShader,
		unlitFragShader,

		{ magikarpBodyTexture },

		PolygonMode::Fill,
		CullingMode::None,
		FrontFaceMode::Clockwise
	};

	Magikarp.GetMaterial(0)->CreatePipeline(instance, bodyPipelineInfos);

	PipelineCreateInfos eyesPipelineInfos
	{
		surface,
		surface.GetViewport(),

		unlitVertShader,
		unlitFragShader,

		{ magikarpEyesTexture },

		PolygonMode::Fill,
		CullingMode::None,
		FrontFaceMode::Clockwise
	};

	Magikarp.GetMaterial(1)->CreatePipeline(instance, eyesPipelineInfos);


	const float r = 1.0f;
	const float l = -1.0f;
	const float t = 1.0f;
	const float b = -1.0f;
	const float n = 0.1f;
	const float f = 15.0f;

	Mat4f orthoMat
	(
		2 / (r - l), 0, 0, -(r + l) / (r - l),
		0, 2 / (t - b), 0, -(t + b) / (t - b),
		0, 0, -2 / (f - n), -(f + n) / (f - n),
		0, 0, 0, 1
	);


	const float fov = 90.0f;
	const float ratio = 1;
	const float scale = 1.f / tanf((fov / 2.f) * Maths::DegToRad);
	float tan_half_angle = std::tan(Maths::DegToRad * fov / 2);

	Mat4f perspMat
	(
		1 / (ratio * tan_half_angle), 0, 0, 0,
		0, -1 / (tan_half_angle), 0, 0,
		0, 0, -(f + n) / (f - n), -(2 * f * n) / (f - n),
		0, 0, -1, 1
	);


	Transff camTr;
	camTr.position.z = 1.0f;

	Chrono chrono;
	float time = 0.0f;
	float speed = 0.1f;

	// Main Loop
	while (!window.ShouldClose())
	{
		float deltaTime = chrono.Restart() * 0.00005f;
		time += deltaTime;

		window.Update();
		instance.Update();

		window.TEST(camTr, speed * deltaTime);


		// Update Uniform Buffer.
		UniformBufferObject ubo;
		ubo.modelMat = Mat4f::MakeScale(Vec3f(0.000001f)) * Mat4f::MakeRotation(Quatf(90_deg, Vec3f::Right));
		//ubo.modelMat = Mat4f::MakeRotation(Quatf(time, Vec3f::Right));
		ubo.viewMat = camTr.Matrix().GetTransposed();
		ubo.projMat = orthoMat;


		VkRenderFrame frame = surface.GetSwapChain().Update(instance.GetDevice());

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


		VkClearValue clearValue[2];
		clearValue[0].color = VkClearColorValue{ 0.0f, 0.0f, 0.07f, 1.0f };
		clearValue[1].depthStencil = VkClearDepthStencilValue{ 1.0f, 0 };


		const VkRenderPassBeginInfo renderPassBeginInfo
		{
			VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,							// sType.
			nullptr,															// pNext.
			surface.GetRenderPass(),											// renderPass.
			frame.frameBuffer,													// framebuffer
			VkRect2D{ VkOffset2D{}, surface.GetImageExtent() },					// renderArea.
			sizeof(clearValue) / sizeof(VkClearValue),							// clearValueCount.
			clearValue															// pClearValues.
		};

		vkCmdBeginRenderPass(frame.graphicsCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		Magikarp.Draw(frame);

		vkCmdEndRenderPass(frame.graphicsCommandBuffer);

		SA_VK_ASSERT(vkEndCommandBuffer(frame.graphicsCommandBuffer),
			LibCommandFailed, Rendering, L"Failed to end command buffer!");
	}



	// === Destroy ===
	vkDeviceWaitIdle(instance.GetDevice());

	Magikarp.GetMaterial(0)->DestroyPipeline(instance);
	Magikarp.GetMaterial(1)->DestroyPipeline(instance);
	assetMgr.Free(instance);

	instance.DestroyRenderSurface(window);
	
	window.Destroy();

	instance.Destroy();


	LOG("\n=== End ===");

	return 0;
}
