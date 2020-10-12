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

#include <Sapphire/SDK/Assets/AssetManager.hpp>

using namespace Sa;

#define LOG(_str) std::cout << _str << std::endl;

// Default
constexpr const char* defaultVertShaderAsset = "Bin/Shaders/default_VS.spha";
constexpr const char* defaultFragShaderAsset = "Bin/Shaders/default_FS.spha";

// Magikarp
IMesh* magikarpBodyMesh = nullptr;
IMesh* magikarpEyesMesh = nullptr;

IRenderMaterial* magikarpBodyMat = nullptr;
IRenderMaterial* magikarpEyesMat = nullptr;

// Gizmo
IMesh* gizmoMesh = nullptr;
IRenderMaterial* gizmoMat = nullptr;

// Drone
IMesh* droneMesh = nullptr;
IRenderMaterial* droneMat = nullptr;


void CreateDefaultResources(AssetManager& _assetMgr)
{
	// Vertex Shader.
	if (!_assetMgr.shaderMgr.Load(defaultVertShaderAsset, true)) // Try load.
	{
		// Import on load failed.
		IAssetImportResult result = _assetMgr.Import("../../Engine/Resources/Shaders/default.vert");
		result[0]->Save(defaultVertShaderAsset);
	}


	// Fragment Shader.
	if (!_assetMgr.shaderMgr.Load(defaultFragShaderAsset, true)) // Try load.
	{
		// Import on load failed.
		IAssetImportResult result = _assetMgr.Import("../../Engine/Resources/Shaders/default.frag");
		result[0]->Save(defaultFragShaderAsset);
	}
}

void CreateMagikarp(AssetManager& _assetMgr)
{
	constexpr const char* textureAssets[] = { "Bin/Magikarp/Body_T.spha", "Bin/Magikarp/Eyes_T.spha" };
	constexpr const char* meshAssets[] = { "Bin/Magikarp/Body_M.spha", "Bin/Magikarp/Eyes_M.spha" };
	constexpr const char* materialAssets[] = { "Bin/Magikarp/Body_Mat.spha", "Bin/Magikarp/Eyes_Mat.spha" };


	// Body Texture.
	if (!_assetMgr.textureMgr.Load(textureAssets[0], true)) // Try load.
	{
		// Import on load failed.
		IAssetImportResult result = _assetMgr.Import("../../Samples/Prototype/Resources/Magikarp/Body.png");
		result[0]->Save(textureAssets[0]);
	}


	// Eyes Texture.
	if (!_assetMgr.textureMgr.Load(textureAssets[1], true)) // Try load.
	{
		// Import on load failed.
		IAssetImportResult result = _assetMgr.Import("../../Samples/Prototype/Resources/Magikarp/Eyes.png");
		result[0]->Save(textureAssets[1]);
	}


	// Model.
	{
		// Try load.
		magikarpBodyMesh = _assetMgr.meshMgr.Load(meshAssets[0]);
		magikarpEyesMesh = _assetMgr.meshMgr.Load(meshAssets[1]);

		magikarpBodyMat = _assetMgr.renderMatMgr.Load(materialAssets[0]);
		magikarpEyesMat = _assetMgr.renderMatMgr.Load(materialAssets[1]);

		if (!magikarpBodyMesh || !magikarpEyesMesh || !magikarpBodyMat || !magikarpEyesMat)
		{
			// Import on load failed.
			IAssetImportResult result = _assetMgr.Import("../../Samples/Prototype/Resources/Magikarp/Magikarp.obj");
			
			// meshes
			result[0]->Save(meshAssets[0]);
			magikarpBodyMesh = result[0]->As<MeshAsset>().GetResource();

			result[1]->Save(meshAssets[1]);
			magikarpEyesMesh = result[1]->As<MeshAsset>().GetResource();


			RenderMaterialAsset& bodyRenderMat = result[2]->As<RenderMaterialAsset>();
			bodyRenderMat.infos.vertexShaderPath = defaultVertShaderAsset;
			bodyRenderMat.infos.fragmentShaderPath = defaultFragShaderAsset;
			bodyRenderMat.infos.texturePaths = { textureAssets[0] };
			bodyRenderMat.Save(materialAssets[0]);
			magikarpBodyMat = bodyRenderMat.GetResource();


			RenderMaterialAsset& eyesRenderMat = result[3]->As<RenderMaterialAsset>();
			eyesRenderMat.infos.vertexShaderPath = defaultVertShaderAsset;
			eyesRenderMat.infos.fragmentShaderPath = defaultFragShaderAsset;
			eyesRenderMat.infos.texturePaths = { textureAssets[1] };
			eyesRenderMat.Save(materialAssets[1]);
			magikarpEyesMat = eyesRenderMat.GetResource();
		}
	}
}

void CreateGizmo(AssetManager& _assetMgr)
{
	constexpr const char* vertShaderAsset = "Bin/Gizmo/Gizmo_VS.spha";
	constexpr const char* fragShaderAsset = "Bin/Gizmo/Gizmo_FS.spha";

	constexpr const char* meshAsset = "Bin/Gizmo/Square_M.spha";
	constexpr const char* materialAssets = "Bin/Gizmo/Square_Mat.spha";

	// Vertex Shader.
	if (!_assetMgr.shaderMgr.Load(vertShaderAsset, true)) // Try load.
	{
		// Import on load failed.
		IAssetImportResult result = _assetMgr.Import("../../Engine/Resources/Shaders/gizmo.vert");
		result[0]->Save(vertShaderAsset);
	}

	// Fragment Shader.
	if (!_assetMgr.shaderMgr.Load(fragShaderAsset, true)) // Try load.
	{
		// Import on load failed.
		IAssetImportResult result = _assetMgr.Import("../../Engine/Resources/Shaders/gizmo.frag");
		result[0]->Save(fragShaderAsset);
	}


	// Mesh.
	gizmoMesh = _assetMgr.meshMgr.Load(meshAsset);

	if (!gizmoMesh) // Try load.
	{
		MeshRawData gizmoRawMesh;

		gizmoRawMesh.vertices =
		{
			{ { -0.5f, -0.5f, 0.0f }, Vec3f::Forward, { 1.0f, 0.0f } },
			{ { 0.5f, -0.5f, 0.0f }, Vec3f::Forward, { 0.0f, 0.0f } },
			{ { 0.5f, 0.5f, 0.0f }, Vec3f::Forward, { 0.0f, 1.0f } },
			{ { -0.5f, 0.5f, 0.0f }, Vec3f::Forward, { 1.0f, 1.0f } },
		};

		gizmoRawMesh.indices =
		{
			0, 1, 2, 2, 3, 0
		};

		// Create on load failed.
		MeshAsset gizmoMeshAsset = _assetMgr.meshMgr.Create(Move(gizmoRawMesh));
		gizmoMeshAsset.Save(meshAsset);
		gizmoMesh = gizmoMeshAsset.GetResource();
	}

	// Materials.
	gizmoMat = _assetMgr.renderMatMgr.Load(materialAssets);
	
	if (!gizmoMat) // Try load.
	{
		// Import on load failed.
		RenderMaterialRawData gizmoRawMat;
		gizmoRawMat.vertexShaderPath = vertShaderAsset;
		gizmoRawMat.fragmentShaderPath = fragShaderAsset;

		gizmoRawMat.renderInfos = PipelineRenderInfos
		{
			sizeof(Mat4f),
			AlphaModel::Opaque,
			PolygonMode::Fill,
			CullingMode::None,
			FrontFaceMode::Clockwise,
			IlluminationModel::None
		};

		RenderMaterialAsset gizmoMatAsset = _assetMgr.renderMatMgr.Create(Move(gizmoRawMat));
		gizmoMatAsset.Save(materialAssets);
		gizmoMat = gizmoMatAsset.GetResource();
	}
}

void CreateDrone(AssetManager& _assetMgr)
{
	//constexpr const char* textureAssets[] = { "Bin/Drone/BodyBaseColor_T.spha", "Bin/Drone/MaterialBaseColor_T.spha" };
	//constexpr const char* meshAssets[] = { "Bin/Drone/Drone_M.spha" };
	//constexpr const char* materialAssets[] = { "Bin/Drone/Drone_Mat.spha" };


	//constexpr const char* textureResources[] = { "../../Samples/Prototype/Resources/BusterDrone/GLTF/textures/body_baseColor.png",
	//												"../../Samples/Prototype/Resources/BusterDrone/GLTF/textures/material_baseColor.png" };

	//for (uint32 i = 0u; i < SizeOf(textureAssets); ++i)
	//{
	//	if (!_assetMgr.textureMgr.Create(textureAssets[i]).IsValid()) // Try load.
	//	{
	//		// Import on load failed.
	//		TextureImportInfos importInfos;
	//		importInfos.outFilePath = textureAssets[i];

	//		_assetMgr.textureMgr.Import(textureResources[i], importInfos);
	//	}
	//}


	//// Meshes.
	//{
	//	{
	//		// Try load.

	//		MeshAsset droneMeshAsset = _assetMgr.meshMgr.Create(meshAssets[0]);

	//		if (droneMeshAsset.IsValid())
	//			droneMesh = droneMeshAsset.GetResource();
	//	}

	//	if (!droneMesh)
	//	{
	//		// Import on load failed.
	//		MeshImportInfos importInfos;
	//		importInfos.outFilePaths = { meshAssets[0] };

	//		auto meshes = _assetMgr.meshMgr.Import("../../Samples/Prototype/Resources/BusterDrone/GLTF/BusterDrone.gltf", importInfos);

	//		droneMesh = meshes[0].GetResource();
	//	}
	//}

	//// Materials.
	//{
	//	{
	//		// Try load.

	//		MaterialAsset droneMatAsset = _assetMgr.renderMaterialMgr.Create(materialAssets[0], false);

	//		if (droneMatAsset.IsValid())
	//		{
	//			droneMatAsset.infos.renderPasses = { &_mainRenderPass };
	//			droneMatAsset.infos.cameras = { &_camera };

	//			droneMat = _assetMgr.renderMaterialMgr.Load(droneMatAsset);
	//		}
	//	}

	//	//if (!droneMat)
	//	//{
	//	//	// Import on load failed.
	//	//	MaterialImportInfos importInfos;
	//	//	importInfos.outFilePaths = { meshAssets[0] , meshAssets[1] };

	//	//	auto materials = _assetMgr.renderMaterialMgr.Import("../../Engine/Resources/Models/Magikarp/Magikarp.mtl", importInfos);

	//	//	materials[0].infos.vertexShaderPath = defaultVertShaderAsset;
	//	//	materials[1].infos.vertexShaderPath = defaultVertShaderAsset;

	//	//	materials[0].infos.fragmentShaderPath = defaultFragShaderAsset;
	//	//	materials[1].infos.fragmentShaderPath = defaultFragShaderAsset;

	//	//	materials[0].infos.texturePaths = { textureAssets[0] };
	//	//	materials[1].infos.texturePaths = { textureAssets[1] };

	//	//	materials[0].infos.renderPasses = { &_mainRenderPass };
	//	//	materials[1].infos.renderPasses = { &_mainRenderPass };

	//	//	materials[0].infos.cameras = { &_camera };
	//	//	materials[1].infos.cameras = { &_camera };

	//	//	materials[0].infos.bDynamicViewport = false; // TODO: FIX LATER.
	//	//	materials[1].infos.bDynamicViewport = false; // TODO: FIX LATER.

	//	//	materials[0].Save(materialAssets[0]);
	//	//	materials[1].Save(materialAssets[1]);

	//	//	magikarpBodyMat = materials[0].GetResource();
	//	//	magikarpEyesMat = materials[1].GetResource();
	//	//}
	//}
}

void CreateResources(AssetManager& _assetMgr)
{
	CreateDefaultResources(_assetMgr);
	CreateMagikarp(_assetMgr);
	CreateGizmo(_assetMgr);
	//CreateDrone(_assetMgr);
}

void DestroyResources(AssetManager& _assetMgr)
{
	// === Gizmo ===
	_assetMgr.renderMatMgr.Unload(gizmoMat);
	_assetMgr.meshMgr.Unload(gizmoMesh);


	// === Magikarp ===
	_assetMgr.renderMatMgr.Unload(magikarpEyesMat);
	_assetMgr.renderMatMgr.Unload(magikarpBodyMat);

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
	IRenderPass::mainRenderPass = &mainRenderPass;

	Camera mainCamera(surface.GetImageExtent());
	Camera::mainCamera = &mainCamera;

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
			gizmoMat->UpdateVariable(instance, frame, &oubo, sizeof(oubo));

			vkDeviceWaitIdle(instance.GetDevice());
			pLight1.position = API_ConvertCoordinateSystem(pL1Pos);
			instance.mPointLightBuffer.UpdateObject(instance.GetDevice(), pLight1, pLight1ID);
		}


		mainRenderPass.Begin(frame);


		// Draw Magikarp.
		magikarpBodyMat->Bind(frame);
		magikarpBodyMesh->Draw(frame);

		magikarpEyesMat->Bind(frame);
		magikarpEyesMesh->Draw(frame);


		// Draw gizmos.
		gizmoMat->Bind(frame);
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
