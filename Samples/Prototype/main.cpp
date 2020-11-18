// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <string>
#include <iostream>

#include <Core/Time/Chrono.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>
#include <Rendering/Vulkan/System/VkRenderPass.hpp>
#include <Rendering/Vulkan/System/Surface/VkRenderSurface.hpp>

#include <Rendering/Vulkan/Primitives/Mesh/VkMesh.hpp>
#include <Rendering/Vulkan/Primitives/Shader/VkShader.hpp>
#include <Rendering/Vulkan/Primitives/Texture/VkTexture.hpp>
#include <Rendering/Vulkan/Primitives/Texture/VkCubemap.hpp>
#include <Rendering/Vulkan/Primitives/Pipeline/VkPipeline.hpp>
#include <Rendering/Vulkan/Primitives/Material/VkMaterial.hpp>
#include <Rendering/Vulkan/Buffers/VkBuffer.hpp>

#include <SDK/Assets/Shader/ShaderAsset.hpp>
#include <SDK/Assets/Texture/TextureAsset.hpp>
#include <SDK/Assets/Texture/CubemapAsset.hpp>

#include <Window/GLFW/System/GLFWWindow.hpp>
using namespace Sa;

#define LOG(_str) std::cout << _str << std::endl;


struct MainRenderInfos
{
	Vk::RenderPass renderPass;
	RenderPassDescriptor renderPassDesc;

	// Subpass 0: G-Buffer composition.
	Vk::Shader litCompVert;
	Vk::Shader litCompFrag;
	Vk::Pipeline mainPipeline;

	// Subpass 1: Illumination.
	Vk::Shader litVert;
	Vk::Shader litFrag;
	Vk::Pipeline litPipeline;
	Vk::Material litmaterial;

	TransffPRS camTr;
	Vk::Buffer camUBO;

	struct camUBOData
	{
		Mat4f proj = Mat4f::Identity;
		Mat4f viewInv = Mat4f::Identity;
		Vec3f viewPosition;
	};

	camUBOData camUBOd;

	void Create(IRenderInstance& _instance, IRenderSurface& _surface)
	{
		// RenderPass.
		renderPassDesc = RenderPassDescriptor::DefaultPBR(&_surface);
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
			{
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

				pipelineInfos.subPassIndex = 0u;
				pipelineInfos.shaders.push_back(PipelineShaderInfos{ &litCompVert, ShaderStage::Vertex });
				pipelineInfos.shaders.push_back(PipelineShaderInfos{ &litCompFrag, ShaderStage::Fragment });

				pipelineInfos.vertexBindingLayout.meshLayout = VertexLayout::Make<VertexComp::Default>();
				pipelineInfos.vertexBindingLayout.desiredLayout = VertexLayout::Make<VertexComp::Default>();

				mainPipeline.Create(_instance, pipelineInfos);
			}
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
			{
				PipelineCreateInfos pipelineInfos(renderPass, renderPassDesc);

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

				pipelineInfos.subPassIndex = 1u;
				pipelineInfos.shaders.push_back(PipelineShaderInfos{ &litVert, ShaderStage::Vertex });
				pipelineInfos.shaders.push_back(PipelineShaderInfos{ &litFrag, ShaderStage::Fragment });

				pipelineInfos.vertexBindingLayout.meshLayout = VertexLayout::Make<VertexComp::Default>();
				pipelineInfos.vertexBindingLayout.desiredLayout = VertexLayout::Make<VertexComp::Default>();

				litPipeline.Create(_instance, pipelineInfos);
			}


			// Material.
			{
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
		}


		// Create cam UBO
		camTr.position = Vec3f(0.0f, 0.0f, 10.0f);
		camUBOd.proj = Mat4f::MakePerspective(90.0f, 1200.0f / 800.0f);

		camUBO.Create(_instance.As<Vk::RenderInstance>().device, sizeof(camUBOData),
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &camUBOd);
	}
	
	void Update(const Vk::RenderInstance& _instance)
	{
		camUBOd.viewInv = API_ConvertCoordinateSystem(camTr.Matrix()).GetInversed();
		camUBOd.viewPosition = API_ConvertCoordinateSystem(camTr.position);
		camUBO.UpdateData(_instance.device, &camUBOd, sizeof(camUBOd));
	}

	void Destroy(IRenderInstance& _instance, IRenderSurface& _surface)
	{
		litmaterial.Destroy(_instance);

		litPipeline.Destroy(_instance);
		litVert.Destroy(_instance);
		litFrag.Destroy(_instance);

		mainPipeline.Destroy(_instance);
		litCompVert.Destroy(_instance);
		litCompFrag.Destroy(_instance);

		camUBO.Destroy(_instance.As<Vk::RenderInstance>().device);

		_surface.DestroyFrameBuffers(_instance);
		renderPass.Destroy(_instance);
	}
};

MainRenderInfos mainRender;

struct CubeRender
{
	Vk::Material material;


	Vk::Buffer modelUBO;
	Vk::Texture texture;

	struct modelUBOData
	{
		Mat4f modelMat = Mat4f::Identity;

		float uvTilling = 1.0f;
		float uvOffset = 0.0f;
	};

	modelUBOData modelUBOd;

	void Create(Vk::RenderInstance& _instance)
	{
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


		modelUBOd.modelMat = API_ConvertCoordinateSystem(Mat4f::MakeTransform(Vec3f(0.0f, 0.0f, 0.0f), Quatf::Identity, Vec3f::One * 5.0f));
		modelUBO.Create(_instance.device, sizeof(modelUBOData),
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &modelUBOd);


		MaterialCreateInfos matCreateInfos(mainRender.mainPipeline);

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
		material.Destroy(_instance);

		modelUBO.Destroy(_instance.device);

		texture.Destroy(_instance);
	}
};

CubeRender cubeRender;


struct SkyboxRender
{
	Vk::Cubemap cubemap;
	Vk::Shader skyboxVert;
	Vk::Shader skyboxFrag;
	Vk::Pipeline pipeline;
	Vk::Material material;

	void Create(Vk::RenderInstance& _instance)
	{
		// HDR Texture Asset.
		{
			const char* assetPath = "Bin/Skybox/Milkyway.spha";
			const char* resourcePath = "../../Samples/Prototype/Resources/Skybox/Milkyway.hdr";

			CubemapAsset asset;
			uint32 res = asset.TryLoadImport(assetPath, resourcePath, CubemapImportInfos(_instance));

			//if (res != 1)
			//	asset.Save(assetPath);
			//else if (res == -1)
			//	SA_ASSERT(false, InvalidParam, SDK, L"Import failed");

			cubemap.Create(_instance, asset.GetRawData());
		}

		// Vertex Shader.
		{
			const char* assetPath = "Bin/Engine/Shaders/skybox_VS.spha";
			const char* resourcePath = "../../Engine/Resources/Shaders/skybox.vert";

			ShaderAsset asset;
			uint32 res = asset.TryLoadImport(assetPath, resourcePath, ShaderImportInfos());

			if (res != 1)
				asset.Save(assetPath);
			else if (res == -1)
				SA_ASSERT(false, InvalidParam, SDK, L"Import failed");

			skyboxVert.Create(_instance, asset.GetRawData());
		}

		// Fragment Shader.
		{
			const char* assetPath = "Bin/Engine/Shaders/skybox_FS.spha";
			const char* resourcePath = "../../Engine/Resources/Shaders/skybox.frag";

			ShaderAsset asset;
			uint32 res = asset.TryLoadImport(assetPath, resourcePath, ShaderImportInfos());

			if (res != 1)
				asset.Save(assetPath);
			else if (res == -1)
				SA_ASSERT(false, InvalidParam, SDK, L"Import failed");

			skyboxFrag.Create(_instance, asset.GetRawData());
		}

		// Pipeline.
		{
			PipelineCreateInfos pipelineInfos(mainRender.renderPass, mainRender.renderPassDesc);

			{
				PipelineBindingInfos& inPositionBinding = pipelineInfos.bindings.emplace_back();
				inPositionBinding.binding = 0u;
				inPositionBinding.stages = ShaderStage::Vertex;
				inPositionBinding.type = ShaderBindingType::UniformBuffer;


				PipelineBindingInfos& inNormalBinding = pipelineInfos.bindings.emplace_back();
				inNormalBinding.binding = 1u;
				inNormalBinding.stages = ShaderStage::Fragment;
				inNormalBinding.type = ShaderBindingType::ImageSamplerCube;
			}

			pipelineInfos.subPassIndex = 1u;
			pipelineInfos.shaders.push_back(PipelineShaderInfos{ &skyboxVert, ShaderStage::Vertex });
			pipelineInfos.shaders.push_back(PipelineShaderInfos{ &skyboxFrag, ShaderStage::Fragment });

			pipelineInfos.vertexBindingLayout.meshLayout = VertexLayout::Make<VertexComp::Default>();
			pipelineInfos.vertexBindingLayout.desiredLayout = VertexLayout::Make<VertexComp::Position>();

			pipeline.Create(_instance, pipelineInfos);
		}

		// Material.
		{
			MaterialCreateInfos matCreateInfos(pipeline);

			MaterialBindingInfos& camUBOBinding = matCreateInfos.bindings.emplace_back();
			camUBOBinding.binding = 0;
			camUBOBinding.SetUniformBuffers({ &mainRender.camUBO });

			MaterialBindingInfos& skyboxCubeBinding = matCreateInfos.bindings.emplace_back();
			skyboxCubeBinding.binding = 1;
			skyboxCubeBinding.SetImageSamplerCubes({ &cubemap });

			material.Create(_instance, matCreateInfos);
		}
	}

	void Destroy(const Vk::RenderInstance& _instance)
	{
		material.Destroy(_instance);

		cubemap.Destroy(_instance);
	}
};

SkyboxRender skyboxRender;

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
	skyboxRender.Create(instance);


	Vk::Mesh cubeMesh;
	cubeMesh.Create(instance, RawMesh::Cube());

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
		mainRender.mainPipeline.Bind(frame);
		cubeRender.material.Bind(frame, mainRender.mainPipeline);
		cubeMesh.Draw(frame);


		// Subpass 1.
		frame.buffer.NextSubpass();


		// Illumination.
		mainRender.litPipeline.Bind(frame);
		mainRender.litmaterial.Bind(frame, mainRender.litPipeline);
		cubeMesh.Draw(frame);

		// Skybox.
		skyboxRender.pipeline.Bind(frame);
		skyboxRender.material.Bind(frame, skyboxRender.pipeline);
		cubeMesh.Draw(frame);


		// End Surface.
		surface.End(instance);
	}

	
	LOG("=== Destroy ===");
	vkDeviceWaitIdle(instance.device);


	cubeRender.Destroy(instance);
	cubeMesh.Destroy(instance);

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
