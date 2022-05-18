#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>
#include <string>
#include <unordered_map>

namespace Hephaestus
{
	class PhysicalDevice;
	class Queue;

	/*
	*	
	*/
	struct Device_Constructor
	{
		explicit Device_Constructor( PhysicalDevice& _physicalDevice, VkSurfaceKHR _surface, std::unordered_map<const char*, bool> _requestedExtensions = {} ) :
			physicalDevice( _physicalDevice ),
			surface( _surface ),
			requestedExtensions( _requestedExtensions )
		{};

		const PhysicalDevice& physicalDevice;
		VkSurfaceKHR surface;
		std::unordered_map<const char*, bool> requestedExtensions;
	};

	/*
	*	Wrapper class for the Vulkan Logical Device:
	*	Represents an intialized Vulkan device that is ready to create all other objects
	*/
	class Device
	{
		Device( const Device& ) = delete;
		Device& operator=( const Device& ) = delete;
		Device( Device&& ) = delete;
		Device& operator=( Device&& ) = delete;
	public:
		Device( const Device_Constructor& deviceConstructor );
		~Device();

		bool OnCreate();
		void OnDestroy();

		inline const VkDevice& GetVkDevice() const
		{
			return m_logicaldevice;
		}

		uint32_t GetQueueFamilyIndex( VkQueueFlagBits queueFlags );

		bool IsExtensionSupported( const std::string& requestedExtension );

	private:
		VkDevice m_logicaldevice;
		const PhysicalDevice& m_physicalDevice;
		VkSurfaceKHR m_surface;
		std::vector<VkExtensionProperties> m_deviceExtensions;
		std::vector<const char*> m_enabledExtensions;
		std::vector<std::vector<Queue>> m_queues;
	};
}