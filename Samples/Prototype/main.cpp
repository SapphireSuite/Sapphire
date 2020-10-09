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
#include <Sapphire/Rendering/Framework/Primitives/Camera/Camera.hpp>

#include <Sapphire/Rendering/Vulkan/System/VkRenderInstance.hpp>
#include <Sapphire/Rendering/Vulkan/System/VkRenderPass.hpp>

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

void CreateResources(AssetManager& _assetMgr, const Sa::IRenderPass& _mainRenderPass, const Camera& _camera)
{
	// === Magikarp ===
	{
		constexpr const char* vertShaderAsset = "Bin/Magikarp/Magikarp_VS.spha";
		constexpr const char* fragShaderAsset = "Bin/Magikarp/Magikarp_FS.spha";

		constexpr const char* textureAssets[] = { "Bin/Magikarp/Body_T.spha", "Bin/Magikarp/Eyes_T.spha" };
		constexpr const char* meshAssets[] = { "Bin/Magikarp/Body_M.spha", "Bin/Magikarp/Eyes_M.spha" };
		constexpr const char* materialAssets[] = { "Bin/Magikarp/Body_Mat.spha", "Bin/Magikarp/Eyes_Mat.spha" };

		// Vertex Shader.
		if (!_assetMgr.shaderMgr.Create(vertShaderAsset).IsValid()) // Try load.
		{
			// Import on load failed.
			ShaderImportInfos importInfos;
			importInfos.outFilePath = vertShaderAsset;

			_assetMgr.shaderMgr.Import("../../Engine/Resources/Shaders/default.vert", importInfos);
		}

		// Fragment Shader.
		if (!_assetMgr.shaderMgr.Create(fragShaderAsset).IsValid()) // Try load.
		{
			// Import on load failed.
			ShaderImportInfos importInfos;
			importInfos.outFilePath = fragShaderAsset;

			_assetMgr.shaderMgr.Import("../../Engine/Resources/Shaders/default.frag", importInfos);
		}


		// Body Texture.
		if (!_assetMgr.textureMgr.Create(textureAssets[0]).IsValid()) // Try load.
		{
			// Import on load failed.
			TextureImportInfos importInfos;
			importInfos.outFilePath = textureAssets[0];

			_assetMgr.textureMgr.Import("../../Engine/Resources/Models/Magikarp/Body.png", importInfos);
		}


		// Eyes Texture.
		if (!_assetMgr.textureMgr.Create(textureAssets[1]).IsValid()) // Try load.
		{
			// Import on load failed.
			TextureImportInfos importInfos;
			importInfos.outFilePath = textureAssets[1];

			_assetMgr.textureMgr.Import("../../Engine/Resources/Models/Magikarp/Eyes.png", importInfos);
		}


		// Meshes.
		{
			{
				// Try load.

				MeshAsset bodyMeshAsset = _assetMgr.meshMgr.Create(meshAssets[0]);

				if (bodyMeshAsset.IsValid())
					magikarpBodyMesh = bodyMeshAsset.GetResource();

				MeshAsset eyesMeshAsset = _assetMgr.meshMgr.Create(meshAssets[1]);

				if (eyesMeshAsset.IsValid())
					magikarpEyesMesh = eyesMeshAsset.GetResource();
			}

			if (!magikarpBodyMesh || !magikarpEyesMesh)
			{
				// Import on load failed.
				MeshImportInfos importInfos;
				importInfos.outFilePaths = { meshAssets[0] , meshAssets[1] };

				auto meshes = _assetMgr.meshMgr.Import("../../Engine/Resources/Models/Magikarp/Magikarp.obj", importInfos);

				magikarpBodyMesh = meshes[0].GetResource();
				magikarpEyesMesh = meshes[1].GetResource();
			}
		}

		// Materials.
		{
			{
				// Try load.

				MaterialAsset bodyMatAsset = _assetMgr.renderMaterialMgr.Create(materialAssets[0], false);
				
				if (bodyMatAsset.IsValid())
				{
					bodyMatAsset.infos.renderPasses = { &_mainRenderPass };
					bodyMatAsset.infos.cameras = { &_camera };

					magikarpBodyMat = _assetMgr.renderMaterialMgr.Load(bodyMatAsset);
				}


				MaterialAsset eyesMatAsset = _assetMgr.renderMaterialMgr.Create(materialAssets[1], false);
				
				if (eyesMatAsset.IsValid())
				{
					eyesMatAsset.infos.renderPasses = { &_mainRenderPass };
					eyesMatAsset.infos.cameras = { &_camera };
					magikarpEyesMat = _assetMgr.renderMaterialMgr.Load(eyesMatAsset);
				}
			}

			if (!magikarpBodyMat || !magikarpEyesMat)
			{
				// Import on load failed.
				MaterialImportInfos importInfos;
				importInfos.outFilePaths = { meshAssets[0] , meshAssets[1] };

				auto materials = _assetMgr.renderMaterialMgr.Import("../../Engine/Resources/Models/Magikarp/Magikarp.mtl", importInfos);

				materials[0].infos.vertexShaderPath = vertShaderAsset;
				materials[1].infos.vertexShaderPath = vertShaderAsset;

				materials[0].infos.fragmentShaderPath = fragShaderAsset;
				materials[1].infos.fragmentShaderPath = fragShaderAsset;

				materials[0].infos.texturePaths = { textureAssets[0] };
				materials[1].infos.texturePaths = { textureAssets[1] };

				materials[0].infos.renderPasses = { &_mainRenderPass };
				materials[1].infos.renderPasses = { &_mainRenderPass };
				
				materials[0].infos.cameras = { &_camera };
				materials[1].infos.cameras = { &_camera };
				
				materials[0].infos.bDynamicViewport = false; // TODO: FIX LATER.
				materials[1].infos.bDynamicViewport = false; // TODO: FIX LATER.

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
		if (!_assetMgr.shaderMgr.Create(vertShaderAsset).IsValid()) // Try load.
		{
			// Import on load failed.
			ShaderImportInfos importInfos;
			importInfos.outFilePath = vertShaderAsset;

			_assetMgr.shaderMgr.Import("../../Engine/Resources/Shaders/gizmo.vert", importInfos);
		}

		// Fragment Shader.
		if (!_assetMgr.shaderMgr.Create(fragShaderAsset).IsValid()) // Try load.
		{
			// Import on load failed.
			ShaderImportInfos importInfos;
			importInfos.outFilePath = fragShaderAsset;

			_assetMgr.shaderMgr.Import("../../Engine/Resources/Shaders/gizmo.frag", importInfos);
		}


		// Mesh.
		{
			MeshAsset gizmoMeshAsset = _assetMgr.meshMgr.Create(meshAsset);

			if(gizmoMeshAsset.IsValid())
				gizmoMesh = gizmoMeshAsset.GetResource();
		}

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
			{
				// Try load.
				MaterialAsset gizmoMatAsset = _assetMgr.renderMaterialMgr.Create(materialAssets, false);

				if (gizmoMatAsset.IsValid())
				{
					gizmoMatAsset.infos.renderPasses = { &_mainRenderPass };
					gizmoMatAsset.infos.cameras = { &_camera };

					gizmoMat = _assetMgr.renderMaterialMgr.Load(gizmoMatAsset);
				}
			}

			if (!gizmoMat)
			{
				// Import on load failed.
				MaterialCreateInfos matInfos;
				matInfos.renderPasses = { &_mainRenderPass };
				matInfos.cameras = { &_camera };
				matInfos.bDynamicViewport = false; // TODO: FIX LATER.
				matInfos.outFilePaths = materialAssets;

				matInfos.vertexShaderPath = vertShaderAsset;
				matInfos.fragmentShaderPath = fragShaderAsset;

				matInfos.renderInfos = PipelineRenderInfos
				{
					sizeof(Mat4f),
					AlphaModel::Opaque,
					PolygonMode::Fill,
					CullingMode::None,
					FrontFaceMode::Clockwise,
					IlluminationModel::None
				};

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

	Sa::VkRenderPass& mainRenderPass = surface.CreateRenderPass(instance, RenderPassCreateInfos{}).As<Sa::VkRenderPass>();

	Camera mainCamera(surface.GetImageExtent());

	AssetManager assetMgr(instance);
	CreateResources(assetMgr, mainRenderPass, mainCamera);


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
		magikarpBodyMat->InitVariable(instance, 0u, &oubo, sizeof(oubo));
		magikarpEyesMat->InitVariable(instance, 0u, &oubo, sizeof(oubo));
	}

	{
		Mat4f modelMat = API_ConvertCoordinateSystem(TransffPRS(pLight1.position, Quatf::Identity, Vec3f::One * 0.5f).Matrix());
		gizmoMat->InitVariable(instance, 0u, &modelMat, sizeof(Mat4f));
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

		VkRenderFrame frame = surface.GetSwapChain().Begin(instance.GetDevice());


		window.TEST(camTr, pL1Pos, speed * deltaTime);

		// Update Uniform Buffer.
		StaticUniformBuffer ubo;
		//ubo.modelMat = Mat4f::MakeRotation(Quatf(time, Vec3f::Right));
		ubo.viewInvMat = API_ConvertCoordinateSystem(camTr.Matrix()).Inverse();
		ubo.projMat = perspMat;

		mainRenderPass.GetUniformBuffer(frame.index).UpdateData(instance.GetDevice(), &ubo, sizeof(ubo));
		
		{
			ObjectUniformBuffer oubo;
			oubo.modelMat = API_ConvertCoordinateSystem(TransffPRS(pL1Pos, Quatf::Identity, Vec3f::One * 0.5f).Matrix());
			gizmoMat->UpdateVariable(instance, frame, 0u, &oubo, sizeof(oubo));

			vkDeviceWaitIdle(instance.GetDevice());
			pLight1.position = API_ConvertCoordinateSystem(pL1Pos);
			instance.mPointLightBuffer.UpdateObject(instance.GetDevice(), pLight1, pLight1ID);
		}


		mainRenderPass.Begin(frame);


		// Draw Magikarp.
		magikarpBodyMat->Bind(frame, 0u);
		magikarpBodyMesh->Draw(frame);

		magikarpEyesMat->Bind(frame, 0u);
		magikarpEyesMesh->Draw(frame);


		// Draw gizmos.
		gizmoMat->Bind(frame, 0u);
		gizmoMesh->Draw(frame);


		mainRenderPass.End(frame);


		surface.GetSwapChain().End(instance.GetDevice());
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
