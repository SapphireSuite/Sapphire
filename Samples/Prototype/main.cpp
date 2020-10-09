// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <string>
#include <iostream>

#include <Sapphire/Core/Time/Chrono.hpp>
#include <Sapphire/Maths/Space/Transform.hpp>

#include <Sapphire/Window/GLFWWindow.hpp>

#include <Sapphire/Rendering/Misc/APISpecific.hpp>
#include <Sapphire/Rendering/Framework/Primitives/Mesh/IMesh.hpp>
#include <Sapphire/Rendering/Framework/Primitives/Material/UniformBuffers.hpp>
#include <Sapphire/Rendering/Framework/Primitives/Material/IRenderMaterial.hpp>
#include <Sapphire/Rendering/Framework/Primitives/Pipeline/PipelineCreateInfos.hpp>

#include <Sapphire/Rendering/Vulkan/System/VkRenderInstance.hpp>

#include <Sapphire/SDK/Resources/Assets/AssetManager.hpp>

using namespace Sa;

#define LOG(_str) std::cout << _str << std::endl;


// Magikarp
IMesh* magikarpBodyMesh = nullptr;
IMesh* magikarpEyesMesh = nullptr;

IRenderMaterial* magikarpBodyMat = nullptr;
IRenderMaterial* magikarpEyesMat = nullptr;

// Gizmo
IMesh* gizmoMesh = nullptr;
IRenderMaterial* gizmoMat = nullptr;

void CreateResources(AssetManager& _assetMgr)
{
	// === Magikarp ===
	{
		constexpr const char* vertShaderAsset = "Bin/Magikarp/Magikarp_VS.spha";
		constexpr const char* fragShaderAsset = "Bin/Magikarp/Magikarp_FS.spha";

		constexpr const char* textureAssets[] = { "Bin/Magikarp/Body_T.spha", "Bin/Magikarp/Eyes_T.spha" };
		constexpr const char* meshAssets[] = { "Bin/Magikarp/Body_M.spha", "Bin/Magikarp/Eyes_M.spha" };
		constexpr const char* materialAssets[] = { "Bin/Magikarp/Body_Mat.spha", "Bin/Magikarp/Eyes_Mat.spha" };

		// Vertex Shader.
		if (!_assetMgr.shaderMgr.Load(vertShaderAsset, true)) // Try load.
		{
			// Import on load failed.
			_assetMgr.shaderMgr.Import("../../Engine/Resources/Shaders/default.vert", { vertShaderAsset });
		}

		// Fragment Shader.
		if (!_assetMgr.shaderMgr.Load(fragShaderAsset, true)) // Try load.
		{
			// Import on load failed.
			_assetMgr.shaderMgr.Import("../../Engine/Resources/Shaders/default.frag", { fragShaderAsset });
		}


		// Body Texture.
		if (!_assetMgr.textureMgr.Load(textureAssets[0], true)) // Try load.
		{
			// Import on load failed.
			_assetMgr.textureMgr.Import("../../Engine/Resources/Models/Magikarp/Body.png", { textureAssets[0] });
		}


		// Eyes Texture.
		if (!_assetMgr.textureMgr.Load(textureAssets[1], true)) // Try load.
		{
			// Import on load failed.
			_assetMgr.textureMgr.Import("../../Engine/Resources/Models/Magikarp/Eyes.png", { textureAssets[1] });
		}


		// Meshes.
		magikarpBodyMesh = _assetMgr.meshMgr.Load(meshAssets[0]);
		magikarpEyesMesh = _assetMgr.meshMgr.Load(meshAssets[1]);

		if (!magikarpBodyMesh || !magikarpEyesMesh) // Try load.
		{
			// Import on load failed.
			auto meshes = _assetMgr.meshMgr.Import("../../Engine/Resources/Models/Magikarp/Magikarp.obj", { { meshAssets[0] , meshAssets[1] } });

			magikarpBodyMesh = meshes[0].GetResource();
			magikarpEyesMesh = meshes[1].GetResource();
		}

		// Materials.
		{
			magikarpBodyMat = _assetMgr.renderMaterialMgr.Load(materialAssets[0]);
			magikarpEyesMat = _assetMgr.renderMaterialMgr.Load(materialAssets[1]);

			if (!magikarpBodyMat || !magikarpEyesMat) // Try load.
			{
				// Import on load failed.
				auto materials = _assetMgr.renderMaterialMgr.Import("../../Engine/Resources/Models/Magikarp/Magikarp.mtl",
					{ { materialAssets[0] , materialAssets[1] } });

				materials[0].infos.vertexShaderPath = vertShaderAsset;
				materials[1].infos.vertexShaderPath = vertShaderAsset;

				materials[0].infos.fragmentShaderPath = fragShaderAsset;
				materials[1].infos.fragmentShaderPath = fragShaderAsset;

				materials[0].infos.texturePaths = { textureAssets[0] };
				materials[1].infos.texturePaths = { textureAssets[1] };

				materials[0].Save(materialAssets[0]);
				materials[1].Save(materialAssets[1]);

				magikarpBodyMat = materials[0].GetResource();
				magikarpEyesMat = materials[1].GetResource();
			}
		}
	}

	// === Gizmo ===
	{
		constexpr const char* vertShaderAsset = "Bin/Gizmo/Gizmo_VS.spha";
		constexpr const char* fragShaderAsset = "Bin/Gizmo/Gizmo_FS.spha";

		constexpr const char* meshAsset = "Bin/Gizmo/Square_M.spha";
		constexpr const char* materialAssets = "Bin/Gizmo/Square_Mat.spha";

		// Vertex Shader.
		if (!_assetMgr.shaderMgr.Load(vertShaderAsset, true)) // Try load.
		{
			// Import on load failed.
			_assetMgr.shaderMgr.Import("../../Engine/Resources/Shaders/gizmo.vert", { vertShaderAsset });
		}

		// Fragment Shader.
		if (!_assetMgr.shaderMgr.Load(fragShaderAsset, true)) // Try load.
		{
			// Import on load failed.
			_assetMgr.shaderMgr.Import("../../Engine/Resources/Shaders/gizmo.frag", { fragShaderAsset });
		}


		// Mesh.
		gizmoMesh = _assetMgr.meshMgr.Load(meshAsset);

		if (!gizmoMesh) // Try load.
		{
			MeshCreateInfos gizmoMeshInfos;

			gizmoMeshInfos.vertices =
			{
				{ { -0.5f, -0.5f, 0.0f }, Vec3f::Forward, { 1.0f, 0.0f } },
				{ { 0.5f, -0.5f, 0.0f }, Vec3f::Forward, { 0.0f, 0.0f } },
				{ { 0.5f, 0.5f, 0.0f }, Vec3f::Forward, { 0.0f, 1.0f } },
				{ { -0.5f, 0.5f, 0.0f }, Vec3f::Forward, { 1.0f, 1.0f } },
			};

			gizmoMeshInfos.indices =
			{
				0, 1, 2, 2, 3, 0
			};

			gizmoMeshInfos.outFilePaths = meshAsset;

			// Create on load failed.
			gizmoMesh = _assetMgr.meshMgr.Create(Move(gizmoMeshInfos)).GetResource();
		}

		// Materials.
		{
			gizmoMat = _assetMgr.renderMaterialMgr.Load(materialAssets);

			if (!gizmoMat) // Try load.
			{
				// Import on load failed.
				MaterialCreateInfos matInfos;
				matInfos.outFilePaths = materialAssets;

				matInfos.vertexShaderPath = vertShaderAsset;
				matInfos.fragmentShaderPath = fragShaderAsset;

				matInfos.uniformBufferSize = sizeof(Mat4f);
				matInfos.alphaModel = AlphaModel::Opaque;
				matInfos.cullingMode = CullingMode::None;
				matInfos.illumModel = IlluminationModel::None;

				gizmoMat = _assetMgr.renderMaterialMgr.Create(Move(matInfos)).GetResource();
			}
		}
	}
}

void DestroyResources(AssetManager& _assetMgr)
{
	// === Gizmo ===
	_assetMgr.renderMaterialMgr.Unload(gizmoMat);
	_assetMgr.meshMgr.Unload(gizmoMesh);


	// === Magikarp ===
	_assetMgr.renderMaterialMgr.Unload(magikarpEyesMat);
	_assetMgr.renderMaterialMgr.Unload(magikarpBodyMat);

	_assetMgr.meshMgr.Unload(magikarpEyesMesh);
	_assetMgr.meshMgr.Unload(magikarpBodyMesh);


	_assetMgr.Clear();
}

int main()
{
	LOG("=== Start ===");


	// === Init ===
	IRenderInstance::Init();
	IWindow::Init();


	// === Create ===
	VkRenderInstance instance;
	instance.Create();

	GLFWWindow window;
	window.Create(800u, 800u);

	VkRenderSurface& surface = const_cast<VkRenderSurface&>(static_cast<const VkRenderSurface&>(instance.CreateRenderSurface(window)));

	AssetManager assetMgr(instance);
	CreateResources(assetMgr);


	// Create Lights.
	PLightInfos pLight1;
	pLight1.position = API_ConvertCoordinateSystem(Vec3f(-1.0f, 2.0f, 3.0f));
	pLight1.color = Vec3f(0.9f, 0.7f, 0.3f);

	uint32 pLight1ID = instance.InstantiatePointLight(pLight1);

	PLightInfos pLight2;
	pLight2.position = API_ConvertCoordinateSystem(Vec3f(-2.0f, 2.0f, -2.0f));
	pLight2.color = Vec3f(0.9f, 0.7f, 0.3f);
	pLight2.intensity = 5.0f;

	uint32 pLight2ID = instance.InstantiatePointLight(pLight2);

	{
		ObjectUniformBuffer oubo;
		oubo.modelMat = API_ConvertCoordinateSystem(TransffPRS(Vec3f(-1.0f, -1.0, -2.5f),
			Quatf(180, Vec3f::Up) * Quatf(-90, Vec3f::Right), Vec3f(0.000001f)).Matrix());
		magikarpBodyMat->InitVariable(instance, &oubo, sizeof(oubo));
		magikarpEyesMat->InitVariable(instance, &oubo, sizeof(oubo));
	}

	{
		Mat4f modelMat = API_ConvertCoordinateSystem(TransffPRS(pLight1.position, Quatf::Identity, Vec3f::One * 0.5f).Matrix());
		gizmoMat->InitVariable(instance, &modelMat, sizeof(Mat4f));
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


	TransffPRS camTr;
	camTr.position = Vec3f(-2.0f, 1.0f, 5.0f);
	Vec3f pL1Pos = API_ConvertCoordinateSystem(pLight1.position);

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


		window.TEST(camTr, pL1Pos, speed * deltaTime);

		// Update Uniform Buffer.
		StaticUniformBuffer ubo;
		//ubo.modelMat = Mat4f::MakeRotation(Quatf(time, Vec3f::Right));
		ubo.viewInvMat = API_ConvertCoordinateSystem(camTr.Matrix()).Inverse();
		ubo.projMat = perspMat;

		frame.uniformBuffer.UpdateData(instance.GetDevice(), &ubo, sizeof(ubo));
		
		{
			ObjectUniformBuffer oubo;
			oubo.modelMat = API_ConvertCoordinateSystem(TransffPRS(pL1Pos, Quatf::Identity, Vec3f::One * 0.5f).Matrix());
			gizmoMat->UpdateVariable(instance, frame, &oubo, sizeof(oubo));

			vkDeviceWaitIdle(instance.GetDevice());
			pLight1.position = API_ConvertCoordinateSystem(pL1Pos);
			instance.mPointLightBuffer.UpdateObject(instance.GetDevice(), pLight1, pLight1ID);
		}


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

		// Draw Magikarp.
		magikarpBodyMat->Bind(frame);
		magikarpBodyMesh->Draw(frame);

		magikarpEyesMat->Bind(frame);
		magikarpEyesMesh->Draw(frame);


		// Draw gizmos.
		gizmoMat->Bind(frame);
		gizmoMesh->Draw(frame);


		vkCmdEndRenderPass(frame.graphicsCommandBuffer);

		SA_VK_ASSERT(vkEndCommandBuffer(frame.graphicsCommandBuffer),
			LibCommandFailed, Rendering, L"Failed to end command buffer!");
	}



	// === Destroy ===
	vkDeviceWaitIdle(instance.GetDevice());

	DestroyResources(assetMgr);

	instance.DestroyRenderSurface(window);
	
	window.Destroy();

	instance.Destroy();


	// === UnInit ===
	IWindow::UnInit();
	IRenderInstance::UnInit();


	LOG("\n=== End ===");

	return 0;
}
