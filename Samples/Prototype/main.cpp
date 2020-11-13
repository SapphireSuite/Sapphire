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


struct RenderInfos
{
	Vk::RenderPass renderPass;
	uint32 renderPassID = uint32(-1);

	Vk::Shader vert;
	Vk::Shader frag;
	
	Vk::Pipeline pipeline;
};

RenderInfos mainRender;
RenderInfos UIRender;

void CreateMainRender(IRenderInstance& _instance, IRenderSurface& _surface)
{
	// RenderPass.
	const RenderPassDescriptor renderPassDesc = RenderPassDescriptor::CreateDefaultForward(&_surface);
	mainRender.renderPass.Create(_instance, renderPassDesc);
	mainRender.renderPassID = _surface.AddRenderPass(_instance, mainRender.renderPass, renderPassDesc);


	// Vertex Shader.
	{
		const char* assetPath = "Bin/Engine/Shaders/unlit_VS.spha";
		const char* resourcePath = "../../Engine/Resources/Shaders/unlit.vert";

		ShaderAsset asset;
		uint32 res = asset.TryLoadImport(assetPath, resourcePath, ShaderImportInfos());

		if (res == 0)
			asset.Save(assetPath);
		else if(res == -1)
			SA_ASSERT(false, InvalidParam, SDK, L"Import failed");

		mainRender.vert.Create(_instance, asset.GetRawData());
	}

	// Fragment Shader.
	{
		const char* assetPath = "Bin/Engine/Shaders/unlit_FS.spha";
		const char* resourcePath = "../../Engine/Resources/Shaders/unlit.frag";

		ShaderAsset asset;
		uint32 res = asset.TryLoadImport(assetPath, resourcePath, ShaderImportInfos());

		if (res == 0)
			asset.Save(assetPath);
		else if (res == -1)
			SA_ASSERT(false, InvalidParam, SDK, L"Import failed");

		mainRender.frag.Create(_instance, asset.GetRawData());
	}


	// Pipeline.
	PipelineCreateInfos mainPipelineInfos(mainRender.renderPass, renderPassDesc);

	{
		PipelineBindingInfos& camUBOBinding = mainPipelineInfos.bindings.emplace_back();
		camUBOBinding.binding = 0u;
		camUBOBinding.stages = ShaderStage::Vertex;
		camUBOBinding.type = ShaderBindingType::UniformBuffer;


		PipelineBindingInfos& modelUBOBinding = mainPipelineInfos.bindings.emplace_back();
		modelUBOBinding.binding = 1u;
		modelUBOBinding.stages = ShaderStage::Vertex;
		modelUBOBinding.type = ShaderBindingType::UniformBuffer;


		PipelineBindingInfos& textureBinding = mainPipelineInfos.bindings.emplace_back();
		textureBinding.binding = 2u;
		textureBinding.stages = ShaderStage::Fragment;
		textureBinding.type = ShaderBindingType::ImageSampler2D;
	}

	mainPipelineInfos.shaders.push_back(PipelineShaderInfos{ &mainRender.vert, ShaderStage::Vertex });
	mainPipelineInfos.shaders.push_back(PipelineShaderInfos{ &mainRender.frag, ShaderStage::Fragment });

	mainPipelineInfos.vertexBindingLayout.meshLayout = VertexLayout::Make<VertexComp::Default>();
	mainPipelineInfos.vertexBindingLayout.desiredLayout = VertexLayout::Make<VertexComp::Position | VertexComp::Texture>();

	mainRender.pipeline.Create(_instance, mainPipelineInfos);
}
void DestroyMainRender(IRenderInstance& _instance, IRenderSurface& _surface)
{
	mainRender.pipeline.Destroy(_instance);

	mainRender.vert.Destroy(_instance);
	mainRender.frag.Destroy(_instance);

	_surface.RemoveRenderPass(_instance, mainRender.renderPassID);
	mainRender.renderPass.Destroy(_instance);
}

void CreateUIRender(IRenderInstance& _instance, IRenderSurface& _surface)
{
	// RenderPass.
	const RenderPassDescriptor renderPassDesc = RenderPassDescriptor::CreateDefaultForward(&_surface);
	UIRender.renderPass.Create(_instance, renderPassDesc);
	UIRender.renderPassID = _surface.AddRenderPass(_instance, UIRender.renderPass, renderPassDesc);
}
void DestroyUIRender(IRenderInstance& _instance, IRenderSurface& _surface)
{
	_surface.RemoveRenderPass(_instance, UIRender.renderPassID);
	UIRender.renderPass.Destroy(_instance);
}

struct CubeMatInfos
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

			if (res == 0)
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


		MaterialCreateInfos matCreateInfos(mainRender.pipeline);

		MaterialBindingInfos& camBinding = matCreateInfos.bindings.emplace_back();
		camBinding.binding = 0u;
		camBinding.type = ShaderBindingType::UniformBuffer;
		camBinding.bufferDataSize = sizeof(camUBOData);
		camBinding.buffers.push_back(&camUBO);

		MaterialBindingInfos& modelBinding = matCreateInfos.bindings.emplace_back();
		modelBinding.binding = 1u;
		modelBinding.type = ShaderBindingType::UniformBuffer;
		modelBinding.bufferDataSize = sizeof(modelUBOData);
		modelBinding.buffers.push_back(&modelUBO);

		MaterialBindingInfos& textureBinding = matCreateInfos.bindings.emplace_back();
		textureBinding.binding = 2u;
		textureBinding.type = ShaderBindingType::ImageSampler2D;
		textureBinding.buffers.push_back(&texture);

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

CubeMatInfos cubeMatInfos;


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

	CreateMainRender(instance, surface);
	//CreateUIRender(instance, surface);


	cubeMatInfos.Create(instance);


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

		window.TEST(cubeMatInfos.camTr, lightPos, deltaTime * speed);
		cubeMatInfos.Update(instance);


		// Begin Surface.
		surface.Begin(instance);


		// Main framebuffer.
		{
			FrameInfos mainFI = surface.GetFrameInfos(mainRender.renderPassID);

			IFrameBuffer& mainFB = mainFI.frameBuffer;

			mainFB.Begin();

			mainRender.pipeline.Bind(mainFI);

			cubeMatInfos.material.Bind(mainFI, mainRender.pipeline);

			cubeMesh.Draw(mainFB);

			mainFB.End();
		}


		//// UI framebuffer.
		//{
		//	FrameInfos UIFI = surface.GetFrameInfos(UIRPID);
		//	IFrameBuffer& UIFB = UIFI.frameBuffer;

		//	UIFB.Begin();

		//	UIFB.End();
		//}


		// End Surface.
		surface.End(instance);
	}

	
	LOG("=== Destroy ===");
	vkDeviceWaitIdle(instance.device);


	DestroyMainRender(instance, surface);
	//DestroyUIRender(instance, surface);

	cubeMatInfos.Destroy(instance);

	cubeMesh.Destroy(instance);


	instance.DestroyRenderSurface(surface);

	window.Destroy();

	instance.Destroy();


	LOG("=== UnInit ===");
	IWindow::UnInit();
	IRenderInstance::UnInit();


	LOG("\n=== End ===");

	return 0;
}
