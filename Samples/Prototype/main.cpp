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
#include <Rendering/Vulkan/Buffers/VkBuffer.hpp>
#include <Rendering/Vulkan/Buffers/VkFrameBuffer.hpp>

#include <SDK/Assets/Shader/ShaderAsset.hpp>
#include <SDK/Assets/Texture/TextureAsset.hpp>

#include <Window/GLFW/System/GLFWWindow.hpp>
using namespace Sa;

#define LOG(_str) std::cout << _str << std::endl;


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

	// Main RenderPass.
	Vk::RenderPass mainRP;
	const RenderPassDescriptor mainRPDesc = RenderPassDescriptor::CreateDefaultForward(&surface);
	mainRP.Create(instance, mainRPDesc);
	const uint32 mainRPID = surface.AddRenderPass(instance, mainRP, mainRPDesc);

	//// UI RenderPass.
	//Vk::RenderPass UIRP;
	//const RenderPassDescriptor UIRPDesc = RenderPassDescriptor::CreateDefaultForward(&surface);
	//UIRP.Create(instance, UIRPDesc);
	//const uint32 UIRPID = surface.AddRenderPass(instance, UIRP, UIRPDesc);


	struct camUBOData
	{
		Mat4f proj = Mat4f::Identity;
		Mat4f viewInv = Mat4f::Identity;
		Vec3f viewPosition;
	};

	camUBOData camUBOd;
	TransffPRS camTr;
	camTr.position = Vec3f(0.0f, 0.0f, 10.0f);
	camUBOd.proj = Mat4f::MakePerspective(90.0f, 1200.0f / 800.0f);

	Vk::Buffer camUBO;
	camUBO.Create(instance.device, sizeof(camUBOData),
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &camUBOd);

	struct modelUBOData
	{
		Mat4f modelMat = Mat4f::Identity;

		float uvTilling = 1.0f;
		float uvOffset = 0.0f;
	};

	modelUBOData modelUBOd;
	modelUBOd.modelMat = API_ConvertCoordinateSystem(Mat4f::MakeTransform(Vec3f(0.0f, 0.0f, 0.0f), Quatf::Identity, Vec3f::One * 5.0f));

	Vk::Buffer modelUBO;
	modelUBO.Create(instance.device, sizeof(modelUBOData),
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &modelUBOd);


	Vk::Mesh cubeMesh;
	cubeMesh.Create(instance, RawMesh::Cube());

	TextureImportInfos importInfos; importInfos.format = RenderFormat::sRGBA_32;
	TextureAsset cubeTextAsset;
	SA_ASSERT(cubeTextAsset.Import("../../Engine/Resources/Textures/missing_texture.png", importInfos), InvalidParam, SDK, L"Import failed");
	Vk::Texture cubeText;
	cubeText.Create(instance, cubeTextAsset.GetRawData());

	Vk::Shader cubeVert;
	ShaderAsset cubeVertAsset;
	SA_ASSERT(cubeVertAsset.Import("../../Engine/Resources/Shaders/unlit.vert"), InvalidParam, SDK, L"Import failed");
	cubeVert.Create(instance, cubeVertAsset.GetRawData());

	Vk::Shader cubeFrag;
	ShaderAsset cubeFragAsset;
	SA_ASSERT(cubeFragAsset.Import("../../Engine/Resources/Shaders/unlit.frag"), InvalidParam, SDK, L"Import failed");
	cubeFrag.Create(instance, cubeFragAsset.GetRawData());


	// Pipeline.
	PipelineCreateInfos cubePipelineInfos(mainRP, mainRPDesc);

	{
		PipelineBindingInfos camUBOBinding;
		camUBOBinding.binding = 0u;
		camUBOBinding.stages = ShaderStage::Vertex;
		camUBOBinding.type = ShaderBindingType::UniformBuffer;
		camUBOBinding.bufferDataSize = sizeof(camUBOData);
		camUBOBinding.buffers.push_back(&camUBO);

		cubePipelineInfos.AddBinding(camUBOBinding);


		PipelineBindingInfos modelUBOBinding;
		modelUBOBinding.binding = 1u;
		modelUBOBinding.stages = ShaderStage::Vertex;
		modelUBOBinding.type = ShaderBindingType::UniformBuffer;
		modelUBOBinding.bufferDataSize = sizeof(modelUBOData);
		modelUBOBinding.buffers.push_back(&modelUBO);

		cubePipelineInfos.AddBinding(modelUBOBinding);


		PipelineBindingInfos textureBinding;
		textureBinding.binding = 2u;
		textureBinding.stages = ShaderStage::Fragment;
		textureBinding.type = ShaderBindingType::ImageSampler2D;
		textureBinding.buffers.push_back(&cubeText);

		cubePipelineInfos.AddBinding(textureBinding);
	}

	cubePipelineInfos.shaders.push_back(PipelineShaderInfos{ &cubeVert, ShaderStage::Vertex });
	cubePipelineInfos.shaders.push_back(PipelineShaderInfos{ &cubeFrag, ShaderStage::Fragment });

	cubePipelineInfos.vertexBindingLayout.meshLayout = cubeMesh.GetLayout();
	cubePipelineInfos.vertexBindingLayout.desiredLayout = VertexLayout::Make<VertexComp::Position | VertexComp::Texture>();

	Vk::Pipeline cubePipeline;
	cubePipeline.Create(instance, cubePipelineInfos);


	Vec3f lightPos;

	Chrono chrono;
	float speed = 0.1f;


	LOG("=== Loop ===");
	while (!window.ShouldClose())
	{
		float deltaTime = chrono.Restart() * 0.00005f;


		window.Update();

		window.TEST(camTr, lightPos, deltaTime * speed);
		camUBOd.viewInv = API_ConvertCoordinateSystem(camTr.Matrix()).GetInversed();
		camUBOd.viewPosition = API_ConvertCoordinateSystem(camTr.position);
		camUBO.UpdateData(instance.device, &camUBOd, sizeof(camUBOd));


		// Begin Surface.
		surface.Begin(instance);


		// Main framebuffer.
		{
			FrameInfos mainFI = surface.GetFrameInfos(mainRPID);

			IFrameBuffer& mainFB = mainFI.frameBuffer;

			mainFB.Begin();

			cubePipeline.Bind(mainFI);

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

	cubePipeline.Destroy(instance);

	camUBO.Destroy(instance.device);
	modelUBO.Destroy(instance.device);


	cubeVert.Destroy(instance);
	cubeFrag.Destroy(instance);
	cubeText.Destroy(instance);
	cubeMesh.Destroy(instance);

	//surface.RemoveRenderPass(instance, UIRPID);
	surface.RemoveRenderPass(instance, mainRPID);

	//UIRP.Destroy(instance);
	mainRP.Destroy(instance);

	instance.DestroyRenderSurface(surface);

	window.Destroy();

	instance.Destroy();


	LOG("=== UnInit ===");
	IWindow::UnInit();
	IRenderInstance::UnInit();


	LOG("\n=== End ===");

	return 0;
}
