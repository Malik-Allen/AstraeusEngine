// Copyright (c) 2022 Malik Allen

#include "CommandBuffer.h"

namespace Hephaestus
{

	CommandBuffer::CommandBuffer() :
		m_commandBuffer( VK_NULL_HANDLE )
	{}

	CommandBuffer::~CommandBuffer()
	{}

	const VkCommandBuffer& CommandBuffer::GetVkCommandBuffer() const
	{
		return m_commandBuffer;
	}

}