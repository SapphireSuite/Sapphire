// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/Framebuffer.hpp>

namespace Sa::vk
{
	Framebuffer::Framebuffer(const DummyRenderPass* _renderPass, const ImageExtent& _extent)
		: mRenderPass{ _renderPass }, mExtent { _extent }
	{
		throw;
	}

	Framebuffer::~Framebuffer()
	{
		throw;
	}

	void Framebuffer::Begin() const
	{
		throw;
	}

	void Framebuffer::End() const
	{
		throw;
	}
}
