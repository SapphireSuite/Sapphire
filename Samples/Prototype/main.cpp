// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <string>
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "../../Engine/Libs/stblib/tiny_obj_loader.h"

#include <Sapphire/Core/Time/Chrono.hpp>
#include <Sapphire/Maths/Space/Transform.hpp>

#include <Sapphire/Window/GLFWWindow.hpp>

#include <Sapphire/Rendering/Vulkan/VkRenderInstance.hpp>
#include <Sapphire/Rendering/Vulkan/Queue/VkCommandBuffer.hpp>
#include <Sapphire/Rendering/Framework/Model/UniformBufferObject.hpp>

// Material
#include <Sapphire/Rendering/Framework/Primitives/Pipeline/PipelineCreateInfos.hpp>
#include <Sapphire/Rendering/Vulkan/Primitives/Pipeline/VkShader.hpp>
#include <Sapphire/Rendering/Vulkan/Model/VkTexture.hpp>
#include <Sapphire/Rendering/Vulkan/Primitives/VkRenderMaterial.hpp>

// Mesh
#include <Sapphire/Rendering/Vulkan/Model/VkMesh.hpp>

using namespace Sa;

#define LOG(_str) std::cout << _str << std::endl;

void LoadModel(const std::string& _filename,
	std::vector<std::vector<Vertex>>& _meshesVertices,
	std::vector<std::vector<uint32>>& _meshedIndices)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	SA_ASSERT(tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, _filename.c_str()),
		InvalidParam, Rendering, L"Failed to load obj model!")

	for (uint32 i = 0u; i < shapes.size(); ++i)
	{
		std::vector<Vertex>& vertices = _meshesVertices.emplace_back();
		std::vector<uint32>& indices = _meshedIndices.emplace_back();

		for (auto indexIt = shapes[i].mesh.indices.begin(); indexIt != shapes[i].mesh.indices.end(); ++indexIt)
		{
			const Vertex vertex
			{
				Vec3f(attrib.vertices[3 * indexIt->vertex_index + 0],
				attrib.vertices[3 * indexIt->vertex_index + 1],
				attrib.vertices[3 * indexIt->vertex_index + 2]),

				Vec3f(attrib.normals[3 * indexIt->normal_index + 0],
				attrib.normals[3 * indexIt->normal_index + 1],
				attrib.normals[3 * indexIt->normal_index + 2]),

				Vec2f(attrib.texcoords[2 * indexIt->texcoord_index + 0],
				attrib.texcoords[2 * indexIt->texcoord_index + 1])
			};

			vertices.push_back(vertex);
			indices.push_back(static_cast<uint32>(indices.size()));
		}
	}
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


	// Create Material.
	VkShader vertShader;
	vertShader.Create(instance, L"../../Bin/Shaders/default_vert.spv");

	VkShader fragShader;
	fragShader.Create(instance, L"../../Bin/Shaders/default_frag.spv");

	VkTexture bodyTexture;
	bodyTexture.Create(instance, "../../Engine/Resources/Models/Magikarp/Body.png");

	VkTexture eyesTexture;
	eyesTexture.Create(instance, "../../Engine/Resources/Models/Magikarp/Eyes.png");

	PipelineCreateInfos bodyPipelineInfos
	{
		surface,
		surface.GetViewport(),

		&vertShader,
		&fragShader,

		{ &bodyTexture },

		PolygonMode::Fill,
		CullingMode::None,
		FrontFaceMode::Clockwise
	};

	VkRenderMaterial bodyMat;
	bodyMat.CreatePipeline(instance, bodyPipelineInfos);

	PipelineCreateInfos eyesPipelineInfos
	{
		surface,
		surface.GetViewport(),

		&vertShader,
		&fragShader,

		{ &eyesTexture },

		PolygonMode::Fill,
		CullingMode::None,
		FrontFaceMode::Clockwise
	};

	VkRenderMaterial eyesMat;
	eyesMat.CreatePipeline(instance, eyesPipelineInfos);

	std::vector<std::vector<Vertex>> meshesVertices;
	std::vector<std::vector<uint32>> meshedIndices;

	LoadModel("../../Engine/Resources/Models/Magikarp/Magikarp.obj", meshesVertices, meshedIndices);

	VkMesh bodyMesh;
	bodyMesh.Create(instance, meshesVertices[0], meshedIndices[0]);

	VkMesh eyesMesh;
	eyesMesh.Create(instance, meshesVertices[1], meshedIndices[1]);

	// Create Mesh.
	//VkMesh mesh;
	//const std::vector<Vertex> vertices =
	//{
	//	{ { -0.5f, -0.5f, 0.5f }, Vec3f::Forward, { 0.0f, 0.0f } },
	//	{ { 0.5f, -0.5f, 0.5f }, Vec3f::Forward, { 1.0f, 0.0f } },
	//	{ { 0.5f, 0.5f, 0.5f }, Vec3f::Forward, { 1.0f, 1.0f } },
	//	{ { -0.5f, 0.5f, 0.5f }, Vec3f::Forward, { 0.0f, 1.0f } },

	//	{ { 0.5f + -0.5f, 0.5f + -0.5f, -0.5f }, Vec3f::Forward, { 0.0f, 0.0f } },
	//	{ { 0.5f + 0.5f, 0.5f + -0.5f, -0.5f }, Vec3f::Forward, { 1.0f, 0.0f } },
	//	{ { 0.5f + 0.5f, 0.5f + 0.5f, -0.5f }, Vec3f::Forward, { 1.0f, 1.0f } },
	//	{ { 0.5f + -0.5f, 0.5f + 0.5f, -0.5f }, Vec3f::Forward, { 0.0f, 1.0f } }
	//};
	//
	//const std::vector<uint32> indices =
	//{
	//	0, 1, 2, 2, 3, 0,
	//	4, 5, 6, 6, 7, 4
	//};

	//mesh.Create(instance, vertices, indices);


	const float r = 1.0f;
	const float l = -1.0f;
	const float t = 1.0f;
	const float b = -1.0f;
	const float n = 0.1f;
	const float f = 15.0f;

	Mat4f orthoMat
	(
		2 / (r - l), 0, 0, -(r + l) / (r - l),
		0, 2 / (t - b), 0, -(t + b) / (t - b),
		0, 0, -2 / (f - n), -(f + n) / (f - n),
		0, 0, 0, 1
	);


	const float fov = 90.0f;
	const float ratio = 1;
	const float scale = 1.f / tanf((fov / 2.f) * Maths::DegToRad);
	float tan_half_angle = std::tan(Maths::DegToRad * fov / 2);

	//Mat4f perspMat
	//(
	//	scale / ratio, 0, 0, 0,
	//	0, scale, 0, 0,
	//	0, 0, (f + n)/(n - f), (2 * n * f) / (n - f),
	//	0, 0, -1, 0
	//);


	Mat4f perspMat
	(
		1 / (ratio * tan_half_angle), 0, 0, 0,
		0, -1 / (tan_half_angle), 0, 0,
		0, 0, -(f + n) / (f - n), -(2 * f * n) / (f - n),
		0, 0, -1, 1
	);


	Transff camTr;
	camTr.position.z = 1.0f;

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

		window.TEST(camTr, speed * deltaTime);


		// Update Uniform Buffer.
		UniformBufferObject ubo;
		ubo.modelMat = Mat4f::MakeScale(Vec3f(0.000001f)) * Mat4f::MakeRotation(Quatf(90_deg, Vec3f::Right));
		//ubo.modelMat = Mat4f::MakeRotation(Quatf(time, Vec3f::Right));
		ubo.viewMat = camTr.Matrix().GetTransposed();
		ubo.projMat = orthoMat;


		VkRenderFrame frame = surface.GetSwapChain().Update(instance.GetDevice());

		void* data;
		vkMapMemory(instance.GetDevice(), frame.uniformBuffer, 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(instance.GetDevice(), frame.uniformBuffer);


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

		bodyMat.Bind(frame);
		bodyMesh.Draw(frame);

		eyesMat.Bind(frame);
		eyesMesh.Draw(frame);

		vkCmdEndRenderPass(frame.graphicsCommandBuffer);

		SA_VK_ASSERT(vkEndCommandBuffer(frame.graphicsCommandBuffer),
			LibCommandFailed, Rendering, L"Failed to end command buffer!");
	}



	// === Destroy ===
	vkDeviceWaitIdle(instance.GetDevice());

	// Destroy mesh.
	bodyMesh.Destroy(instance);
	eyesMesh.Destroy(instance);

	// Destroy Material.
	bodyMat.DestroyPipeline(instance);
	eyesMat.DestroyPipeline(instance);

	bodyTexture.Destroy(instance);
	eyesTexture.Destroy(instance);
	fragShader.Destroy(instance);
	vertShader.Destroy(instance);


	instance.DestroyRenderSurface(window);
	
	window.Destroy();

	instance.Destroy();


	LOG("\n=== End ===");

	return 0;
}
