#include "Queue.h"
#include "../Devices/Device.h"
#include "CommandBuffer.h"

namespace Hephaestus
{

	Queue::Queue( const Queue_Constructor& queueConstructor ) :
		m_device( queueConstructor.device ),
		m_queue( VK_NULL_HANDLE ),
		m_familyIndex( queueConstructor.familyIndex ),
		m_properties( queueConstructor.properties ),
		m_canPresent( queueConstructor.canPresent ),
		m_index( queueConstructor.index )
	{
		// Retrieves queue handles for the queue family
		vkGetDeviceQueue( m_device.GetVkDevice(), m_familyIndex, m_index, &m_queue );
	}

	Queue::Queue( Queue&& other ) :
		m_device( other.m_device ),
		m_queue( other.m_queue ),
		m_familyIndex( other.m_familyIndex ),
		m_properties( other.m_properties ),
		m_canPresent( other.m_canPresent ),
		m_index( other.m_index )
	{}

	Queue::~Queue()
	{}

	const Device& Queue::GetDevice() const
	{
		return m_device;
	}

	const VkQueue& Queue::GetVkQueue() const
	{
		return m_queue;
	}

	uint32_t Queue::GetFamilyIndex() const
	{
		return m_familyIndex;
	}

	const VkQueueFamilyProperties& Queue::GetProperties() const
	{
		return m_properties;
	}

	VkBool32 Queue::CanPresent() const
	{
		return m_canPresent;
	}

	uint32_t Queue::GetIndex() const
	{
		return m_index;
	}

	VkResult Queue::Submit( const std::vector<VkSubmitInfo>& submitInfos, VkFence fence ) const
	{
		return vkQueueSubmit( m_queue, static_cast<uint32_t>( submitInfos.size() ), submitInfos.data(), fence );
	}

	VkResult Queue::Submit( const CommandBuffer& commandBuffer, VkFence fence ) const
	{
		VkSubmitInfo submitInfo{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer.GetVkCommandBuffer();

		return Submit( { submitInfo }, fence );
	}

	VkResult Queue::Present( const VkPresentInfoKHR& presentInfos ) const
	{
		if( !m_canPresent )
		{
			return VK_ERROR_INCOMPATIBLE_DISPLAY_KHR;
		}

		return vkQueuePresentKHR( m_queue, &presentInfos );
	}

	VkResult Queue::WaitIdle() const
	{
		return vkQueueWaitIdle( m_queue );
	}

}