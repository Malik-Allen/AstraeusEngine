#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>
#include <string>

namespace Hephaestus
{
	class Instance;

	/*
	*
	*/
	struct PhysicalDevice_Constructor
	{
		explicit PhysicalDevice_Constructor( Instance& _instance, VkPhysicalDevice _physicalDevice ) :
			instance( _instance ),
			physicalDevice( _physicalDevice )
		{};

		const Instance& instance;
		VkPhysicalDevice physicalDevice;
	};

	/*
	*	PhsyicalDevice is a wrapper class for the VkPhysicalDevice:
	*	Representing a specific Vulkan-compatible device, like a graphics card
	*/
	class PhysicalDevice
	{
		PhysicalDevice( const PhysicalDevice& ) = delete;
		PhysicalDevice& operator=( const PhysicalDevice& ) = delete;
		PhysicalDevice( PhysicalDevice&& ) = delete;
		PhysicalDevice& operator=( PhysicalDevice&& ) = delete;
	public:
		PhysicalDevice( const PhysicalDevice_Constructor& physicalDeviceConstructor );
		~PhysicalDevice();

		bool OnCreate();
		void OnDestroy();

		inline const VkPhysicalDevice& GetVkPhysicalDevice() const
		{
			return m_physicalDevice;
		}

		inline const Instance& GetInstance() const
		{
			return m_instance;
		}

		const VkPhysicalDeviceFeatures& GetFeatures() const;

		const VkPhysicalDeviceProperties& GetProperties() const;

		const VkPhysicalDeviceMemoryProperties& GetMemoryProperties() const;

		const std::vector<VkQueueFamilyProperties>& GetQueueFamilyProperties() const;

		VkBool32 IsPresentSupported( VkSurfaceKHR surface, uint32_t queue_family_index ) const;

	private:
		VkPhysicalDevice m_physicalDevice;
		const Instance& m_instance;

		/** @brief Properties of the physical device including limits that the application can check against */
		VkPhysicalDeviceProperties properties;
		/** @brief Features of the physical device that an application can use to check if a feature is supported */
		VkPhysicalDeviceFeatures features;
		/** @brief Features that have been enabled for use on the physical device */
		VkPhysicalDeviceFeatures enabledFeatures;
		/** @brief Memory types and heaps of the physical device */
		VkPhysicalDeviceMemoryProperties memoryProperties;
		/** @brief Queue family properties of the physical device */
		std::vector<VkQueueFamilyProperties> queueFamilyProperties;
		/** @brief List of extensions supported by the device */
		std::vector<std::string> supportedExtensions;
	};

}