#pragma once

#include <vulkan/vulkan_core.h>

namespace Hephaestus
{
	class CommandBuffer
	{
	public:
		CommandBuffer();
		~CommandBuffer();

		const VkCommandBuffer& GetVkCommandBuffer() const;

	private:
		VkCommandBuffer m_commandBuffer;
	};
}