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
#include <Sapphire/Rendering/Framework/Primitives/Camera/CameraUniformBuffer.hpp>

#include <Sapphire/Rendering/Vulkan/System/VkRenderInstance.hpp>
#include <Sapphire/Rendering/Vulkan/System/VkRenderPass.hpp>
#include <Sapphire/Rendering/Vulkan/Primitives/Camera/VkCamera.hpp>

#include <Sapphire/SDK/Assets/AssetManager.hpp>

using namespace Sa;

#define LOG(_str) std::cout << _str << std::endl;

// Default
constexpr const char* litVertShaderAsset = "Bin/Shaders/lit_VS.spha";
constexpr const char* litFragShaderAsset = "Bin/Shaders/lit_FS.spha";

constexpr const char* unlitVertShaderAsset = "Bin/Shaders/lit_VS.spha";
constexpr const char* unlitFragShaderAsset = "Bin/Shaders/lit_FS.spha";

IMesh* squareMesh = nullptr;
constexpr const char* squareMeshAsset = "Bin/Meshes/Square_M.spha";


// Magikarp
IMesh* magikarpBodyMesh = nullptr;
IMesh* magikarpEyesMesh = nullptr;

IRenderMaterial* magikarpBodyMat = nullptr;
IRenderMaterial* magikarpEyesMat = nullptr;

// Gizmo
IRenderMaterial* gizmoMat = nullptr;

// Helmet
IMesh* helmetMesh = nullptr;
IRenderMaterial* helmetMat = nullptr;

// Bricks
IRenderMaterial* bricksMat = nullptr;

// Window.
IRenderMaterial* windowMat = nullptr;


void CreateDefaultResources(AssetManager& _assetMgr)
{
	// Lit Vertex Shader.
	if (!_assetMgr.shaderMgr.Load(litVertShaderAsset, true)) // Try load.
	{
		// Import on load failed.
		IAssetImportResult result = _assetMgr.Import("../../Engine/Resources/Shaders/lit.vert");
		result[0]->Save(litVertShaderAsset);
	}


	// Lit Fragment Shader.
	if (!_assetMgr.shaderMgr.Load(litFragShaderAsset, true)) // Try load.
	{
		// Import on load failed.
		IAssetImportResult result = _assetMgr.Import("../../Engine/Resources/Shaders/lit.frag");
		result[0]->Save(litFragShaderAsset);
	}


	// Unlit Vertex Shader.
	if (!_assetMgr.shaderMgr.Load(unlitVertShaderAsset, true)) // Try load.
	{
		// Import on load failed.
		IAssetImportResult result = _assetMgr.Import("../../Engine/Resources/Shaders/unlit.vert");
		result[0]->Save(unlitVertShaderAsset);
	}


	// Unlit fragment Shader.
	if (!_assetMgr.shaderMgr.Load(unlitFragShaderAsset, true)) // Try load.
	{
		// Import on load failed.
		IAssetImportResult result = _assetMgr.Import("../../Engine/Resources/Shaders/unlit.frag");
		result[0]->Save(unlitFragShaderAsset);
	}


	// Mesh.
#if !__SA_ALWAYS_REIMPORT
	squareMesh = _assetMgr.meshMgr.Load(squareMeshAsset);
#endif

	if (!squareMesh) // Try load.
	{
		RawMesh rawMesh;

		rawMesh.vertices =
		{
			{ { -0.5f, -0.5f, 0.0f }, Vec3f::Forward, Vec3f::Zero, { 1.0f, 0.0f } },
			{ { 0.5f, -0.5f, 0.0f }, Vec3f::Forward, Vec3f::Zero, { 0.0f, 0.0f } },
			{ { 0.5f, 0.5f, 0.0f }, Vec3f::Forward, Vec3f::Zero, { 0.0f, 1.0f } },
			{ { -0.5f, 0.5f, 0.0f }, Vec3f::Forward, Vec3f::Zero, { 1.0f, 1.0f } },
		};

		rawMesh.indices =
		{
			0, 1, 2, 2, 3, 0
		};

		// Create on load failed.
		MeshAsset gizmoMeshAsset = _assetMgr.meshMgr.Create(Move(rawMesh));
		gizmoMeshAsset.ComputeTangents();
		gizmoMeshAsset.Save(squareMeshAsset);
		squareMesh = gizmoMeshAsset.GetResource();
	}
}

void CreateMagikarp(AssetManager& _assetMgr)
{
	constexpr const char* textureAssets[] = {
		"Bin/Magikarp/Body_T.spha",
		//"Bin/Magikarp/BodyNorm_T.spha",
		//"Bin/Magikarp/BodyPow_T.spha",
		"Bin/Magikarp/Eyes_T.spha",
		//"Bin/Magikarp/EyesNorm_T.spha",
		//"Bin/Magikarp/EyesPow_T.spha"
	};
	constexpr const char* textureResources[] = {
		"../../Samples/Prototype/Resources/Magikarp/Body.png",
		//"../../Samples/Prototype/Resources/Magikarp/Body_norm.png",
		//"../../Samples/Prototype/Resources/Magikarp/Body_pow.png",
		"../../Samples/Prototype/Resources/Magikarp/Eyes.png",
		//"../../Samples/Prototype/Resources/Magikarp/Eyes_norm.png",
		//"../../Samples/Prototype/Resources/Magikarp/Eyes_pow.png"
	};

	constexpr const char* meshAssets[] = { "Bin/Magikarp/Body_M.spha", "Bin/Magikarp/Eyes_M.spha" };
	constexpr const char* materialAssets[] = { "Bin/Magikarp/Body_Mat.spha", "Bin/Magikarp/Eyes_Mat.spha" };


	for (uint32 i = 0u; i < SizeOf(textureAssets); ++i)
	{
		if (!_assetMgr.textureMgr.Load(textureAssets[i], true)) // Try load.
		{
			// Import on load failed.
			IAssetImportResult result = _assetMgr.Import(textureResources[i]);
			result[0]->Save(textureAssets[i]);
		}
	}


	// Model.
	{
		// Try load.
#if !__SA_ALWAYS_REIMPORT
		magikarpBodyMesh = _assetMgr.meshMgr.Load(meshAssets[0]);
		magikarpEyesMesh = _assetMgr.meshMgr.Load(meshAssets[1]);

		magikarpBodyMat = _assetMgr.renderMatMgr.Load(materialAssets[0]);
		magikarpEyesMat = _assetMgr.renderMatMgr.Load(materialAssets[1]);
#endif

		if (!magikarpBodyMesh || !magikarpEyesMesh || !magikarpBodyMat || !magikarpEyesMat)
		{
			// Import on load failed.
			IAssetImportResult result = _assetMgr.Import("../../Samples/Prototype/Resources/Magikarp/Magikarp.obj");
			
			// meshes
			result[0]->As<MeshAsset>().ComputeTangents();
			result[0]->Save(meshAssets[0]);
			magikarpBodyMesh = result[0]->As<MeshAsset>().GetResource();

			result[1]->As<MeshAsset>().ComputeTangents();
			result[1]->Save(meshAssets[1]);
			magikarpEyesMesh = result[1]->As<MeshAsset>().GetResource();


			RenderMaterialAsset& bodyRenderMat = result[2]->As<RenderMaterialAsset>();
			bodyRenderMat.vertexShaderPath = litVertShaderAsset;
			bodyRenderMat.fragmentShaderPath = litFragShaderAsset;
			bodyRenderMat.texturePaths = { textureAssets[0] };
			bodyRenderMat.Save(materialAssets[0]);
			magikarpBodyMat = bodyRenderMat.GetResource();


			RenderMaterialAsset& eyesRenderMat = result[3]->As<RenderMaterialAsset>();
			eyesRenderMat.vertexShaderPath = litVertShaderAsset;
			eyesRenderMat.fragmentShaderPath = litFragShaderAsset;
			eyesRenderMat.texturePaths = { textureAssets[1] };
			eyesRenderMat.Save(materialAssets[1]);
			magikarpEyesMat = eyesRenderMat.GetResource();
		}
	}
}

void CreateGizmo(AssetManager& _assetMgr)
{
	constexpr const char* vertShaderAsset = "Bin/Gizmo/Gizmo_VS.spha";
	constexpr const char* fragShaderAsset = "Bin/Gizmo/Gizmo_FS.spha";

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


	// Materials.
#if !__SA_ALWAYS_REIMPORT
	gizmoMat = _assetMgr.renderMatMgr.Load(materialAssets);
#endif

	if (!gizmoMat) // Try load.
	{
		// Import on load failed.
		RawMaterial gizmoRawMat;

		gizmoRawMat.uniformBufferSize = sizeof(Mat4f);
		gizmoRawMat.alphaModel = AlphaModel::Opaque;
		gizmoRawMat.cullingMode = CullingMode::None;
		gizmoRawMat.illumModel = IlluminationModel::None;

		RenderMaterialAsset gizmoMatAsset = _assetMgr.renderMatMgr.Create(Move(gizmoRawMat));
		
		gizmoMatAsset.vertexShaderPath = vertShaderAsset;
		gizmoMatAsset.fragmentShaderPath = fragShaderAsset;
		gizmoMatAsset.Save(materialAssets);

		gizmoMat = gizmoMatAsset.GetResource();
	}
}

void CreateHelmet(AssetManager& _assetMgr)
{
	constexpr const char* textureAssets[] = { "Bin/Helmet/Albedo_T.spha" };
	constexpr const char* meshAssets[] = { "Bin/Helmet/Helmet_M.spha" };
	constexpr const char* materialAssets[] = { "Bin/Helmet/Helmet_Mat.spha" };


	constexpr const char* textureResources[] = { "../../Samples/Prototype/Resources/DamagedHelmet/Default_albedo.png" };

	if (!_assetMgr.textureMgr.Load(textureAssets[0], true)) // Try load.
	{
		// Import on load failed.
		IAssetImportResult result = _assetMgr.Import(textureResources[0]);
		result[0]->Save(textureAssets[0]);
	}


	// Model.
	{
		// Try load.
#if !__SA_ALWAYS_REIMPORT
		helmetMesh = _assetMgr.meshMgr.Load(meshAssets[0]);
		helmetMat = _assetMgr.renderMatMgr.Load(materialAssets[0]);
#endif

		if (!helmetMesh || !helmetMesh)
		{
			// Import on load failed.
			IAssetImportResult result = _assetMgr.Import("../../Samples/Prototype/Resources/DamagedHelmet/DamagedHelmet.gltf");

			// meshes
			result[0]->Save(meshAssets[0]);
			helmetMesh = result[0]->As<MeshAsset>().GetResource();


			RenderMaterialAsset& bodyRenderMat = result[2]->As<RenderMaterialAsset>();
			bodyRenderMat.vertexShaderPath = litVertShaderAsset;
			bodyRenderMat.fragmentShaderPath = litFragShaderAsset;
			bodyRenderMat.texturePaths = { textureAssets[0] };
			bodyRenderMat.Save(materialAssets[0]);
			helmetMat = bodyRenderMat.GetResource();
		}
	}
}

void CreateBricks(AssetManager& _assetMgr)
{
	constexpr const char* textureAssets[] = {
	"Bin/Bricks/albedo_T.spha",
	"Bin/Bricks/normal_T.spha",
	};
	constexpr const char* textureResources[] = {
		"../../Samples/Prototype/Resources/Bricks/bricks.jpg",
		"../../Samples/Prototype/Resources/Bricks/bricks_normal.jpg"
	};

	constexpr const char* matAsset = "Bin/Bricks/brick_Mat.spha";


	// Textures

	for (uint32 i = 0u; i < SizeOf(textureAssets); ++i)
	{
		if (!_assetMgr.textureMgr.Load(textureAssets[i], true)) // Try load.
		{
			// Import on load failed.
			IAssetImportResult result = _assetMgr.Import(textureResources[i]);
			result[0]->As<TextureAsset>().FlipVertically();
			result[0]->Save(textureAssets[i]);
		}
	}



	// Material
	bricksMat = _assetMgr.renderMatMgr.Load(matAsset);

	if (!bricksMat)
	{
		RawMaterial infos;

		RenderMaterialAsset renderMatAsset = _assetMgr.renderMatMgr.Create(Move(infos));

		renderMatAsset.vertexShaderPath = litVertShaderAsset;
		renderMatAsset.fragmentShaderPath = litFragShaderAsset;
		renderMatAsset.texturePaths = { textureAssets[0], textureAssets[1] };

		renderMatAsset.Save(matAsset);

		bricksMat = renderMatAsset.GetResource();
	}
}

void CreateWindow(AssetManager& _assetMgr)
{
	constexpr const char* textureAssets[] = {
		"Bin/Window/albedo_T.spha",
	};
	constexpr const char* textureResources[] = {
		"../../Samples/Prototype/Resources/Window/RedWindow.png",
	};

	constexpr const char* matAsset = "Bin/Window/RedWindow_Mat.spha";


	// Textures
	for (uint32 i = 0u; i < SizeOf(textureAssets); ++i)
	{
		if (!_assetMgr.textureMgr.Load(textureAssets[i], true)) // Try load.
		{
			// Import on load failed.
			IAssetImportResult result = _assetMgr.Import(textureResources[i]);
			result[0]->As<TextureAsset>().FlipVertically();
			result[0]->Save(textureAssets[i]);
		}
	}


	// Material
	//windowMat = _assetMgr.renderMatMgr.Load(matAsset);

	if (!windowMat)
	{
		RawMaterial infos;

		infos.cullingMode = CullingMode::None;
		infos.alphaModel == AlphaModel::Transparent;

		RenderMaterialAsset renderMatAsset = _assetMgr.renderMatMgr.Create(Move(infos));

		renderMatAsset.vertexShaderPath = litVertShaderAsset;
		renderMatAsset.fragmentShaderPath = litFragShaderAsset;
		renderMatAsset.texturePaths = { textureAssets[0] };

		renderMatAsset.Save(matAsset);

		windowMat = renderMatAsset.GetResource();
	}
}

void CreateResources(AssetManager& _assetMgr)
{
	CreateDefaultResources(_assetMgr);
	CreateMagikarp(_assetMgr);
	CreateGizmo(_assetMgr);
	CreateBricks(_assetMgr);
	CreateWindow(_assetMgr);

	//CreateHelmet(_assetMgr);
}

void DestroyResources(AssetManager& _assetMgr)
{
	// === Gizmo ===
	_assetMgr.renderMatMgr.Unload(gizmoMat);


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

	RenderPassCreateInfos mainRenderPassInfos;
	mainRenderPassInfos.sampling = SampleBits::Sample8Bits;

	Sa::VkRenderPass& mainRenderPass = surface.CreateRenderPass(instance, mainRenderPassInfos).As<Sa::VkRenderPass>();
	IRenderPass::mainRenderPass = &mainRenderPass;

	VkCamera mainCamera(surface.GetImageExtent());
	mainCamera.Create(instance, surface);
	VkCamera::mainCamera = &mainCamera;

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

	//uint32 pLight2ID = instance.InstantiatePointLight(pLight2);


	// Magikarp
	{
		DefaultUniformBuffer ubo;
		ubo.modelMat = API_ConvertCoordinateSystem(TransffPRS(Vec3f(-1.0f, -1.0, -2.5f),
			Quatf(180, Vec3f::Up) * Quatf(-90, Vec3f::Right), Vec3f(0.000001f)).Matrix());
		ubo.normalMat = !Maths::Equals0(ubo.modelMat.Determinant()) ?  ubo.modelMat.GetInversed().Transpose() : Mat4f::Identity;

		magikarpBodyMat->InitVariable(instance, &ubo, sizeof(ubo));
		magikarpEyesMat->InitVariable(instance, &ubo, sizeof(ubo));
	}


	// Bricks
	{
		DefaultUniformBuffer ubo;
		ubo.modelMat = API_ConvertCoordinateSystem(TransffPRS(Vec3f(6.0f, 0.0, 2.0f),
			Quatf(-90.0f, Vec3f::Up), Vec3f(5.0f)).Matrix());
		ubo.normalMat = !Maths::Equals0(ubo.modelMat.Determinant()) ? ubo.modelMat.GetInversed().Transpose() : Mat4f::Identity;
		
		bricksMat->InitVariable(instance, &ubo, sizeof(ubo));
	}


	// Window.
	{
		DefaultUniformBuffer ubo;
		ubo.modelMat = API_ConvertCoordinateSystem(TransffPRS(Vec3f(-6.0f, 0.0, 2.0f),
			Quatf(90.0f, Vec3f::Up), Vec3f(5.0f)).Matrix());
		ubo.normalMat = !Maths::Equals0(ubo.modelMat.Determinant()) ? ubo.modelMat.GetInversed().Transpose() : Mat4f::Identity;
		
		windowMat->InitVariable(instance, &ubo, sizeof(ubo));
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
	const float f = 20.0f;

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
		{
			CameraUniformBuffer ubo;
			//ubo.modelMat = Mat4f::MakeRotation(Quatf(time, Vec3f::Right));
			ubo.viewInvMat = API_ConvertCoordinateSystem(camTr.Matrix()).Inverse();
			ubo.projMat = perspMat;
			ubo.viewPosition = API_ConvertCoordinateSystem(camTr.position);

			mainCamera.GetUniformBuffer(frame.index).UpdateData(instance.GetDevice(), &ubo, sizeof(ubo));
		}
		
		{
			Mat4f modelMat = API_ConvertCoordinateSystem(TransffPRS(pL1Pos, Quatf::Identity, Vec3f::One * 0.5f).Matrix());
			gizmoMat->UpdateVariable(instance, frame, &modelMat, sizeof(Mat4f));

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


		// Draw bricks.
		bricksMat->Bind(frame);
		squareMesh->Draw(frame);

		
		// Draw window.
		windowMat->Bind(frame);
		squareMesh->Draw(frame);


		// Draw gizmos.
		gizmoMat->Bind(frame);
		squareMesh->Draw(frame);


		mainRenderPass.End(frame);


		surface.GetSwapChain().End(instance.GetDevice());
	}



	// === Destroy ===
	vkDeviceWaitIdle(instance.GetDevice());

	DestroyResources(assetMgr);

	mainCamera.Destroy(instance);

	instance.DestroyRenderSurface(window);
	
	window.Destroy();

	instance.Destroy();


	// === UnInit ===
	IWindow::UnInit();
	IRenderInstance::UnInit();


	LOG("\n=== End ===");

	return 0;
}
