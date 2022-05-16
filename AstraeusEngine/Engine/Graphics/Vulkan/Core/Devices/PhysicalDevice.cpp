#include "PhysicalDevice.h"

#include <DebugLog.h>

namespace Hephaestus
{

	PhysicalDevice::PhysicalDevice( const PhysicalDevice_Constructor& physicalDeviceConstructor ) :
		m_physicalDevice( physicalDeviceConstructor.physicalDevice ),
		m_instance( physicalDeviceConstructor.instance )
	{
		vkGetPhysicalDeviceFeatures( m_physicalDevice, &features );
		vkGetPhysicalDeviceProperties( m_physicalDevice, &properties );
		vkGetPhysicalDeviceMemoryProperties( m_physicalDevice, &memoryProperties );

		DEBUG_LOG( LOG::INFO, "Found GPU: {}", std::string( properties.deviceName ) );

		uint32_t queue_family_properties_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties( m_physicalDevice, &queue_family_properties_count, nullptr );
		queueFamilyProperties = std::vector<VkQueueFamilyProperties>( queue_family_properties_count );
		vkGetPhysicalDeviceQueueFamilyProperties( m_physicalDevice, &queue_family_properties_count, queueFamilyProperties.data() );
	}

	PhysicalDevice::~PhysicalDevice()
	{}

	bool PhysicalDevice::OnCreate()
	{
		return true;
	}

	void PhysicalDevice::OnDestroy()
	{}

	const VkPhysicalDeviceFeatures& PhysicalDevice::GetFeatures() const
	{
		return features;
	}

	const VkPhysicalDeviceProperties& PhysicalDevice::GetProperties() const
	{
		return properties;
	}

	const VkPhysicalDeviceMemoryProperties& PhysicalDevice::GetMemoryProperties() const
	{
		return memoryProperties;
	}

	const std::vector<VkQueueFamilyProperties>& PhysicalDevice::GetQueueFamilyProperties() const
	{
		return queueFamilyProperties;
	}

	VkBool32 PhysicalDevice::IsPresentSupported( VkSurfaceKHR surface, uint32_t queue_family_index ) const
	{
		VkBool32 present_supported{ VK_FALSE };

		if( surface != VK_NULL_HANDLE )
		{
			if( vkGetPhysicalDeviceSurfaceSupportKHR( m_physicalDevice, queue_family_index, surface, &present_supported ) != VK_SUCCESS )
			{

			}
		}

		return present_supported;
	}

}