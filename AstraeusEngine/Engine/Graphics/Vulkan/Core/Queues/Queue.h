// Copyright (c) 2022 Malik Allen

#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>

namespace Hephaestus
{
	class Device;
	class CommandBuffer;

	struct Queue_Constructor
	{
		explicit Queue_Constructor( Device& _device, uint32_t _familyIndex, VkQueueFamilyProperties _properties, VkBool32 _canPresent, uint32_t _index ) :
			device( _device ),
			familyIndex( _familyIndex ),
			properties( _properties ),
			canPresent( _canPresent ),
			index( _index )
		{};

		Device& device;
		uint32_t familyIndex;
		VkQueueFamilyProperties properties;
		VkBool32 canPresent;
		uint32_t index;
	};

	/*
	*	Queue is a wrapper class for VkQueue:
	*	Representing a queue of commands to be executed on the device, requested by filling CommandBuffers and submitting them to Queues
	*/
	class Queue
	{
		Queue( const Queue& ) = delete;
		Queue& operator=( const Queue& ) = delete;
		Queue& operator=( Queue&& ) = delete;

	public:
		Queue( const Queue_Constructor& queueConstructor );
		Queue( Queue&& other );
		~Queue();

		const Device& GetDevice() const;
		const VkQueue& GetVkQueue() const;
		uint32_t GetFamilyIndex() const;
		const VkQueueFamilyProperties& GetProperties() const;
		VkBool32 CanPresent() const;
		uint32_t GetIndex() const;

		/*
		*	@brief	Submits a submit info array to graphics queue
		*/
		VkResult Submit( const std::vector<VkSubmitInfo>& submitInfos, VkFence fence ) const;

		/*
		*	@brief	Submits a command buffer to graphics queue, creating VkSubmitInfo in the process
		*/
		VkResult Submit( const CommandBuffer& commandBuffer, VkFence fence ) const;

		/*	@brief	Submits the request to present an image to the swap chain*/
		VkResult Present( const VkPresentInfoKHR& presentInfos ) const;

		/*
		*	@brief	Waits for operations in command queue to be finished
		*/
		VkResult WaitIdle() const;


	private:
		Device& m_device;
		VkQueue m_queue;
		uint32_t m_familyIndex;
		VkQueueFamilyProperties m_properties;
		VkBool32 m_canPresent;
		uint32_t m_index;
	};
};