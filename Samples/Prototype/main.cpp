// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <string>
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "../../Engine/Libs/stblib/tiny_obj_loader.h"

#include <Sapphire/Core/Time/Chrono.hpp>
#include <Sapphire/Maths/Space/Transform.hpp>

#include <Sapphire/Window/GLFWWindow.hpp>

#include <Sapphire/Rendering/Framework/Primitives/Mesh/IMesh.hpp>
#include <Sapphire/Rendering/Framework/Primitives/Light/ILight.hpp>
#include <Sapphire/Rendering/Framework/Primitives/Material/UniformBuffers.hpp>
#include <Sapphire/Rendering/Framework/Primitives/Material/IRenderMaterial.hpp>
#include <Sapphire/Rendering/Framework/Primitives/Pipeline/PipelineCreateInfos.hpp>

#include <Sapphire/Rendering/Vulkan/System/VkRenderInstance.hpp>

#include <Sapphire/Sdk/Asset/AssetManager.hpp>
#include <Sapphire/Sdk/Asset/Loaders/ObjLoader.hpp>
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


	// Create Lights.
	LightInfos pLight1;
	pLight1.position = Vec3f(-2.0f, -2.0f, -4.0f);
	pLight1.color = Vec3f(0.9f, 0.7f, 0.3f);

	ILight& plight1 = instance.InstantiatePointLight(pLight1);


	AssetManager assetMgr;

	// Load assets.
	const IShader* vertShader = assetMgr.Shader(instance, "../../Bin/Shaders/default_vert.spv");
	const IShader* fragShader = assetMgr.Shader(instance, "../../Bin/Shaders/default_frag.spv");

	std::vector<ModelCreateInfos> modelInfos;
	ObjLoader::Load("../../Engine/Resources/Models/Magikarp/Magikarp.obj", modelInfos);
	std::vector<IMesh*> Magikarp;
	for (auto it = modelInfos.begin(); it != modelInfos.end(); ++it)
		Magikarp.push_back(IMesh::CreateInstance(instance, it->vertices, it->indices));

	//Model Magikarp = assetMgr.Model(instance, "../../Engine/Resources/Models/Magikarp/Magikarp.obj");

	// Create Materials.
	IRenderMaterial* magikarpBodyMat = IRenderMaterial::CreateInstance();
	{
		const ITexture* magikarpBodyTexture = assetMgr.Texture(instance, "../../Engine/Resources/Models/Magikarp/Body.png");
		const ITexture* magikarpBodyNormTexture = assetMgr.Texture(instance, "../../Engine/Resources/Models/Magikarp/Body_norm.png");
		const ITexture* magikarpBodySpecTexture = assetMgr.Texture(instance, "../../Engine/Resources/Models/Magikarp/Body_spc.png");
		const ITexture* magikarpBodyPowTexture = assetMgr.Texture(instance, "../../Engine/Resources/Models/Magikarp/Body_pow.png");

		PipelineCreateInfos bodyPipelineInfos
		{
			surface,
			surface.GetViewport(),

			vertShader,
			fragShader,

			modelInfos[0].matConstants,

			{ magikarpBodyTexture, magikarpBodyNormTexture, magikarpBodySpecTexture, magikarpBodyPowTexture },

			PolygonMode::Fill,
			CullingMode::Back,
			FrontFaceMode::Clockwise,
			AlphaModel::Opaque,
			IlluminationModel::PBR
		};

		magikarpBodyMat->CreatePipeline(instance, bodyPipelineInfos);
	}

	IRenderMaterial* magikarpEyesMat = IRenderMaterial::CreateInstance();
	{
		const ITexture* magikarpEyesTexture = assetMgr.Texture(instance, "../../Engine/Resources/Models/Magikarp/Eyes.png");
		const ITexture* magikarpEyesNormTexture = assetMgr.Texture(instance, "../../Engine/Resources/Models/Magikarp/Eyes_norm.png");
		const ITexture* magikarpEyesSpecTexture = assetMgr.Texture(instance, "../../Engine/Resources/Models/Magikarp/Eyes_spc.png");
		const ITexture* magikarpEyesPowTexture = assetMgr.Texture(instance, "../../Engine/Resources/Models/Magikarp/Eyes_pow.png");

		PipelineCreateInfos eyesPipelineInfos
		{
			surface,
			surface.GetViewport(),

			vertShader,
			fragShader,

			modelInfos[1].matConstants,

			{ magikarpEyesTexture, magikarpEyesNormTexture, magikarpEyesSpecTexture, magikarpEyesPowTexture },

			PolygonMode::Fill,
			CullingMode::Back,
			FrontFaceMode::Clockwise,
			AlphaModel::Opaque,
			IlluminationModel::PBR
		};

		magikarpEyesMat->CreatePipeline(instance, eyesPipelineInfos);
	}

	{
		ObjectUniformBuffer oubo;
		oubo.modelMat = Mat4f::MakeScale(Vec3f(0.000001f)) * Mat4f::MakeRotation(Quatf(90_deg, Vec3f::Right));
		magikarpBodyMat->InitVariable(instance, &oubo, sizeof(oubo));
		magikarpEyesMat->InitVariable(instance, &oubo, sizeof(oubo));
	}


	const float r = 1.0f;
	const float l = -1.0f;
	const float t = 1.0f;
	const float b = -1.0f;
	const float n = 0.5f;
	const float f = 10.0f;

	Mat4f orthoMat
	(
		2 / (r - l), 0, 0, -(r + l) / (r - l),
		0, 2 / (t - b), 0, -(t + b) / (t - b),
		0, 0, -2 / (f - n), -(f + n) / (f - n),
		0, 0, 0, 1
	);


	const float fov = 60.0f;
	const float ratio = 1;
	const float scale = 1.f / tanf((fov / 2.f) * Maths::DegToRad);
	float tan_half_angle = std::tan(Maths::DegToRad * fov / 2);

	Mat4f perspMat
	(
		1 / (ratio * tan_half_angle), 0, 0, 0,
		0, 1 / (tan_half_angle), 0, 0,
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

		VkRenderFrame frame = surface.GetSwapChain().Update(instance.GetDevice());


		window.TEST(camTr, speed * deltaTime);

		// Update Uniform Buffer.
		StaticUniformBuffer ubo;
		//ubo.modelMat = Mat4f::MakeRotation(Quatf(time, Vec3f::Right));
		ubo.viewInvMat = camTr.Matrix().GetTransposed().Inverse();
		ubo.projMat = perspMat;

		frame.uniformBuffer.UpdateData(instance.GetDevice(), &ubo, sizeof(ubo));
		

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

		magikarpBodyMat->Bind(frame);
		Magikarp[0]->Draw(frame);

		magikarpEyesMat->Bind(frame);
		Magikarp[1]->Draw(frame);

		//Magikarp.Draw(frame);

		vkCmdEndRenderPass(frame.graphicsCommandBuffer);

		SA_VK_ASSERT(vkEndCommandBuffer(frame.graphicsCommandBuffer),
			LibCommandFailed, Rendering, L"Failed to end command buffer!");
	}



	// === Destroy ===
	vkDeviceWaitIdle(instance.GetDevice());

	//Magikarp.GetMaterial(0)->DestroyPipeline(instance);
	//Magikarp.GetMaterial(1)->DestroyPipeline(instance);
	magikarpBodyMat->DestroyPipeline(instance);
	magikarpEyesMat->DestroyPipeline(instance);
	assetMgr.Free(instance);

	instance.DestroyRenderSurface(window);
	
	window.Destroy();

	instance.Destroy();


	LOG("\n=== End ===");

	return 0;
}
