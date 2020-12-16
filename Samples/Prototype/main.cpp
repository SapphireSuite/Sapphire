// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <string>
#include <iostream>

#include <Collections/Thread>
#include <Core/Time/Chrono.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>
#include <Rendering/Vulkan/System/VkRenderPass.hpp>
#include <Rendering/Vulkan/System/Surface/VkRenderSurface.hpp>

#include <Rendering/Vulkan/Primitives/Mesh/VkMesh.hpp>
#include <Rendering/Vulkan/Primitives/Shader/VkShader.hpp>
#include <Rendering/Vulkan/Primitives/Texture/VkTexture.hpp>
#include <Rendering/Vulkan/Primitives/Pipeline/VkPipeline.hpp>
#include <Rendering/Vulkan/Primitives/Material/VkMaterial.hpp>
#include <Rendering/Vulkan/Buffers/VkBuffer.hpp>

#include <SDK/Assets/Shader/ShaderAsset.hpp>
#include <SDK/Assets/Texture/TextureAsset.hpp>
#include <SDK/Assets/Mesh/MeshAsset.hpp>

#include <Window/GLFW/System/GLFWWindow.hpp>
using namespace Sa;

#define LOG(_str) std::cout << _str << std::endl;


struct modelUBOData
{
	Mat4f modelMat = Mat4f::Identity;

	float uvTilling = 1.0f;
	float uvOffset = 0.0f;
};

struct MainRenderInfos
{
	Vk::RenderPass renderPass;

	// Subpass 0: G-Buffer composition.
	Vk::Shader litCompVert;
	Vk::Shader litCompFrag;
	Vk::Pipeline litCompPipeline;

	// Subpass 1: Illumination.
	Vk::Shader litVert;
	Vk::Shader litFrag;
	Vk::Pipeline litPipeline;
	Vk::Material litmaterial;


	// Subpass 1: unlit.
	Vk::Shader unlitVert;
	Vk::Shader unlitFrag;
	Vk::Pipeline unlitPipeline;

	// Camera.
	TransffPRS camTr;
	Vk::Buffer camUBO;

	struct camUBOData
	{
		Mat4f proj = Mat4f::Identity;
		Mat4f viewInv = Mat4f::Identity;
		Vec3f viewPosition;
	};

	camUBOData camUBOd;


	void Create(Vk::RenderInstance& _instance, IRenderSurface& _surface)
	{
		// RenderPass.
		const RenderPassDescriptor renderPassDesc = RenderPassDescriptor::DefaultPBR(&_surface);
		renderPass.Create(_instance, renderPassDesc);
		
		uint32 frameBuffersSize = 0u;
		const std::vector<IFrameBuffer*>& frameBuffers = _surface.CreateFrameBuffers(_instance, renderPass, renderPassDesc, &frameBuffersSize);

		// Subpass 0: G-Buffer composition.
		{
			// Vertex Shader.
			{
				const char* assetPath = "Bin/Engine/Shaders/lit_composition_VS.spha";
				const char* resourcePath = "../../Engine/Resources/Shaders/lit_composition.vert";

				ShaderAsset asset;
				uint32 res = asset.TryLoadImport(assetPath, resourcePath, ShaderImportInfos());

				if (res != 1)
					asset.Save(assetPath);
				else if (res == -1)
					SA_ASSERT(false, InvalidParam, SDK, L"Import failed");

				litCompVert.Create(_instance, asset.GetRawData());
			}

			// Fragment Shader.
			{
				const char* assetPath = "Bin/Engine/Shaders/lit_composition_FS.spha";
				const char* resourcePath = "../../Engine/Resources/Shaders/lit_composition.frag";

				ShaderAsset asset;
				uint32 res = asset.TryLoadImport(assetPath, resourcePath, ShaderImportInfos());

				if (res != 1)
					asset.Save(assetPath);
				else if (res == -1)
					SA_ASSERT(false, InvalidParam, SDK, L"Import failed");

				litCompFrag.Create(_instance, asset.GetRawData());
			}


			// Pipeline.
			PipelineCreateInfos pipelineInfos(renderPass, renderPassDesc);

			{
				PipelineBindingInfos& camUBOBinding = pipelineInfos.bindings.emplace_back();
				camUBOBinding.binding = 0u;
				camUBOBinding.stages = ShaderStage::Vertex;
				camUBOBinding.type = ShaderBindingType::UniformBuffer;


				PipelineBindingInfos& modelUBOBinding = pipelineInfos.bindings.emplace_back();
				modelUBOBinding.binding = 1u;
				modelUBOBinding.stages = ShaderStage::Vertex;
				modelUBOBinding.type = ShaderBindingType::UniformBuffer;


				PipelineBindingInfos& textureBinding = pipelineInfos.bindings.emplace_back();
				textureBinding.binding = 2u;
				textureBinding.num = 6u;
				textureBinding.stages = ShaderStage::Fragment;
				textureBinding.type = ShaderBindingType::ImageSampler2D;
			}

			pipelineInfos.subPassIndex = 0u;
			pipelineInfos.shaders.push_back(PipelineShaderInfos{ &litCompVert, ShaderStage::Vertex });
			pipelineInfos.shaders.push_back(PipelineShaderInfos{ &litCompFrag, ShaderStage::Fragment });

			pipelineInfos.vertexBindingLayout.meshLayout = VertexLayout::Make<VertexComp::Default>();

			litCompPipeline.Create(_instance, pipelineInfos);
		}

		// Subpass 1: Illumination.
		{
			// Vertex Shader.
			{
				const char* assetPath = "Bin/Engine/Shaders/lit_VS.spha";
				const char* resourcePath = "../../Engine/Resources/Shaders/lit.vert";

				ShaderAsset asset;
				uint32 res = asset.TryLoadImport(assetPath, resourcePath, ShaderImportInfos());

				if (res != 1)
					asset.Save(assetPath);
				else if (res == -1)
					SA_ASSERT(false, InvalidParam, SDK, L"Import failed");

				litVert.Create(_instance, asset.GetRawData());
			}

			// Fragment Shader.
			{
				const char* assetPath = "Bin/Engine/Shaders/lit_FS.spha";
				const char* resourcePath = "../../Engine/Resources/Shaders/lit.frag";

				ShaderAsset asset;
				uint32 res = asset.TryLoadImport(assetPath, resourcePath, ShaderImportInfos());

				if (res != 1)
					asset.Save(assetPath);
				else if (res == -1)
					SA_ASSERT(false, InvalidParam, SDK, L"Import failed");

				litFrag.Create(_instance, asset.GetRawData());
			}


			// Pipeline.
			PipelineCreateInfos pipelineInfos(renderPass, renderPassDesc);

			{
				PipelineBindingInfos& inPositionBinding = pipelineInfos.bindings.emplace_back();
				inPositionBinding.binding = 0u;
				inPositionBinding.stages = ShaderStage::Fragment;
				inPositionBinding.type = ShaderBindingType::InputAttachment;


				PipelineBindingInfos& inNormalBinding = pipelineInfos.bindings.emplace_back();
				inNormalBinding.binding = 1u;
				inNormalBinding.stages = ShaderStage::Fragment;
				inNormalBinding.type = ShaderBindingType::InputAttachment;


				PipelineBindingInfos& inAlbedoBinding = pipelineInfos.bindings.emplace_back();
				inAlbedoBinding.binding = 2u;
				inAlbedoBinding.stages = ShaderStage::Fragment;
				inAlbedoBinding.type = ShaderBindingType::InputAttachment;

				PipelineBindingInfos& inPBRBinding = pipelineInfos.bindings.emplace_back();
				inPBRBinding.binding = 3u;
				inPBRBinding.stages = ShaderStage::Fragment;
				inPBRBinding.type = ShaderBindingType::InputAttachment;
			}

			pipelineInfos.subPassIndex = 1u;
			pipelineInfos.shaders.push_back(PipelineShaderInfos{ &litVert, ShaderStage::Vertex });
			pipelineInfos.shaders.push_back(PipelineShaderInfos{ &litFrag, ShaderStage::Fragment });

			pipelineInfos.vertexBindingLayout.meshLayout = VertexLayout::Make<VertexComp::None>();

			litPipeline.Create(_instance, pipelineInfos);


			// Material.
			MaterialCreateInfos matCreateInfos(litPipeline, frameBuffersSize);

			for (uint32 i = 0; i < 3u; ++i)
			{
				for (uint32 j = 0; j < 4u; ++j)
				{
					MaterialBindingInfos& inBinding = matCreateInfos.bindings.emplace_back();
					inBinding.binding = j;
					inBinding.descriptor = i;

					inBinding.SetInputBuffer(frameBuffers[i]->GetInputAttachment(j));
				}
			}

			litmaterial.Create(_instance, matCreateInfos);
		}

		// Subpass 1: Unlit.
		{
			// Vertex Shader.
			{
				const char* assetPath = "Bin/Engine/Shaders/unlit_VS.spha";
				const char* resourcePath = "../../Engine/Resources/Shaders/unlit.vert";

				ShaderAsset asset;
				uint32 res = asset.TryLoadImport(assetPath, resourcePath, ShaderImportInfos());

				if (res != 1)
					asset.Save(assetPath);
				else if (res == -1)
					SA_ASSERT(false, InvalidParam, SDK, L"Import failed");

				unlitVert.Create(_instance, asset.GetRawData());
			}

			// Fragment Shader.
			{
				const char* assetPath = "Bin/Engine/Shaders/unlit_FS.spha";
				const char* resourcePath = "../../Engine/Resources/Shaders/unlit.frag";

				ShaderAsset asset;
				uint32 res = asset.TryLoadImport(assetPath, resourcePath, ShaderImportInfos());

				if (res != 1)
					asset.Save(assetPath);
				else if (res == -1)
					SA_ASSERT(false, InvalidParam, SDK, L"Import failed");

				unlitFrag.Create(_instance, asset.GetRawData());
			}


			// Pipeline.
			PipelineCreateInfos pipelineInfos(renderPass, renderPassDesc);

			{
				PipelineBindingInfos& camUBOBinding = pipelineInfos.bindings.emplace_back();
				camUBOBinding.binding = 0u;
				camUBOBinding.stages = ShaderStage::Vertex;
				camUBOBinding.type = ShaderBindingType::UniformBuffer;


				PipelineBindingInfos& modelUBOBinding = pipelineInfos.bindings.emplace_back();
				modelUBOBinding.binding = 1u;
				modelUBOBinding.stages = ShaderStage::Vertex;
				modelUBOBinding.type = ShaderBindingType::UniformBuffer;


				PipelineBindingInfos& textureBinding = pipelineInfos.bindings.emplace_back();
				textureBinding.binding = 2u;
				textureBinding.stages = ShaderStage::Fragment;
				textureBinding.type = ShaderBindingType::ImageSampler2D;
			}

			pipelineInfos.subPassIndex = 1u;
			pipelineInfos.shaders.push_back(PipelineShaderInfos{ &unlitVert, ShaderStage::Vertex });
			pipelineInfos.shaders.push_back(PipelineShaderInfos{ &unlitFrag, ShaderStage::Fragment });

			pipelineInfos.vertexBindingLayout.meshLayout = VertexLayout::Make<VertexComp::Default>();
			pipelineInfos.vertexBindingLayout.desiredLayout = VertexLayout::Make<VertexComp::PTex>();

			unlitPipeline.Create(_instance, pipelineInfos);
		}

		// Camera
		{
			camTr.position = Vec3f(0.0f, 0.0f, 10.0f);
			camUBOd.proj = Mat4f::MakePerspective(90.0f, 1200.0f / 800.0f);

			camUBO.Create(_instance.device, sizeof(camUBOData),
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &camUBOd);
		}
	}
	
	void Destroy(Vk::RenderInstance& _instance, IRenderSurface& _surface)
	{
		litmaterial.Destroy(_instance);

		litPipeline.Destroy(_instance);
		litVert.Destroy(_instance);
		litFrag.Destroy(_instance);

		litCompPipeline.Destroy(_instance);
		litCompVert.Destroy(_instance);
		litCompFrag.Destroy(_instance);

		unlitPipeline.Destroy(_instance);
		unlitVert.Destroy(_instance);
		unlitFrag.Destroy(_instance);

		_surface.DestroyFrameBuffers(_instance);
		renderPass.Destroy(_instance);

		camUBO.Destroy(_instance.device);
	}

	void Update(const Vk::RenderInstance& _instance)
	{
		camUBOd.viewInv = API_ConvertCoordinateSystem(camTr.Matrix()).GetInversed();
		camUBOd.viewPosition = API_ConvertCoordinateSystem(camTr.position);
		camUBO.UpdateData(_instance.device, &camUBOd, sizeof(camUBOd));
	}
};

MainRenderInfos mainRender;

struct CubeRender
{
	Vk::Mesh cubeMesh;

	Vk::Material material;

	Vk::Buffer modelUBO;
	Vk::Texture texture;

	void Create(Vk::RenderInstance& _instance)
	{
		cubeMesh.Create(_instance, RawMesh::Cube());

		// Texture Asset.
		{
			const char* assetPath = "Bin/Engine/Textures/missText.spha";
			const char* resourcePath = "../../Engine/Resources/Textures/missing_texture.png";

			TextureAsset asset;
			TextureImportInfos importInfos; importInfos.format = Format::sRGBA_32;
			uint32 res = asset.TryLoadImport(assetPath, resourcePath, importInfos);

			if (res != 1)
				asset.Save(assetPath);
			else if (res == -1)
				SA_ASSERT(false, InvalidParam, SDK, L"Import failed");

			texture.Create(_instance, asset.GetRawData());
		}

		modelUBOData modelUBOd;
		modelUBOd.modelMat = API_ConvertCoordinateSystem(Mat4f::MakeTransform(Vec3f(10.0f, 0.0f, 5.0f), Quatf::Identity, Vec3f::One * 5.0f));
		modelUBO.Create(_instance.device, sizeof(modelUBOData),
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &modelUBOd);


		MaterialCreateInfos matCreateInfos(mainRender.unlitPipeline);

		MaterialBindingInfos& camBinding = matCreateInfos.bindings.emplace_back();
		camBinding.binding = 0u;
		camBinding.SetUniformBuffers({ &mainRender.camUBO });

		MaterialBindingInfos& modelBinding = matCreateInfos.bindings.emplace_back();
		modelBinding.binding = 1u;
		modelBinding.SetUniformBuffers({ &modelUBO });

		MaterialBindingInfos& textureBinding = matCreateInfos.bindings.emplace_back();
		textureBinding.binding = 2u;
		textureBinding.SetImageSamplers2D({ &texture });

		material.Create(_instance, matCreateInfos);
	}

	void Destroy(const Vk::RenderInstance& _instance)
	{
		cubeMesh.Destroy(_instance);
		
		material.Destroy(_instance);

		modelUBO.Destroy(_instance.device);

		texture.Destroy(_instance);
	}

	void Draw(const RenderFrame& _frame)
	{
		material.Bind(_frame, mainRender.unlitPipeline);
		cubeMesh.Draw(_frame);
	}
};

CubeRender cubeRender;


struct SphereRender
{
	static constexpr uint32 sphereNum = 14u;

	Vk::Mesh sphereMesh;

	Vk::Material materials[sphereNum];

	Vk::Buffer modelUBOs[sphereNum];
	Vk::Texture textures[sphereNum * 6u];

	void Create(Vk::RenderInstance& _instance)
	{
		// Mesh asset.
		{
			const char* assetPath = "Bin/Spheres/sphere_M.spha";
			const char* resourcePath = "../../Samples/Prototype/Resources/Spheres/sphere.obj";

			MeshAsset asset;
			MeshImportInfos importInfos; importInfos.index = 0u;
			uint32 res = asset.TryLoadImport(assetPath, resourcePath, importInfos);

			if (res != 1)
				asset.Save(assetPath);
			else if (res == -1)
				SA_ASSERT(false, InvalidParam, SDK, L"Import failed");

			sphereMesh.Create(_instance, asset.GetRawData());
		}

		// Texture asset.
		{
			const std::vector<std::string> names =
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
			const std::vector<std::vector<std::string>> matResourcePaths =
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
					"lightgold_roughness",
					""
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
					"rustediron-streaks_roughness",
					""
				},
				{
					"rustediron2_basecolor",
					"rustediron2_normal",
					"",
					"rustediron2_metallic",
					"rustediron2_roughness",
					""
				},
				{
					"gold-scuffed_basecolor-boosted",
					"gold-scuffed_normal",
					"",
					"gold-scuffed_metallic",
					"gold-scuffed_roughness",
					""
				},
				{
					"Titanium-Scuffed_basecolor",
"Titanium-Scuffed_normal",
"",
"Titanium-Scuffed_metallic",
"Titanium-Scuffed_roughness",
""
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
			textureImpIfos[0].format = Format::sRGBA_32; // albedo.
			textureImpIfos[2].format = Format::R_8; // metallness.
			textureImpIfos[4].format = Format::R_8; // roughness.
			textureImpIfos[5].format = Format::R_8; // ambiant occlusion.


			// Async texture loading / import.
			//Thread jobs[sphereNum];

			for (uint32 i = 0u; i < sphereNum; ++i)
			{
				//jobs[i] = [this, i, &_instance, &matResourcePaths, &names, &textureImpIfos]()
				{
					// Textures.
					{
						std::string sphaPath;
						std::string resPath;

						for (uint32 j = 0u; j < 6u; ++j)
						{
							if (matResourcePaths[i][j].empty())
							{
								sphaPath = "Bin/Spheres/" + names[i] + "/missing" + std::to_string(j) + "_T" + ".spha";
								resPath = "../../Engine/Resources/Textures/missing_texture.png";
							}
							else
							{
								sphaPath = "Bin/Spheres/" + names[i] + '/' + matResourcePaths[i][j] + ".spha";
								resPath = "../../Samples/Prototype/Resources/Spheres/" + names[i] + '/' + matResourcePaths[i][j] + ".png";
							}


							TextureAsset asset;
							uint32 res = asset.TryLoadImport(sphaPath, resPath, textureImpIfos[j]);

							if (res != 1)
								asset.Save(sphaPath);
							else if (res == -1)
								SA_ASSERT(false, InvalidParam, SDK, L"Import failed");

							textures[i * 6u + j].Create(_instance, asset.GetRawData());
						}
					}
				};
			}

			//for (uint32 i = 0u; i < sphereNum; ++i)
			//	jobs[i].Join();
		}

		// Material Asset.
		{
			const uint32 maxY = 3u;
			Atomic<uint32> currX = 0u;
			Atomic<uint32> currY = 0u;

			for (uint32 i = 0u; i < sphereNum; ++i)
			{
				//jobs[i] = [this, &_instance, i]()
				{
					modelUBOData modelUBOd;
					modelUBOd.modelMat = API_ConvertCoordinateSystem(Mat4f::MakeTransform(Vec3f(-10.0f + currX * 7.5f, -2.0f + currY * 7.5f, -5.0f),
						Quatf::Identity, Vec3f::One * 0.1f));

					currY = (currY + 1) % maxY;

					if (currY == 0)
						++currX;


					modelUBOs[i].Create(_instance.device, sizeof(modelUBOData),
						VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &modelUBOd);

					MaterialCreateInfos matCreateInfos(mainRender.litCompPipeline);

					MaterialBindingInfos& camBinding = matCreateInfos.bindings.emplace_back();
					camBinding.binding = 0u;
					camBinding.SetUniformBuffers({ &mainRender.camUBO });

					MaterialBindingInfos& modelBinding = matCreateInfos.bindings.emplace_back();
					modelBinding.binding = 1u;
					modelBinding.SetUniformBuffers({ &modelUBOs[i] });

					MaterialBindingInfos& textureBinding = matCreateInfos.bindings.emplace_back();
					textureBinding.binding = 2u;
					textureBinding.SetImageSamplers2D({ &textures[i * 6],
						&textures[i * 6 + 1],
						&textures[i * 6 + 2],
						&textures[i * 6 + 3],
						&textures[i * 6 + 4],
						&textures[i * 6 + 5]
						});

					materials[i].Create(_instance, matCreateInfos);
				};
			}

			//for (uint32 i = 0u; i < sphereNum; ++i)
			//	jobs[i].Join();
		}
	}

	void Destroy(const Vk::RenderInstance& _instance)
	{
		sphereMesh.Destroy(_instance);

		for (uint32 i = 0u; i < sphereNum; ++i)
		{
			materials[i].Destroy(_instance);

			modelUBOs[i].Destroy(_instance.device);

			for (uint32 j = 0u; j < 6u; ++j)
				textures[i * 6 + j].Destroy(_instance);
		}
	}

	void Draw(const RenderFrame& _frame)
	{
		for (uint32 i = 0u; i < sphereNum; ++i)
		{
			materials[i].Bind(_frame, mainRender.litCompPipeline);
			sphereMesh.Draw(_frame);
		}
	}
};

SphereRender sphereRender;


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


	mainRender.Create(instance, surface);

	cubeRender.Create(instance);
	sphereRender.Create(instance);


	Vec3f lightPos;

	Chrono chrono;
	float speed = 0.1f;


	LOG("=== Loop ===");

#ifndef __SA_TRAVIS
	while (!window.ShouldClose())
#endif
	{
		float deltaTime = chrono.Restart() * 0.00005f;


		window.Update();

		window.TEST(mainRender.camTr, lightPos, deltaTime * speed);
		mainRender.Update(instance);


		// Begin Surface.
		RenderFrame frame = surface.Begin(instance);


		// Subpass 0: G-Buffer composition.
		mainRender.litCompPipeline.Bind(frame);

		sphereRender.Draw(frame);
		//

		frame.buffer.NextSubpass();

		// Subpass 1: Illumination.
		mainRender.litPipeline.Bind(frame);

		mainRender.litmaterial.Bind(frame, mainRender.litPipeline);
		vkCmdDraw(frame.buffer.As<Vk::FrameBuffer>().commandBuffer.Get(), 4u, 1u, 0u, 0u);


		mainRender.unlitPipeline.Bind(frame);
		cubeRender.Draw(frame);

		// End Surface.
		surface.End(instance);
	}

	
	LOG("=== Destroy ===");
	vkDeviceWaitIdle(instance.device);


	cubeRender.Destroy(instance);
	sphereRender.Destroy(instance);

	mainRender.Destroy(instance, surface);


	instance.DestroyRenderSurface(surface);

	window.Destroy();

	instance.Destroy();


	LOG("=== UnInit ===");
	IWindow::UnInit();
	IRenderInstance::UnInit();


	LOG("\n=== End ===");

	return 0;
}
