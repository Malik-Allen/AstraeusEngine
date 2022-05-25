// Copyright (c) 2022 Malik Allen

#include "Instance.h"
#include "Devices/PhysicalDevice.h"

#include <DebugLog.h>

// TODO: get rid of GLFW dependency
#include <GLFW/glfw3.h>
#include <iostream>
#include <assert.h>

namespace Hephaestus
{

	bool enable_extension( const char* required_ext_name,
		const std::vector<VkExtensionProperties>& available_exts,
		std::vector<const char*>& enabled_extensions )
	{
		for( auto& avail_ext_it : available_exts )
		{
			if( strcmp( avail_ext_it.extensionName, required_ext_name ) == 0 )
			{
				auto it = std::find_if( enabled_extensions.begin(), enabled_extensions.end(),
					[required_ext_name] ( const char* enabled_ext_name )
				{
					return strcmp( enabled_ext_name, required_ext_name ) == 0;
				} );
				if( it != enabled_extensions.end() )
				{
					// Extension is already enabled
				}
				else
				{
					DEBUG_LOG( LOG::INFO, "Extension {} found, enabling it", std::string( required_ext_name ) );
					enabled_extensions.emplace_back( required_ext_name );
				}
				return true;
			}
		}
		DEBUG_LOG( LOG::WARNING, "Extension {} not found", std::string( required_ext_name ) );
		return false;
	}

	Instance::Instance( const Instance_Constructor& instanceConstructor ) :
		m_vkInstance( VK_NULL_HANDLE )
	{
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = instanceConstructor.applicationName.c_str();
		appInfo.pEngineName = instanceConstructor.engineName.c_str();
		appInfo.apiVersion = instanceConstructor.version;

		uint32_t instanceExtensionCount;
		VkResult result;
		result = vkEnumerateInstanceExtensionProperties( nullptr, &instanceExtensionCount, nullptr );
		if( result != VK_SUCCESS )
		{
			DEBUG_LOG( LOG::ERRORLOG, "Failed to create instance: could not obtain instance extension count!" );
			throw std::runtime_error( "Failed to create instance: could not obtain instance extension count!" );
		}

		// Getting available extensions on the Vulkan Instance
		std::vector<VkExtensionProperties> availableInstanceExtensions( instanceExtensionCount );
		result = vkEnumerateInstanceExtensionProperties( nullptr, &instanceExtensionCount, availableInstanceExtensions.data() );
		if( result != VK_SUCCESS )
		{
			DEBUG_LOG( LOG::ERRORLOG, "Failed to create instance: could not obtain instance extension properties!" );
			throw std::runtime_error( "Failed to create instance: could not obtain instance extension properties!" );
		}

		if( availableInstanceExtensions.size() > 0 )
		{
			DEBUG_LOG( LOG::INFO, "Instance supports the following extensions:" );
			for( auto& extension : availableInstanceExtensions )
			{
				DEBUG_LOG( LOG::INFO, "\t{}", std::string( extension.extensionName ) );
			}
		}

		// Mandatory surface extension
		m_enabledExtensions.push_back( VK_KHR_SURFACE_EXTENSION_NAME );

		// Enabling required extensions provided to the instance
		auto extensionError = false;
		for( auto extension : instanceConstructor.requiredExtensions )
		{
			auto extensionName = extension.first;
			auto extensionIsOptional = extension.second;
			if( !enable_extension( extensionName, availableInstanceExtensions, m_enabledExtensions ) )
			{
				if( extensionIsOptional )
				{
					DEBUG_LOG( LOG::WARNING, "Optional instance extension {} not available, some features may be disabled", std::string( extensionName ) );
				}
				else
				{
					DEBUG_LOG( LOG::ERRORLOG, "Required instance extension {} not available, cannot run", std::string( extensionName ) );
					extensionError = true;
				}
				extensionError = extensionError || !extensionIsOptional;
			}
		}

		if( extensionError )
		{
			DEBUG_LOG( LOG::ERRORLOG, "Failed to create instance: Required instance extensions are missing." );
			throw std::runtime_error( "Required instance extensions are missing." );
		}

		// add more extensions as need to the instanceExtensions vector<>

		VkInstanceCreateInfo instanceCreateInfo = {};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pNext = NULL;
		instanceCreateInfo.pApplicationInfo = &appInfo;
		if( m_enabledExtensions.size() > 0 )
		{
			if( instanceConstructor.enableValidationLayers )
			{
				m_enabledExtensions.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
			}
			instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>( m_enabledExtensions.size() );
			instanceCreateInfo.ppEnabledExtensionNames = m_enabledExtensions.data();
		}

		// TODO: Revisit on debugger/ debug validation layer update to Vulkan framework
		if( instanceConstructor.enableValidationLayers )
		{
			// The VK_LAYER_KHRONOS_validation contains all current validation functionality.
			// Note that on Android this layer requires at least NDK r20
			const char* validationLayerName = "VK_LAYER_KHRONOS_validation";
			// Check if this layer is available at instance level
			uint32_t instanceLayerCount;
			vkEnumerateInstanceLayerProperties( &instanceLayerCount, nullptr );
			std::vector<VkLayerProperties> instanceLayerProperties( instanceLayerCount );
			vkEnumerateInstanceLayerProperties( &instanceLayerCount, instanceLayerProperties.data() );
			bool validationLayerPresent = false;
			for( VkLayerProperties layer : instanceLayerProperties )
			{
				if( strcmp( layer.layerName, validationLayerName ) == 0 )
				{
					validationLayerPresent = true;
					break;
				}
			}
			if( validationLayerPresent )
			{
				instanceCreateInfo.ppEnabledLayerNames = &validationLayerName;
				instanceCreateInfo.enabledLayerCount = 1;
			}
			else
			{
				std::cerr << "Validation layer VK_LAYER_KHRONOS_validation not present, validation is disabled";
			}
		}

		if( vkCreateInstance( &instanceCreateInfo, nullptr, &m_vkInstance ) != VK_SUCCESS )
		{
			DEBUG_LOG( LOG::ERRORLOG, "Failed to create Vulkan instance" );
			throw std::runtime_error( "Failed to create Vulkan instance" );
		}

		FindGPUs();
	}

	Instance::~Instance()
	{}

	bool Instance::OnCreate()
	{
		return true;
	}

	void Instance::OnDestroy()
	{
		vkDestroyInstance( m_vkInstance, nullptr );
	}

	void Instance::FindGPUs()
	{
		// Querying valid physical devices on the machine
		uint32_t physical_device_count{ 0 };
		if( vkEnumeratePhysicalDevices( m_vkInstance, &physical_device_count, nullptr ) != VK_SUCCESS )
		{
			DEBUG_LOG( LOG::ERRORLOG, "ERROR! TODO: Implement something like VK_CHECK from vulkan_samples if you want more information and throw a run_time error" );
			throw std::runtime_error( "Failed to select a physical device!" );
		}

		if( physical_device_count < 1 )
		{
			throw std::runtime_error( "Couldn't find a physical device that supports Vulkan." );
		}

		std::vector<VkPhysicalDevice> physical_devices;
		physical_devices.resize( physical_device_count );

		if( vkEnumeratePhysicalDevices( m_vkInstance, &physical_device_count, physical_devices.data() ) != VK_SUCCESS )
		{
			DEBUG_LOG( LOG::ERRORLOG, "ERROR! TODO: Implement something like VK_CHECK from vulkan_samples if you want more information and throw a run_time error" );
			throw std::runtime_error( "Failed to register vulkan physical devices!" );
		}

		// Create gpus wrapper objects from the VkPhysicalDevice's
		for( auto& physical_device : physical_devices )
		{
			m_gpus.push_back( std::make_unique<PhysicalDevice>( PhysicalDevice_Constructor( *this, physical_device ) ) );
		}
	}

	PhysicalDevice& Instance::GetSuitableGPU( VkSurfaceKHR surface )
	{
		assert( !m_gpus.empty() && "No physical devices were found on the system." );

		// Find a discrete GPU
		for( auto& gpu : m_gpus )
		{
			if( gpu->GetProperties().deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU )
			{
				//See if it work with the surface
				size_t queue_count = gpu->GetQueueFamilyProperties().size();
				for( uint32_t queue_idx = 0; static_cast< size_t >( queue_idx ) < queue_count; queue_idx++ )
				{
					if( gpu->IsPresentSupported( surface, queue_idx ) )
					{
						return *gpu;
					}
				}
			}
		}

		// Otherwise just pick the first one
		DEBUG_LOG( LOG::WARNING, "Couldn't find a discrete physical device, picking default GPU" );

		return *m_gpus.at( 0 );
	}

	PhysicalDevice& Instance::GetFirstGPU()
	{
		assert( !m_gpus.empty() && "No physical devices were found on the system." );

		// Find a discrete GPU
		for( auto& gpu : m_gpus )
		{
			if( gpu->GetProperties().deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU )
			{
				return *gpu;
			}
		}

		// Otherwise just pick the first one
		DEBUG_LOG( LOG::WARNING, "Couldn't find a discrete physical device, picking default GPU" );
		return *m_gpus.at( 0 );
	}

}

