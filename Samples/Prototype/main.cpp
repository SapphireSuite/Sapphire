// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <string>
#include <iostream>
#include <filesystem>

#include <Sapphire/Core/Time/Chrono.hpp>
#include <Sapphire/Maths/Space/Transform.hpp>

#include <Sapphire/Window/GLFWWindow.hpp>

#include <Sapphire/Rendering/Misc/APISpecific.hpp>
#include <Sapphire/Rendering/Framework/Primitives/Mesh/IMesh.hpp>
#include <Sapphire/Rendering/Framework/Primitives/Material/UniformBuffers.hpp>
#include <Sapphire/Rendering/Framework/Primitives/Material/IRenderMaterial.hpp>

#include <Sapphire/Rendering/Vulkan/System/VkRenderInstance.hpp>
#include <Sapphire/Rendering/Vulkan/System/VkRenderPass.hpp>

#include <Sapphire/SDK/Assets/AssetManager.hpp>
#include <Sapphire/SDK/Assets/Texture/TextureImportInfos.hpp>
#include <Sapphire/SDK/Assets/Texture/CubemapImportInfos.hpp>

using namespace Sa;

#define __SA_ALWAYS_REIMPORT 0

#define LOG(_str) std::cout << _str << std::endl;

// Default
constexpr const char* litVertShaderAssetPath = "Bin/Engine/Shaders/lit_VS.spha";
constexpr const char* litFragShaderAssetPath = "Bin/Engine/Shaders/lit_FS.spha";

constexpr const char* unlitVertShaderAssetPath = "Bin/Engine/Shaders/unlit_VS.spha";
constexpr const char* unlitFragShaderAssetPath = "Bin/Engine/Shaders/unlit_FS.spha";

constexpr const char* missingTextureAssetPath = "Bin/Engine/Textures/missing_T.spha";
constexpr const char* BRDFLookUpTableTextureAssetPath = "Bin/Engine/Textures/brdf_lut_T.spha";

IMesh* squareMesh = nullptr;
constexpr const char* squareMeshAssetPath = "Bin/Meshes/Square_M.spha";

IMesh* cubeMesh = nullptr;
constexpr const char* cubeMeshAssetPath = "Bin/Meshes/Cube_M.spha";


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

// Skybox.
IRenderMaterial* skyboxMat = nullptr;

// Spheres.
IMesh* sphereMesh = nullptr;
uint32 sphereMatNum = 0u;
IRenderMaterial** sphereMats = nullptr;


void CreateDefaultResources(AssetManager& _assetMgr)
{
	// Lit Vertex Shader.
	if (!_assetMgr.shaderMgr.Load(litVertShaderAssetPath, true)) // Try load.
	{
		// Import on load failed.
		auto result = _assetMgr.importer.Import("../../Engine/Resources/Shaders/lit.vert");
		result->Save(litVertShaderAssetPath);
	}


	// Lit Fragment Shader.
	if (!_assetMgr.shaderMgr.Load(litFragShaderAssetPath, true)) // Try load.
	{
		// Import on load failed.
		auto result = _assetMgr.importer.Import("../../Engine/Resources/Shaders/lit.frag");
		result->Save(litFragShaderAssetPath);
	}


	// Unlit Vertex Shader.
	if (!_assetMgr.shaderMgr.Load(unlitVertShaderAssetPath, true)) // Try load.
	{
		// Import on load failed.
		auto result = _assetMgr.importer.Import("../../Engine/Resources/Shaders/unlit.vert");
		result->Save(unlitVertShaderAssetPath);
	}


	// Unlit fragment Shader.
	if (!_assetMgr.shaderMgr.Load(unlitFragShaderAssetPath, true)) // Try load.
	{
		// Import on load failed.
		auto result = _assetMgr.importer.Import("../../Engine/Resources/Shaders/unlit.frag");
		result->Save(unlitFragShaderAssetPath);
	}


	// Textures
	if (!_assetMgr.textureMgr.Load(missingTextureAssetPath, true)) // Try load.
	{
		// Import on load failed.
		TextureImportInfos infos; infos.format = TextureFormat::sRGB;
		auto result = _assetMgr.importer.Import("../../Engine/Resources/Textures/missing_texture.png", &infos);
		result->Save(missingTextureAssetPath);
	}
	if (!(ITexture::brdfLUT = _assetMgr.textureMgr.Load(BRDFLookUpTableTextureAssetPath, true))) // Try load.
	{
		// Import on load failed.
		TextureImportInfos infos; infos.mipLevels = 1u;
		auto result = _assetMgr.importer.Import("../../Engine/Resources/Textures/brdf_lut.jpg");
		result->Save(BRDFLookUpTableTextureAssetPath);
		ITexture::brdfLUT = result->AsPtr<TextureAsset>()->GetResource();
	}



	// Mesh.
	squareMesh = _assetMgr.meshMgr.Load(squareMeshAssetPath);

	if (!squareMesh) // Try load.
	{
		// Create on load failed.
		MeshAsset meshAsset = _assetMgr.meshMgr.Create(Move(RawMesh::SquareMesh<VertexComp::PNTanTex>()));
		meshAsset.Save(squareMeshAssetPath);
		squareMesh = meshAsset.GetResource();
	}

	cubeMesh = _assetMgr.meshMgr.Load(cubeMeshAssetPath);

	if (!cubeMesh) // Try load.
	{
		// Create on load failed.
		MeshAsset meshAsset = _assetMgr.meshMgr.Create(Move(RawMesh::CubeMesh<VertexComp::PNTanTex>()));
		meshAsset.Save(cubeMeshAssetPath);
		cubeMesh = meshAsset.GetResource();
	}
}

void CreateMagikarp(IRenderInstance& _instance, AssetManager& _assetMgr)
{
	constexpr const char* textureAssetPaths[] = {
		"Bin/Magikarp/Body_T.spha",
		"Bin/Magikarp/Eyes_T.spha",

		"Bin/Magikarp/BodyNorm_T.spha",
		"Bin/Magikarp/BodyHeight_T.spha",
		"Bin/Magikarp/EyesNorm_T.spha",
		"Bin/Magikarp/EyesHeight_T.spha"
	};
	constexpr const char* textureResourcePaths[] = {
		"../../Samples/Prototype/Resources/Magikarp/Body.png",
		"../../Samples/Prototype/Resources/Magikarp/Eyes.png",

		"../../Samples/Prototype/Resources/Magikarp/Body_norm.png",
		"../../Samples/Prototype/Resources/Magikarp/Body_pow.png",
		"../../Samples/Prototype/Resources/Magikarp/Eyes_norm.png",
		"../../Samples/Prototype/Resources/Magikarp/Eyes_pow.png"
	};

	constexpr const char* meshAssetPaths[] = { "Bin/Magikarp/Body_M.spha", "Bin/Magikarp/Eyes_M.spha" };
	constexpr const char* materialAssetPaths[] = { "Bin/Magikarp/Body_Mat.spha", "Bin/Magikarp/Eyes_Mat.spha" };


	// Textures.
	for (uint32 i = 0u; i < SizeOf(textureAssetPaths); ++i)
	{
		if (!_assetMgr.textureMgr.Load(textureAssetPaths[i], true)) // Try load.
		{
			// Import on load failed.
			TextureImportInfos infos;

			if(i < 2)
				infos.format = TextureFormat::sRGB;

			auto result = _assetMgr.importer.Import(textureResourcePaths[i], &infos);
			result->Save(textureAssetPaths[i]);
		}
	}


	// Model.
	{
		// Try load.
		magikarpBodyMesh = _assetMgr.meshMgr.Load(meshAssetPaths[0]);
		magikarpEyesMesh = _assetMgr.meshMgr.Load(meshAssetPaths[1]);

		magikarpBodyMat = _assetMgr.renderMatMgr.Load(materialAssetPaths[0]);
		magikarpEyesMat = _assetMgr.renderMatMgr.Load(materialAssetPaths[1]);

		if (!magikarpBodyMesh || !magikarpEyesMesh || !magikarpBodyMat || !magikarpEyesMat)
		{
			// Import on load failed.
			auto resPtr = _assetMgr.importer.Import("../../Samples/Prototype/Resources/Magikarp/Magikarp.obj");
			ModelAsset& result = resPtr->As<ModelAsset>();
			
			// meshes
			//result.meshes[0].ComputeTangents();
			result.meshes[0].Save(meshAssetPaths[0]);
			magikarpBodyMesh = result.meshes[0].GetResource();

			//result.meshes[1].ComputeTangents();
			result.meshes[1].Save(meshAssetPaths[1]);
			magikarpEyesMesh = result.meshes[1].GetResource();


			RenderMaterialAsset& bodyRenderMat = result.renderMats[0];
			bodyRenderMat.vertexShaderPath = litVertShaderAssetPath;
			bodyRenderMat.fragmentShaderPath = litFragShaderAssetPath;
			bodyRenderMat.texturePaths = { textureAssetPaths[0] };
			bodyRenderMat.mRawData.vertexBindingLayout.meshLayout = magikarpBodyMesh->GetLayout(); // TODO: Clean?
			bodyRenderMat.Save(materialAssetPaths[0]);
			magikarpBodyMat = bodyRenderMat.GetResource();


			RenderMaterialAsset& eyesRenderMat = result.renderMats[1];
			eyesRenderMat.vertexShaderPath = litVertShaderAssetPath;
			eyesRenderMat.fragmentShaderPath = litFragShaderAssetPath;
			eyesRenderMat.texturePaths = { textureAssetPaths[1] };
			eyesRenderMat.mRawData.vertexBindingLayout.meshLayout = magikarpEyesMesh->GetLayout(); // TODO: Clean?
			eyesRenderMat.Save(materialAssetPaths[1]);
			magikarpEyesMat = eyesRenderMat.GetResource();
		}

		DefaultUniformBuffer ubo;
		ubo.modelMat = API_ConvertCoordinateSystem(TransffPRS(Vec3f(-1.0f, -1.0, 20.0f),
			Quatf(-90, Vec3f::Right), Vec3f(0.000001f)).Matrix());
		ubo.normalMat = !Maths::Equals0(ubo.modelMat.Determinant()) ? ubo.modelMat.GetInversed().Transpose() : Mat4f::Identity;

		magikarpBodyMat->InitVariable(_instance, &ubo, sizeof(ubo));
		magikarpEyesMat->InitVariable(_instance, &ubo, sizeof(ubo));
	}
}

void CreateGizmo(IRenderInstance& _instance, AssetManager& _assetMgr)
{
	constexpr const char* vertShaderAssetPath = "Bin/Gizmo/Gizmo_VS.spha";
	constexpr const char* fragShaderAssetPath = "Bin/Gizmo/Gizmo_FS.spha";

	constexpr const char* materialAssetPath = "Bin/Gizmo/Gizmo_Mat.spha";

	// Vertex Shader.
	if (!_assetMgr.shaderMgr.Load(vertShaderAssetPath, true)) // Try load.
	{
		// Import on load failed.
		auto result = _assetMgr.importer.Import("../../Engine/Resources/Shaders/gizmo.vert");
		result->Save(vertShaderAssetPath);
	}

	// Fragment Shader.
	if (!_assetMgr.shaderMgr.Load(fragShaderAssetPath, true)) // Try load.
	{
		// Import on load failed.
		auto result = _assetMgr.importer.Import("../../Engine/Resources/Shaders/gizmo.frag");
		result->Save(fragShaderAssetPath);
	}


	// Materials.
	gizmoMat = _assetMgr.renderMatMgr.Load(materialAssetPath);

	if (!gizmoMat) // Try load.
	{
		// Import on load failed.
		RawMaterial gizmoRawMat;

		gizmoRawMat.vertexBindingLayout.desiredLayout = VertexLayout::Make<VertexComp::PTex>();
		gizmoRawMat.vertexBindingLayout.meshLayout = squareMesh->GetLayout();

		gizmoRawMat.uniformBufferSize = sizeof(Mat4f);
		gizmoRawMat.alphaModel = AlphaModel::Opaque;
		gizmoRawMat.cullingMode = CullingMode::None;
		gizmoRawMat.pipelineFlags = 0;

		RenderMaterialAsset gizmoMatAsset = _assetMgr.renderMatMgr.Create(Move(gizmoRawMat));
		
		gizmoMatAsset.vertexShaderPath = vertShaderAssetPath;
		gizmoMatAsset.fragmentShaderPath = fragShaderAssetPath;
		gizmoMatAsset.Save(materialAssetPath);

		gizmoMat = gizmoMatAsset.GetResource();
	}
}

void CreateHelmet(IRenderInstance& _instance, AssetManager& _assetMgr)
{
	constexpr const char* textureAssets[] = { "Bin/Helmet/Albedo_T.spha" };
	constexpr const char* meshAssets[] = { "Bin/Helmet/Helmet_M.spha" };
	constexpr const char* materialAssets[] = { "Bin/Helmet/Helmet_Mat.spha" };


	constexpr const char* textureResources[] = { "../../Samples/Prototype/Resources/DamagedHelmet/Default_albedo.png" };

	if (!_assetMgr.textureMgr.Load(textureAssets[0], true)) // Try load.
	{
		// Import on load failed.
		auto result = _assetMgr.importer.Import(textureResources[0]);
		result->Save(textureAssets[0]);
	}


	// Model.
	{
		// Try load.
		helmetMesh = _assetMgr.meshMgr.Load(meshAssets[0]);
		helmetMat = _assetMgr.renderMatMgr.Load(materialAssets[0]);

		if (!helmetMesh || !helmetMesh)
		{
			// Import on load failed.
			auto resPtr = _assetMgr.importer.Import("../../Samples/Prototype/Resources/DamagedHelmet/DamagedHelmet.gltf");
			ModelAsset& result = resPtr->As<ModelAsset>();

			// meshes
			result.meshes[0].Save(meshAssets[0]);
			helmetMesh = result.meshes[0].GetResource();


			RenderMaterialAsset& bodyRenderMat = result.renderMats[0];
			bodyRenderMat.vertexShaderPath = litVertShaderAssetPath;
			bodyRenderMat.fragmentShaderPath = litFragShaderAssetPath;
			bodyRenderMat.texturePaths = { textureAssets[0] };
			bodyRenderMat.Save(materialAssets[0]);
			helmetMat = bodyRenderMat.GetResource();
		}
	}
}

void CreateBricks(IRenderInstance& _instance, AssetManager& _assetMgr)
{
	constexpr const char* textureAssetPaths[] = {
	"Bin/Bricks/albedo_T.spha",
	"Bin/Bricks/normal_T.spha",
	};
	constexpr const char* textureResourcePaths[] = {
		"../../Samples/Prototype/Resources/Bricks/bricks.jpg",
		"../../Samples/Prototype/Resources/Bricks/bricks_normal.jpg"
	};

	constexpr const char* matAssetPath = "Bin/Bricks/brick_Mat.spha";


	// Textures
	for (uint32 i = 0u; i < SizeOf(textureAssetPaths); ++i)
	{
		if (!_assetMgr.textureMgr.Load(textureAssetPaths[i], true)) // Try load.
		{
			// Import on load failed.
			TextureImportInfos infos;

			if (i < 1)
				infos.format = TextureFormat::sRGB;

			auto result = _assetMgr.importer.Import(textureResourcePaths[i], &infos);
			result->Save(textureAssetPaths[i]);
		}
	}



	// Material
	bricksMat = _assetMgr.renderMatMgr.Load(matAssetPath);

	if (!bricksMat)
	{
		RawMaterial infos;

		RenderMaterialAsset renderMatAsset = _assetMgr.renderMatMgr.Create(Move(infos));

		renderMatAsset.mRawData.vertexBindingLayout.meshLayout = cubeMesh->GetLayout();

		renderMatAsset.vertexShaderPath = litVertShaderAssetPath;
		renderMatAsset.fragmentShaderPath = litFragShaderAssetPath;
		renderMatAsset.texturePaths = { textureAssetPaths[0], textureAssetPaths[1] };

		renderMatAsset.Save(matAssetPath);

		bricksMat = renderMatAsset.GetResource();
	}

	DefaultUniformBuffer ubo;
	ubo.modelMat = API_ConvertCoordinateSystem(TransffPRS(Vec3f(15.0f, 0.0, 22.0f),
		Quatf(-90.0f, Vec3f::Up), Vec3f(5.0f)).Matrix());
	ubo.normalMat = !Maths::Equals0(ubo.modelMat.Determinant()) ? ubo.modelMat.GetInversed().Transpose() : Mat4f::Identity;

	bricksMat->InitVariable(_instance, &ubo, sizeof(ubo));
}

void CreateWindow(IRenderInstance& _instance, AssetManager& _assetMgr)
{
	constexpr const char* textureAssetPaths[] = {
		"Bin/Window/albedo_T.spha",
	};
	constexpr const char* textureResourcePaths[] = {
		"../../Samples/Prototype/Resources/Window/RedWindow.png",
	};

	constexpr const char* matAssetPath = "Bin/Window/RedWindow_Mat.spha";


	// Textures
	for (uint32 i = 0u; i < SizeOf(textureAssetPaths); ++i)
	{
		if (!_assetMgr.textureMgr.Load(textureAssetPaths[i], true)) // Try load.
		{
			// Import on load failed.
			TextureImportInfos infos;
			infos.format = TextureFormat::sRGB;

			auto result = _assetMgr.importer.Import(textureResourcePaths[i], &infos);
			result->Save(textureAssetPaths[i]);
		}
	}


	// Material
	windowMat = _assetMgr.renderMatMgr.Load(matAssetPath);

	if (!windowMat)
	{
		RawMaterial infos;

		infos.matConstants.shininess = 100.0f;
		infos.matConstants.ambient = Vec3f(1.0f, 0.5f, 0.5f);
		infos.matConstants.diffuse = Vec3f(1.0f, 0.5f, 0.5f);
		infos.matConstants.specular = Vec3f(1.0f, 0.5f, 0.5f);

		infos.cullingMode = CullingMode::None;
		infos.alphaModel = AlphaModel::Transparent;

		RenderMaterialAsset renderMatAsset = _assetMgr.renderMatMgr.Create(Move(infos));

		renderMatAsset.mRawData.vertexBindingLayout.meshLayout = squareMesh->GetLayout();
		renderMatAsset.vertexShaderPath = litVertShaderAssetPath;
		renderMatAsset.fragmentShaderPath = litFragShaderAssetPath;
		renderMatAsset.texturePaths = { textureAssetPaths[0] };

		renderMatAsset.Save(matAssetPath);

		windowMat = renderMatAsset.GetResource();
	}

	DefaultUniformBuffer ubo;
	ubo.modelMat = API_ConvertCoordinateSystem(TransffPRS(Vec3f(-15.0f, 0.0, 22.0f),
		Quatf(90.0f, Vec3f::Up), Vec3f(5.0f)).Matrix());
	ubo.normalMat = !Maths::Equals0(ubo.modelMat.Determinant()) ? ubo.modelMat.GetInversed().Transpose() : Mat4f::Identity;

	windowMat->InitVariable(_instance, &ubo, sizeof(ubo));
}

void CreateSkybox(IRenderInstance& _instance, AssetManager& _assetMgr)
{
	constexpr const char* textureAsset = "Bin/Skybox/Skybox_Cube_CM.spha";
	const CubemapAssetImportInfos cubemapInfos(
		"../../Samples/Prototype/Resources/Skybox/Cube/Forest/left.bmp",
		"../../Samples/Prototype/Resources/Skybox/Cube/Forest/right.bmp",
		"../../Samples/Prototype/Resources/Skybox/Cube/Forest/bottom.bmp",
		"../../Samples/Prototype/Resources/Skybox/Cube/Forest/top.bmp",
		"../../Samples/Prototype/Resources/Skybox/Cube/Forest/front.bmp",
		"../../Samples/Prototype/Resources/Skybox/Cube/Forest/back.bmp",
		"../../Samples/Prototype/Resources/Skybox/Cube/Forest/left_irr.bmp",
		"../../Samples/Prototype/Resources/Skybox/Cube/Forest/right_irr.bmp",
		"../../Samples/Prototype/Resources/Skybox/Cube/Forest/bottom_irr.bmp",
		"../../Samples/Prototype/Resources/Skybox/Cube/Forest/top_irr.bmp",
		"../../Samples/Prototype/Resources/Skybox/Cube/Forest/front_irr.bmp",
		"../../Samples/Prototype/Resources/Skybox/Cube/Forest/back_irr.bmp");
	
	constexpr const char* skyboxVertShaderAsset = "Bin/Shaders/skybox_VS.spha";
	constexpr const char* skyboxFragShaderAsset = "Bin/Shaders/skybox_FS.spha";

	constexpr const char* matAsset = "Bin/Skybox/Skybox_Cube_Mat.spha";

	// Vertex Shader.
	if (!_assetMgr.shaderMgr.Load(skyboxVertShaderAsset, true)) // Try load.
	{
		// Import on load failed.
		auto result = _assetMgr.importer.Import("../../Engine/Resources/Shaders/skybox.vert");
		result->Save(skyboxVertShaderAsset);
	}


	// Fragment Shader.
	if (!_assetMgr.shaderMgr.Load(skyboxFragShaderAsset, true)) // Try load.
	{
		// Import on load failed.
		auto result = _assetMgr.importer.Import("../../Engine/Resources/Shaders/skybox.frag");
		result->Save(skyboxFragShaderAsset);
	}


	ICubemap::main = _assetMgr.cubemapMgr.Load(textureAsset, true);
	if (!ICubemap::main) // Try load.
	{
		// Import on load failed.
		CubemapAsset result = _assetMgr.importer.Import(cubemapInfos);
		result.Save(textureAsset);
		ICubemap::main = result.GetResource();
	}


	// Material
	skyboxMat = _assetMgr.renderMatMgr.Load(matAsset);

	if (!skyboxMat)
	{
		RawMaterial infos;
		infos.vertexBindingLayout.desiredLayout = VertexLayout::Make<VertexComp::Position>();
		infos.vertexBindingLayout.meshLayout = cubeMesh->GetLayout();

		infos.cullingMode = CullingMode::None;
		infos.pipelineFlags = 0;
		infos.uniformBufferSize = 0u;

		RenderMaterialAsset renderMatAsset = _assetMgr.renderMatMgr.Create(Move(infos));

		renderMatAsset.vertexShaderPath = skyboxVertShaderAsset;
		renderMatAsset.fragmentShaderPath = skyboxFragShaderAsset;
		//renderMatAsset.texturePaths = { textureAsset };

		renderMatAsset.Save(matAsset);

		skyboxMat = renderMatAsset.GetResource();
	}
}

void CreateSpheres(IRenderInstance& _instance, AssetManager& _assetMgr)
{
	constexpr const char* meshAssetPath = "Bin/Spheres/sphere_M.spha";

	std::vector<std::string> names =
	{
		"Bricks",
		"Gold",
		"Grass",
		"Hardwood",
		"IndustrialBricks",
		"Rusted",
		"Rusted2",
		"ScuffedGold",
		"ScuffedTitanium",
		"Shoreline",
		"Snow",
		"WarpedSheet",
		"Worn",
		"WornShiny",
	};
	std::vector<std::vector<std::string>> matResourcePaths =
	{
		{
			"red-bricks2_albedo",
			"red-bricks2_normal",
			"red-bricks2_height",
			"red-bricks2_metallness",
			"red-bricks2_roughness",
			"red-bricks2_ao"
		},
		{
			"lightgold_albedo",
			"lightgold_normal",
			"",
			"lightgold_metallic",
			"lightgold_roughness"
		},
		{
			"grass1-albedo3",
			"grass1-normal",
			"grass1-height",
			"",
			"grass1-rough",
			"grass1-ao"
		},
		{
			"hardwood-brown-planks-albedo",
			"hardwood-brown-planks-normal",
			"hardwood-brown-planks-height",
			"hardwood-brown-planks-metallic",
			"hardwood-brown-planks-roughness",
			"hardwood-brown-planks-ao"
		},
		{
			"industrial-narrow-brick-albedo",
			"industrial-narrow-brick-normal",
			"industrial-narrow-brick-height",
			"industrial-narrow-brick-metallic",
			"industrial-narrow-brick-roughness",
			"industrial-narrow-brick-ao"
		},
		{
			"rustediron-streaks_basecolor",
			"rustediron-streaks_normal",
			"",
			"rustediron-streaks_metallic",
			"rustediron-streaks_roughness"
		},
		{
			"rustediron2_basecolor",
			"rustediron2_normal",
			"",
			"rustediron2_metallic",
			"rustediron2_roughness"
		},
		{
			"gold-scuffed_basecolor-boosted",
			"gold-scuffed_normal",
			"",
			"gold-scuffed_metallic",
			"gold-scuffed_roughness",
		},
		{
			"Titanium-Scuffed_basecolor",
			"Titanium-Scuffed_normal",
			"",
			"Titanium-Scuffed_metallic",
			"Titanium-Scuffed_roughness"
		},
		{
			"rocky-shoreline1-albedo",
			"rocky-shoreline1-normal",
			"rocky-shoreline1-height",
			"rocky-shoreline1-metallic",
			"rocky-shoreline1-roughness",
			"rocky-shoreline1-ao"
		},
		{
			"snowdrift1_albedo",
			"snowdrift1_Normal",
			"snowdrift1_Height",
			"snowdrift1_Metallic",
			"snowdrift1_Roughness",
			"snowdrift1_ao"
		},
		{
			"warped-sheet-metal_albedo",
			"warped-sheet-metal_normal",
			"warped-sheet-metal_height",
			"warped-sheet-metal_metallic",
			"warped-sheet-metal_roughness",
			"warped-sheet-metal_ao"
		},
		{
			"worn_metal4_albedo",
			"worn_metal4_Normal",
			"worn_metal4_Height",
			"worn_metal4_Metallic",
			"worn_metal4_Roughness",
			"worn_metal4_ao"
		},
		{
			"worn-shiny-metal-albedo",
			"worn-shiny-metal-Normal",
			"worn-shiny-metal-Height",
			"worn-shiny-metal-Metallic",
			"worn-shiny-metal-Roughness",
			"worn-shiny-metal-ao"
		}
	};

	TextureImportInfos textureImpIfos[6];
	textureImpIfos[0].format = TextureFormat::sRGB; // albedo.
	textureImpIfos[3].channelNum = 1u; // metallness.
	textureImpIfos[4].channelNum = 1u; // roughness.
	textureImpIfos[5].channelNum = 1u; // ambiant occlusion.


	sphereMatNum = SizeOf(names);
	sphereMats = new IRenderMaterial*[sphereMatNum] {};

	// Textures.
	{
		std::string sphaPath;
		std::string resPath;

		for (uint32 i = 0u; i < sphereMatNum; ++i)
		{
			for (uint32 j = 0u; j < SizeOf(matResourcePaths[i]); ++j)
			{
				if (matResourcePaths[i][j].empty())
					sphaPath = missingTextureAssetPath;
				else
				{
					sphaPath = "Bin/Spheres/" + names[i] + '/' + matResourcePaths[i][j] + ".spha";
					resPath = "../../Samples/Prototype/Resources/Spheres/" + names[i] + '/' + matResourcePaths[i][j] + ".png";
				}

				if (!_assetMgr.textureMgr.Load(sphaPath, true)) // Try load.
				{
					// Import on load failed.
					auto result = _assetMgr.importer.Import(resPath, &textureImpIfos[j]);
					result->Save(sphaPath);
				}
			}
		}
	}

	// Mesh.
	sphereMesh = _assetMgr.meshMgr.Load(meshAssetPath);
	
	if (!sphereMesh)
	{
		// Import on load failed.
		auto resPtr = _assetMgr.importer.Import("../../Samples/Prototype/Resources/Spheres/sphere.obj");
		ModelAsset& result = resPtr->As<ModelAsset>();

		// meshes
		result.meshes[0].Save(meshAssetPath);
		sphereMesh = result.meshes[0].GetResource();
	}

	// Materials.
	for (uint32 i = 0u; i < sphereMatNum; ++i)
	{
		std::string path = "Bin/Spheres/" + names[i] + '/' + names[i] + "_Mat.spha";

		sphereMats[i] = _assetMgr.renderMatMgr.Load(path);

		if (!sphereMats[i])
		{
			RawMaterial infos;

			RenderMaterialAsset renderMatAsset = _assetMgr.renderMatMgr.Create(Move(infos));

			renderMatAsset.mRawData.vertexBindingLayout.meshLayout = sphereMesh->GetLayout();

			renderMatAsset.vertexShaderPath = litVertShaderAssetPath;
			renderMatAsset.fragmentShaderPath = litFragShaderAssetPath;

			renderMatAsset.texturePaths.reserve(matResourcePaths[i].size());

			for (uint32 j = 0u; j < SizeOf(matResourcePaths[i]); ++j)
			{
				if (matResourcePaths[i][j].empty())
					renderMatAsset.texturePaths.emplace_back(missingTextureAssetPath);
				else
					renderMatAsset.texturePaths.emplace_back("Bin/Spheres/" + names[i] + '/' + matResourcePaths[i][j] + ".spha");
			}

			renderMatAsset.Save(path);

			sphereMats[i] = renderMatAsset.GetResource();
		}
	}


	uint32 x = 0u;
	uint32 y = 0u;

	for (uint32 i = 0u; i < sphereMatNum; ++i)
	{
		DefaultUniformBuffer ubo;
		ubo.modelMat = API_ConvertCoordinateSystem(Mat4f::MakeTransform(Vec3f(-10.0f + x * 7.5f, -2.0f + y * 7.5f, -5.0f), Quatf::Identity, Vec3f::One * 0.1f));
		ubo.normalMat = !Maths::Equals0(ubo.modelMat.Determinant()) ? ubo.modelMat.GetInversed().Transpose() : Mat4f::Identity;

		sphereMats[i]->InitVariable(_instance, &ubo, sizeof(ubo));

		y = (y + 1) % 3;

		if (y == 0)
			++x;
	}
}

void CreateResources(IRenderInstance& _instance, AssetManager& _assetMgr)
{
	CreateDefaultResources(_assetMgr);

	CreateSkybox(_instance, _assetMgr);

	CreateSpheres(_instance, _assetMgr);

	CreateGizmo(_instance, _assetMgr);

	//CreateMagikarp(_instance, _assetMgr);
	//CreateBricks(_instance, _assetMgr);
	//CreateWindow(_instance, _assetMgr);

	//CreateHelmet(_instance, _assetMgr);
}


int main()
{
	LOG("=== Start ===");


#if __SA_ALWAYS_REIMPORT
	std::filesystem::remove_all("Bin");
#endif

	// === Init ===
	IRenderInstance::Init();
	IWindow::Init();


	// === Create ===
	VkRenderInstance instance;
	instance.Create();

	GLFWWindow window;
	window.Create(1200u, 800u);

	VkRenderSurface& surface = const_cast<VkRenderSurface&>(static_cast<const VkRenderSurface&>(instance.CreateRenderSurface(window)));

	RenderPassCreateInfos mainRenderPassInfos;
	mainRenderPassInfos.sampling = SampleBits::Sample8Bits;
	mainRenderPassInfos.extent = surface.ChooseSwapExtent();
	mainRenderPassInfos.format = surface.ChooseSwapSurfaceFormat().format;

	Sa::RenderPass& mainRenderPass = surface.CreateRenderPass(instance, mainRenderPassInfos).As<Sa::RenderPass>();

	Camera& mainCamera = instance.InstantiateCamera();
	mainCamera.SetPosition(Vec3f(-2.0f, 2.0f, 5.0f));
	TransffPRS camTr = mainCamera.GetTransform();
	surface.Create(instance.GetDevice(), instance.GetDevice().GetQueueFamilyIndices(), mainRenderPass);

	AssetManager assetMgr(instance);
	CreateResources(instance, assetMgr);


	// Create Lights.
	PLightInfos pLight1;
	pLight1.position = API_ConvertCoordinateSystem(Vec3f(-1.0f, 2.0f, 0.0f));
	//pLight1.color = Vec3f(0.9f, 0.7f, 0.3f);
	pLight1.intensity = 5.0f;

	uint32 pLight1ID = uint32(-1);
	//pLight1ID = instance.InstantiatePointLight(pLight1);

	//PLightInfos pLight2;
	//pLight2.position = API_ConvertCoordinateSystem(Vec3f(2.0f, 2.0f, -2.0f));
	//pLight2.color = Vec3f(0.9f, 0.7f, 0.3f);
	//pLight2.range = 5.0f;
	//pLight2.intensity = 5.0f;

	//uint32 pLight2ID = instance.InstantiatePointLight(pLight2);

	//{
	//	Mat4f modelMat = API_ConvertCoordinateSystem(TransffPRS(pLight1.position, Quatf::Identity, Vec3f::One * 0.5f).Matrix());
	//	gizmoMat->InitVariable(instance, &modelMat, sizeof(Mat4f));
	//}


	//const float r = 1.0f;
	//const float l = -1.0f;
	//const float t = 1.0f;
	//const float b = -1.0f;
	//const float n = 0.35f;
	//const float f = 10.0f;

	//Mat4f orthoMat = API_ConvertCoordinateSystem(Mat4f
	//(
	//	2 / (r - l), 0, 0, 0,
	//	0, 2 / (t - b), 0, 0,
	//	0, 0, -2 / (f - n), 0,
	//	-(r + l) / (r - l), -(t + b) / (t - b), -(f + n) / (f - n), 1
	//));

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

		mainCamera.SetTransform(camTr);

		if(pLight1ID != uint32(-1))
		{
			if (gizmoMat)
			{
				Mat4f modelMat = API_ConvertCoordinateSystem(TransffPRS(pL1Pos, Quatf::Identity, Vec3f::One * 0.5f).Matrix());
				gizmoMat->UpdateVariable(instance, frame, &modelMat, sizeof(Mat4f));
			}

			vkDeviceWaitIdle(instance.GetDevice());
			pLight1.position = API_ConvertCoordinateSystem(pL1Pos);
			instance.mPointLightBuffer.UpdateObject(instance.GetDevice(), pLight1ID, pLight1);
		}


		// Draw Magikarp.
		if (magikarpBodyMat)
		{
			magikarpBodyMat->Bind(frame);
			magikarpBodyMesh->Draw(frame);

			magikarpEyesMat->Bind(frame);
			magikarpEyesMesh->Draw(frame);
		}


		// Draw bricks.
		if (bricksMat)
		{
			bricksMat->Bind(frame);
			cubeMesh->Draw(frame);
		}

		// Draw spheres.
		if (sphereMats)
		{
			for (uint32 i = 0; i < sphereMatNum; ++i)
			{
				sphereMats[i]->Bind(frame);
				sphereMesh->Draw(frame);
			}
		}


		// Draw skybox.
		if (skyboxMat)
		{
			skyboxMat->Bind(frame);
			cubeMesh->Draw(frame);
		}


		// Draw gizmos.
		if (gizmoMat)
		{
			gizmoMat->Bind(frame);
			squareMesh->Draw(frame);
		}

		
		// Draw window.
		if (windowMat)
		{
			windowMat->Bind(frame);
			squareMesh->Draw(frame);
		}

		surface.GetSwapChain().End(instance.GetDevice());
	}



	// === Destroy ===
	vkDeviceWaitIdle(instance.GetDevice());

	assetMgr.Clear();

	//mainCamera.Destroy(instance);

	instance.DestroyRenderSurface(window);
	
	window.Destroy();

	instance.Destroy();


	// === UnInit ===
	IWindow::UnInit();
	IRenderInstance::UnInit();


	LOG("\n=== End ===");

	return 0;
}
