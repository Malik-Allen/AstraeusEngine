// Copyright (c) 2022 Malik Allen

#include "Device.h"
#include "PhysicalDevice.h"
#include "../Queues/Queue.h"

#include <DebugLog.h>

#include <algorithm>

namespace Hephaestus
{
	Device::Device( const Device_Constructor& deviceConstructor ) :
		m_logicaldevice( VK_NULL_HANDLE ),
		m_physicalDevice( deviceConstructor.physicalDevice ),
		m_surface( deviceConstructor.surface )
	{
		DEBUG_LOG( LOG::INFO, "Selected GPU: {}", std::string( m_physicalDevice.GetProperties().deviceName ) );
		VkResult result;

		// Prepare the device queues
		uint32_t                             queueFamilyPropertiesCount = static_cast< uint32_t >( m_physicalDevice.GetQueueFamilyProperties().size() );
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos( queueFamilyPropertiesCount, { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO } );
		std::vector<std::vector<float>>      queuePriorities( queueFamilyPropertiesCount );

		for( uint32_t queueFamilyIndex = 0U; queueFamilyIndex < queueFamilyPropertiesCount; ++queueFamilyIndex )
		{
			const VkQueueFamilyProperties& queueFamilyProperty = m_physicalDevice.GetQueueFamilyProperties()[queueFamilyIndex];

			queuePriorities[queueFamilyIndex].resize( queueFamilyProperty.queueCount, 0.5f );

			VkDeviceQueueCreateInfo& queue_create_info = queueCreateInfos[queueFamilyIndex];

			queue_create_info.queueFamilyIndex = queueFamilyIndex;
			queue_create_info.queueCount = queueFamilyProperty.queueCount;
			queue_create_info.pQueuePriorities = queuePriorities[queueFamilyIndex].data();
		}

		// Check extensions 
		uint32_t deviceExtensionCount;
		result = vkEnumerateDeviceExtensionProperties( m_physicalDevice.GetVkPhysicalDevice(), nullptr, &deviceExtensionCount, nullptr );
		if( result != VK_SUCCESS )
		{
			DEBUG_LOG( LOG::ERRORLOG, "Failed to create logical device" );
			throw std::runtime_error( "Failed to create logical device!" );
		}

		m_deviceExtensions = std::vector<VkExtensionProperties>( deviceExtensionCount );
		result = vkEnumerateDeviceExtensionProperties( m_physicalDevice.GetVkPhysicalDevice(), nullptr, &deviceExtensionCount, m_deviceExtensions.data() );
		if( result != VK_SUCCESS )
		{
			DEBUG_LOG( LOG::ERRORLOG, "Failed to create logical device" );
			throw std::runtime_error( "Failed to create logical device!" );
		}

		// Display supported extensions
		if( m_deviceExtensions.size() > 0 )
		{
			DEBUG_LOG( LOG::INFO, "Device supports the following extensions:" );
			for( auto& extension : m_deviceExtensions )
			{
				DEBUG_LOG( LOG::INFO, "\t{}", std::string( extension.extensionName ) );
			}
		}

		// Check that extensions are supported before trying to create the device
		std::vector<const char*> unsupportedExtensions{};
		for( auto& extension : deviceConstructor.requestedExtensions )
		{
			if( IsExtensionSupported( extension.first ) )
			{
				m_enabledExtensions.emplace_back( extension.first );
			}
			else
			{
				unsupportedExtensions.emplace_back( extension.first );
			}
		}

		if( m_enabledExtensions.size() > 0 )
		{
			DEBUG_LOG( LOG::INFO, "Device supports the following requested extensions:" );
			for( auto& extension : m_enabledExtensions )
			{
				DEBUG_LOG( LOG::INFO, "\t{}", std::string( extension ) );
			}
		}

		if( unsupportedExtensions.size() > 0 )
		{
			auto error = false;
			for( auto& extension : unsupportedExtensions )
			{
				auto extensionIsOptional = deviceConstructor.requestedExtensions.at( extension );
				if( extensionIsOptional )
				{
					DEBUG_LOG( LOG::WARNING, "Optional device extension {} not available, some features may be disabled", extension );
				}
				else
				{
					DEBUG_LOG( LOG::WARNING, "Required device extension {} not available, cannot run", extension );
					error = true;
				}
			}

			if( error )
			{
				throw std::runtime_error( "Failed to create logical device: extensions not present" );
			}
		}

		VkDeviceCreateInfo createInfo{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };

		// Latest requested feature will have the pNext's all set up for device creation.
		// createInfo.pNext = m_physicalDevice.get_extension_feature_chain();

		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = static_cast<uint32_t>( queueCreateInfos.size() );
		createInfo.enabledExtensionCount = static_cast<uint32_t>( m_enabledExtensions.size() );
		createInfo.ppEnabledExtensionNames = m_enabledExtensions.data();

		const auto requestedGpuFeatures = m_physicalDevice.GetRequestedFeatures();
		createInfo.pEnabledFeatures = &requestedGpuFeatures;

		result = vkCreateDevice( m_physicalDevice.GetVkPhysicalDevice(), &createInfo, nullptr, &m_logicaldevice );

		if( result != VK_SUCCESS )
		{
			DEBUG_LOG( LOG::ERRORLOG, "Cannot create logical device!" );
			throw std::runtime_error( "Cannot create logical device!" );
		}

		m_queues.resize( queueFamilyPropertiesCount );

		for( uint32_t queueFamilyIndex = 0U; queueFamilyIndex < queueFamilyPropertiesCount; ++queueFamilyIndex )
		{
			const VkQueueFamilyProperties& queueFamilyProperty = m_physicalDevice.GetQueueFamilyProperties()[queueFamilyIndex];

			VkBool32 isPresentSupported = m_physicalDevice.IsPresentSupported( m_surface, queueFamilyIndex );

			for( uint32_t queueIndex = 0U; queueIndex < queueFamilyProperty.queueCount; ++queueIndex )
			{
				m_queues[queueFamilyIndex].emplace_back( Queue_Constructor( *this, queueFamilyIndex, queueFamilyProperty, isPresentSupported, queueIndex ) );
			}
		}

	}

	Device::~Device()
	{

	}

	bool Device::OnCreate()
	{

		return true;
	}

	void Device::OnDestroy()
	{
		if( m_logicaldevice != VK_NULL_HANDLE )
		{
			vkDestroyDevice( m_logicaldevice, nullptr );
		}
	}

	uint32_t Device::GetQueueFamilyIndex( VkQueueFlagBits queueFlags )
	{
		const auto& queueFamilyProperties = m_physicalDevice.GetQueueFamilyProperties();

		// Dedicated queue for compute
		// Try to find a queue family index that supports compute but not graphics
		if( queueFlags & VK_QUEUE_COMPUTE_BIT )
		{
			for( uint32_t i = 0; i < static_cast< uint32_t >( queueFamilyProperties.size() ); i++ )
			{
				if( ( queueFamilyProperties[i].queueFlags & queueFlags ) && !( queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT ) )
				{
					return i;
					break;
				}
			}
		}

		// Dedicated queue for transfer
		// Try to find a queue family index that supports transfer but not graphics and compute
		if( queueFlags & VK_QUEUE_TRANSFER_BIT )
		{
			for( uint32_t i = 0; i < static_cast< uint32_t >( queueFamilyProperties.size() ); i++ )
			{
				if( ( queueFamilyProperties[i].queueFlags & queueFlags ) && !( queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT ) && !( queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT ) )
				{
					return i;
					break;
				}
			}
		}

		// For other queue types or if no separate compute queue is present, return the first one to support the requested flags
		for( uint32_t i = 0; i < static_cast< uint32_t >( queueFamilyProperties.size() ); i++ )
		{
			if( queueFamilyProperties[i].queueFlags & queueFlags )
			{
				return i;
				break;
			}
		}

		throw std::runtime_error( "Could not find a matching queue family index" );
	}

	bool Device::IsExtensionSupported( const std::string& requestedExtension )
	{
		return std::find_if( m_deviceExtensions.begin(), m_deviceExtensions.end(),
			[requestedExtension] ( auto& device_extension )
		{
			return std::strcmp( device_extension.extensionName, requestedExtension.c_str() ) == 0;
		} ) != m_deviceExtensions.end();
	}

}