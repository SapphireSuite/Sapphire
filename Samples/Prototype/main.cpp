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
#include <Rendering/Vulkan/Primitives/Pipeline/VkPipeline.hpp>
#include <Rendering/Vulkan/Primitives/Material/VkMaterial.hpp>
#include <Rendering/Vulkan/Buffers/VkBuffer.hpp>
#include <Rendering/Vulkan/Buffers/VkFrameBuffer.hpp>

#include <SDK/Assets/Shader/ShaderAsset.hpp>
#include <SDK/Assets/Texture/TextureAsset.hpp>

#include <Window/GLFW/System/GLFWWindow.hpp>
using namespace Sa;

#define LOG(_str) std::cout << _str << std::endl;


#define __DEFERRED 1

#if __DEFERRED

struct MainRenderInfos
{
	Vk::RenderPass renderPass;

	// Subpass 0: G-Buffer composition.
	Vk::Shader litCompVert;
	Vk::Shader litCompFrag;
	Vk::Pipeline mainPipeline;

	// Subpass 1: Illumination.
	Vk::Shader litVert;
	Vk::Shader litFrag;
	Vk::Pipeline litPipeline;
	Vk::Material litmaterial;


	void Create(IRenderInstance& _instance, IRenderSurface& _surface)
	{
		// RenderPass.
		const RenderPassDescriptor renderPassDesc = RenderPassDescriptor::CreateDefaultPBRDeferred(&_surface);
		renderPass.Create(_instance, renderPassDesc);
		const std::vector<Vk::FrameBuffer>& frameBuffers =
			reinterpret_cast<const std::vector<Vk::FrameBuffer>&>(_surface.CreateFrameBuffers(_instance, renderPass, renderPassDesc));

		// Subpass 0: G-Buffer composition.
		{
			// Vertex Shader.
			{
				const char* assetPath = "Bin/Engine/Shaders/Deferred/lit_composition_VS.spha";
				const char* resourcePath = "../../Engine/Resources/Shaders/Deferred/lit_composition.vert";

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
				const char* assetPath = "Bin/Engine/Shaders/Deferred/lit_composition_FS.spha";
				const char* resourcePath = "../../Engine/Resources/Shaders/Deferred/lit_composition.frag";

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

		// Subpass 1: Illumination.
		{
			// Vertex Shader.
			{
				const char* assetPath = "Bin/Engine/Shaders/Deferred/lit_VS.spha";
				const char* resourcePath = "../../Engine/Resources/Shaders/Deferred/lit.vert";

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
				const char* assetPath = "Bin/Engine/Shaders/Deferred/lit_FS.spha";
				const char* resourcePath = "../../Engine/Resources/Shaders/Deferred/lit.frag";

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

			pipelineInfos.vertexBindingLayout.meshLayout = VertexLayout::Make<VertexComp::Default>();
			pipelineInfos.vertexBindingLayout.desiredLayout = VertexLayout::Make<VertexComp::Default>();

			litPipeline.Create(_instance, pipelineInfos);


			// Material.
			MaterialCreateInfos matCreateInfos(litPipeline);

			for (uint32 i = 0; i < 4u; ++i)
			{
				MaterialBindingInfos& inBinding = matCreateInfos.bindings.emplace_back();
				inBinding.binding = i;

				inBinding.mType = ShaderBindingType::InputAttachment;
			}

			litmaterial.Create(_instance, matCreateInfos);


			// TEMP.
			VkSampler sampler = VK_NULL_HANDLE;

			VkSamplerCreateInfo samplerCreateInfo{};
			samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			samplerCreateInfo.pNext = nullptr;
			samplerCreateInfo.flags = 0;
			samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
			samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
			samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerCreateInfo.mipLodBias = 0.0f;
			samplerCreateInfo.anisotropyEnable = VK_TRUE;
			samplerCreateInfo.maxAnisotropy = 16.0f;
			samplerCreateInfo.compareEnable = VK_FALSE;
			samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
			samplerCreateInfo.minLod = 0.0f;
			samplerCreateInfo.maxLod = 1.0f;
			samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
			samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

			SA_VK_ASSERT(vkCreateSampler(_instance.As<Vk::RenderInstance>().device, &samplerCreateInfo, nullptr, &sampler),
				CreationFailed, Rendering, L"Failed to create texture sampler!");


			std::vector<VkDescriptorImageInfo> imageDescs;
			imageDescs.reserve(3u * 4u);

			std::vector<VkWriteDescriptorSet> descWrites;
			descWrites.reserve(3u * 4u);

			VkWriteDescriptorSet mainWriteDesc{};
			mainWriteDesc.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			mainWriteDesc.pNext = nullptr;
			mainWriteDesc.dstSet = VK_NULL_HANDLE;
			mainWriteDesc.dstBinding = 0u;
			mainWriteDesc.dstArrayElement = 0u;
			mainWriteDesc.descriptorCount = 1u;
			mainWriteDesc.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;

			for (uint32 i = 0; i < 3u; ++i) // Fame num
			{
				mainWriteDesc.dstSet = litmaterial.mDescriptorSets[i];

				for (uint32 j = 0; j < 4u; ++j) // binding num.
				{
					VkWriteDescriptorSet& writeDesc = descWrites.emplace_back(mainWriteDesc);
					writeDesc.dstBinding = j;

					writeDesc.pImageInfo = &imageDescs.emplace_back(frameBuffers[i].GetAttachment(j).CreateDescriptorImageInfo(sampler));
				}
			}

			vkUpdateDescriptorSets(_instance.As<Vk::RenderInstance>().device, SizeOf(descWrites), descWrites.data(), 0, nullptr);
		}
	}
	void Destroy(IRenderInstance& _instance, IRenderSurface& _surface)
	{
		litPipeline.Destroy(_instance);
		litVert.Destroy(_instance);
		litFrag.Destroy(_instance);

		mainPipeline.Destroy(_instance);
		litCompVert.Destroy(_instance);
		litCompFrag.Destroy(_instance);

		_surface.DestroyFrameBuffers(_instance);
		renderPass.Destroy(_instance);
	}
};

#else

struct MainRenderInfos
{
	Vk::RenderPass renderPass;

	Vk::Shader unlitVert;
	Vk::Shader unlitFrag;
	Vk::Pipeline mainPipeline;


	void Create(IRenderInstance& _instance, IRenderSurface& _surface)
	{
		// RenderPass.
		const RenderPassDescriptor renderPassDesc = RenderPassDescriptor::CreateDefaultForward(&_surface);
		renderPass.Create(_instance, renderPassDesc);
		const std::vector<IFrameBuffer>& framebuffers = _surface.CreateFrameBuffers(_instance, renderPass, renderPassDesc);

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

			pipelineInfos.subPassIndex = 0u;
			pipelineInfos.shaders.push_back(PipelineShaderInfos{ &unlitVert, ShaderStage::Vertex });
			pipelineInfos.shaders.push_back(PipelineShaderInfos{ &unlitFrag, ShaderStage::Fragment });

			pipelineInfos.vertexBindingLayout.meshLayout = VertexLayout::Make<VertexComp::Default>();
			pipelineInfos.vertexBindingLayout.desiredLayout = VertexLayout::Make<VertexComp::PTex>();

			mainPipeline.Create(_instance, pipelineInfos);
		}
	}
	void Destroy(IRenderInstance& _instance, IRenderSurface& _surface)
	{
		mainPipeline.Destroy(_instance);
		unlitVert.Destroy(_instance);
		unlitFrag.Destroy(_instance);

		_surface.DestroyFrameBuffers(_instance);
		renderPass.Destroy(_instance);
	}
};

#endif

MainRenderInfos mainRender;

struct CubeRender
{
	Vk::Material material;

	TransffPRS camTr;

	Vk::Buffer camUBO;
	Vk::Buffer modelUBO;
	Vk::Texture texture;


	struct camUBOData
	{
		Mat4f proj = Mat4f::Identity;
		Mat4f viewInv = Mat4f::Identity;
		Vec3f viewPosition;
	};

	camUBOData camUBOd;

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


		camTr.position = Vec3f(0.0f, 0.0f, 10.0f);
		camUBOd.proj = Mat4f::MakePerspective(90.0f, 1200.0f / 800.0f);

		camUBO.Create(_instance.device, sizeof(camUBOData),
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &camUBOd);

		modelUBOd.modelMat = API_ConvertCoordinateSystem(Mat4f::MakeTransform(Vec3f(0.0f, 0.0f, 0.0f), Quatf::Identity, Vec3f::One * 5.0f));
		modelUBO.Create(_instance.device, sizeof(modelUBOData),
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &modelUBOd);


		MaterialCreateInfos matCreateInfos(mainRender.mainPipeline);

		MaterialBindingInfos& camBinding = matCreateInfos.bindings.emplace_back();
		camBinding.binding = 0u;
		camBinding.SetUniformBuffers({ &camUBO });

		MaterialBindingInfos& modelBinding = matCreateInfos.bindings.emplace_back();
		modelBinding.binding = 1u;
		modelBinding.SetUniformBuffers({ &modelUBO });

		MaterialBindingInfos& textureBinding = matCreateInfos.bindings.emplace_back();
		textureBinding.binding = 2u;
		textureBinding.SetImageSamplers2D({ &texture });

		material.Create(_instance, matCreateInfos);
	}

	void Update(const Vk::RenderInstance& _instance)
	{
		camUBOd.viewInv = API_ConvertCoordinateSystem(camTr.Matrix()).GetInversed();
		camUBOd.viewPosition = API_ConvertCoordinateSystem(camTr.position);
		camUBO.UpdateData(_instance.device, &camUBOd, sizeof(camUBOd));
	}

	void Destroy(const Vk::RenderInstance& _instance)
	{
		material.Destroy(_instance);

		camUBO.Destroy(_instance.device);
		modelUBO.Destroy(_instance.device);

		texture.Destroy(_instance);
	}
};

CubeRender cubeRender;


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


	Vk::Mesh cubeMesh;
	cubeMesh.Create(instance, RawMesh::Cube());

	Vec3f lightPos;

	Chrono chrono;
	float speed = 0.1f;


	LOG("=== Loop ===");
	while (!window.ShouldClose())
	{
		float deltaTime = chrono.Restart() * 0.00005f;


		window.Update();

		window.TEST(cubeRender.camTr, lightPos, deltaTime * speed);
		cubeRender.Update(instance);


		// Begin Surface.
		RenderFrame frame = surface.Begin(instance);


		// Subpass 0: G-Buffer composition.
		mainRender.mainPipeline.Bind(frame);

		cubeRender.material.Bind(frame, mainRender.mainPipeline);
		cubeMesh.Draw(frame);
		//

#if __DEFERRED

		frame.buffer.NextSubpass();

		// Subpass 1: Illumination.
		mainRender.litPipeline.Bind(frame);

		mainRender.litmaterial.Bind(frame, mainRender.litPipeline);
		cubeMesh.Draw(frame);

#endif

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
