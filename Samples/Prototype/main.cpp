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

#include <Sapphire/SDK/Resources/Assets/MeshAsset.hpp>
#include <Sapphire/SDK/Resources/Assets/ShaderAsset.hpp>
#include <Sapphire/SDK/Resources/Assets/TextureAsset.hpp>
#include <Sapphire/SDK/Resources/Assets/MaterialAsset.hpp>

using namespace Sa;

#define LOG(_str) std::cout << _str << std::endl;


// Magikarp
IShader* magikarpVertShader = nullptr;
IShader* magikarpFragShader = nullptr;

ITexture* magikarpBodyTexture = nullptr;
ITexture* magikarpEyesTexture = nullptr;

IMesh* magikarpBodyMesh = nullptr;
IMesh* magikarpEyesMesh = nullptr;

IRenderMaterial* magikarpBodyMat = nullptr;
IRenderMaterial* magikarpEyesMat = nullptr;

// Gizmo
IShader* gizmoVertShader = nullptr;
IShader* gizmoFragShader = nullptr;

IMesh* gizmoMesh = nullptr;
IRenderMaterial* gizmoMat = nullptr;

void CreateResources(const IRenderInstance& _instance, const IRenderSurface& _surface)
{
	// === Magikarp ===
	{
		constexpr const char* vertShaderAsset = "Bin/Magikarp/Magikarp_VS.spha";
		constexpr const char* fragShaderAsset = "Bin/Magikarp/Magikarp_FS.spha";

		constexpr const char* textureAssets[] = { "Bin/Magikarp/Body_T.spha", "Bin/Magikarp/Eyes_T.spha" };
		constexpr const char* meshAssets[] = { "Bin/Magikarp/Body_M.spha", "Bin/Magikarp/Eyes_M.spha" };
		constexpr const char* materialAssets[] = { "Bin/Magikarp/Body_Mat.spha", "Bin/Magikarp/Eyes_Mat.spha" };

		// Vertex Shader.
		{
			ShaderAsset vertexShaderAsset;

			// Try load.
			if (!vertexShaderAsset.Load(vertShaderAsset))
			{
				// Import on load failed.
				vertexShaderAsset = ShaderAsset::Import("../../Bin/Shaders/default_vert.spv");
				vertexShaderAsset.Save(vertShaderAsset);
			}

			magikarpVertShader = vertexShaderAsset.Create(_instance);
		}

		// Fragment Shader.
		{
			ShaderAsset fragmentShaderAsset;

			// Try load.
			if (!fragmentShaderAsset.Load(fragShaderAsset))
			{
				// Import on load failed.
				fragmentShaderAsset = ShaderAsset::Import("../../Bin/Shaders/default_frag.spv");
				fragmentShaderAsset.Save(fragShaderAsset);
			}

			magikarpFragShader = fragmentShaderAsset.Create(_instance);
		}

		// Body Texture.
		{
			TextureAsset magikarpBodyTextureAsset;

			// Try load.
			if (!magikarpBodyTextureAsset.Load(textureAssets[0]))
			{
				// Import on load failed.
				magikarpBodyTextureAsset = TextureAsset::Import("../../Engine/Resources/Models/Magikarp/Body.png");
				magikarpBodyTextureAsset.Save(textureAssets[0]);
			}

			magikarpBodyTexture = magikarpBodyTextureAsset.Create(_instance);
		}


		// Eyes Texture.
		{
			TextureAsset magikarpEyesAssetTexture;

			// Try load.
			if (!magikarpEyesAssetTexture.Load(textureAssets[1]))
			{
				// Import on load failed.
				magikarpEyesAssetTexture = TextureAsset::Import("../../Engine/Resources/Models/Magikarp/Eyes.png");
				magikarpEyesAssetTexture.Save(textureAssets[1]);
			}

			magikarpEyesTexture = magikarpEyesAssetTexture.Create(_instance);
		}


		// Meshes.
		{
		std::vector<MeshAsset> magikarpMeshAssets(2);

		// Try load.
		if (!magikarpMeshAssets[0].Load(meshAssets[0]) || !magikarpMeshAssets[1].Load(meshAssets[1]))
		{
			// Import on load failed.
			magikarpMeshAssets = MeshAsset::Import("../../Engine/Resources/Models/Magikarp/Magikarp.obj");

			magikarpMeshAssets[0].Save(meshAssets[0]);
			magikarpMeshAssets[1].Save(meshAssets[1]);
		}

		magikarpBodyMesh = magikarpMeshAssets[0].Create(_instance);
		magikarpEyesMesh = magikarpMeshAssets[1].Create(_instance);
		}


		// Materials.
		{
			std::vector<MaterialAsset> magikarpMaterialAssets(2);

			// Try load.
			if (!magikarpMaterialAssets[0].Load(materialAssets[0]) || !magikarpMaterialAssets[1].Load(materialAssets[1]))
			{
				// Import on load failed.
				magikarpMaterialAssets = MaterialAsset::Import("../../Engine/Resources/Models/Magikarp/Magikarp.mtl");

				magikarpMaterialAssets[0].vertexShaderPath = vertShaderAsset;
				magikarpMaterialAssets[1].vertexShaderPath = vertShaderAsset;

				magikarpMaterialAssets[0].fragmentShaderPath = fragShaderAsset;
				magikarpMaterialAssets[1].fragmentShaderPath = fragShaderAsset;

				magikarpMaterialAssets[0].texturePaths = { textureAssets[0] };
				magikarpMaterialAssets[1].texturePaths = { textureAssets[1] };

				magikarpMaterialAssets[0].Save(materialAssets[0]);
				magikarpMaterialAssets[1].Save(materialAssets[1]);
			}

			magikarpBodyMat = magikarpMaterialAssets[0].Create(_instance, _surface);
			magikarpEyesMat = magikarpMaterialAssets[1].Create(_instance, _surface);

			// TODO: REMOVE LATER.
			{
				PipelineCreateInfos bodyPipelineInfos
				{
					_surface,
					_surface.GetViewport(),

					magikarpVertShader,
					magikarpFragShader,

					MaterialConstants(),

					{ magikarpBodyTexture },

					sizeof(ObjectUniformBuffer),

					AlphaModel::Opaque,
					PolygonMode::Fill,
					CullingMode::Back,
					FrontFaceMode::Clockwise,
					IlluminationModel::PBR
				};

				magikarpBodyMat = IRenderMaterial::CreateInstance();
				magikarpBodyMat->CreatePipeline(_instance, bodyPipelineInfos);


				PipelineCreateInfos eyesPipelineInfos
				{
					_surface,
					_surface.GetViewport(),

					magikarpVertShader,
					magikarpFragShader,

					MaterialConstants(),

					{ magikarpEyesTexture },

					sizeof(ObjectUniformBuffer),

					AlphaModel::Opaque,
					PolygonMode::Fill,
					CullingMode::Back,
					FrontFaceMode::Clockwise,
					IlluminationModel::PBR
				};

				magikarpEyesMat = IRenderMaterial::CreateInstance();
				magikarpEyesMat->CreatePipeline(_instance, eyesPipelineInfos);
			}
		}
	}

	// === Gizmo ===
	{
		constexpr const char* vertShaderAsset = "Bin/Gizmo/Gizmo_VS.spha";
		constexpr const char* fragShaderAsset = "Bin/Gizmo/Gizmo_FS.spha";

		constexpr const char* meshAsset = "Bin/Gizmos/Square_M.spha";
		constexpr const char* materialAssets = "Bin/Gizmos/Square_Mat.spha";

		// Vertex Shader.
		{
			ShaderAsset vertexShaderAsset;

			// Try load.
			if (!vertexShaderAsset.Load(vertShaderAsset))
			{
				// Import on load failed.
				vertexShaderAsset = ShaderAsset::Import("../../Bin/Shaders/gizmo_vert.spv");
				vertexShaderAsset.Save(vertShaderAsset);
			}

			gizmoVertShader = vertexShaderAsset.Create(_instance);
		}

		// Fragment Shader.
		{
			ShaderAsset fragmentShaderAsset;

			// Try load.
			if (!fragmentShaderAsset.Load(fragShaderAsset))
			{
				// Import on load failed.
				fragmentShaderAsset = ShaderAsset::Import("../../Bin/Shaders/gizmo_frag.spv");
				fragmentShaderAsset.Save(fragShaderAsset);
			}

			gizmoFragShader = fragmentShaderAsset.Create(_instance);
		}

		// Mesh.
		{
			MeshAsset gizmoMeshAsset;

			// Try load.
			if (!gizmoMeshAsset.Load(meshAsset))
			{
				// Import on load failed.
				const std::vector<Vertex> vertices =
				{
					{ { -0.5f, -0.5f, 0.0f }, Vec3f::Forward, { 1.0f, 0.0f } },
					{ { 0.5f, -0.5f, 0.0f }, Vec3f::Forward, { 0.0f, 0.0f } },
					{ { 0.5f, 0.5f, 0.0f }, Vec3f::Forward, { 0.0f, 1.0f } },
					{ { -0.5f, 0.5f, 0.0f }, Vec3f::Forward, { 1.0f, 1.0f } },
				};

				const std::vector<uint32> indices =
				{
					0, 1, 2, 2, 3, 0
				};

				gizmoMeshAsset = MeshAsset(vertices, indices);
				gizmoMeshAsset.Save(meshAsset);
			}

			gizmoMesh = gizmoMeshAsset.Create(_instance);
		}

		// Materials.
		{
			MaterialAsset gizmoMaterialAsset;

			// Try load.
			if (!gizmoMaterialAsset.Load(materialAssets))
			{
				// Import on load failed.
				gizmoMaterialAsset.vertexShaderPath = vertShaderAsset;
				gizmoMaterialAsset.fragmentShaderPath = fragShaderAsset;

				gizmoMaterialAsset.Save(materialAssets);
			}

			gizmoMat = gizmoMaterialAsset.Create(_instance, _surface);

			// TODO: REMOVE LATER.
			{
				PipelineCreateInfos gizmoPipelineInfos
				{
					_surface,
					_surface.GetViewport(),

					gizmoVertShader,
					gizmoFragShader,

					MaterialConstants(),

					{ },

					sizeof(Mat4f),

					AlphaModel::Opaque,
					PolygonMode::Fill,
					CullingMode::None,
					FrontFaceMode::Clockwise,
					IlluminationModel::None
				};

				gizmoMat = IRenderMaterial::CreateInstance();
				gizmoMat->CreatePipeline(_instance, gizmoPipelineInfos);
			}
		}
	}
}

void DestroyResources(const IRenderInstance& _instance)
{
	// === Gizmo ===
	gizmoMat->DestroyPipeline(_instance);
	gizmoMesh->Destroy(_instance);

	gizmoFragShader->Destroy(_instance);
	gizmoVertShader->Destroy(_instance);


	// === Magikarp ===
	magikarpBodyMat->DestroyPipeline(_instance);
	magikarpEyesMat->DestroyPipeline(_instance);

	magikarpEyesMesh->Destroy(_instance);
	magikarpBodyMesh->Destroy(_instance);

	magikarpEyesTexture->Destroy(_instance);
	magikarpBodyTexture->Destroy(_instance);

	magikarpFragShader->Destroy(_instance);
	magikarpVertShader->Destroy(_instance);
}

int main()
{
	LOG("=== Start ===");


	// === Create ===
	VkRenderInstance instance;
	instance.Create();

	GLFWWindow window;
	window.Create(800u, 800u);

	VkRenderSurface& surface = const_cast<VkRenderSurface&>(static_cast<const VkRenderSurface&>(instance.CreateRenderSurface(window)));


	CreateResources(instance, surface);


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

	DestroyResources(instance);

	instance.DestroyRenderSurface(window);
	
	window.Destroy();

	instance.Destroy();


	LOG("\n=== End ===");

	return 0;
}
