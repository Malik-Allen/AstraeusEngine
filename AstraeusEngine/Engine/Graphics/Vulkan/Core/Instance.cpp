#include "Instance.h"
#include "Devices/PhysicalDevice.h"

#include <DebugLog.h>

// TODO: get rid of GLFW dependency
#include <GLFW/glfw3.h>
#include <iostream>
#include <assert.h>

namespace Hephaestus
{
	Instance::Instance( const Instance_Constructor& instanceConstructor ) :
		m_vkInstance( VK_NULL_HANDLE )
	{
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = instanceConstructor.applicationName.c_str();
		appInfo.pEngineName = instanceConstructor.engineName.c_str();
		appInfo.apiVersion = instanceConstructor.version;

		// TODO: Look at how I can get rid of this dependency to GLFW below
		// Enable extensions required by GLFW
		uint32_t extensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions( &extensionCount );

		std::vector<const char*> instanceExtensions( glfwExtensions, glfwExtensions + extensionCount );

		// add more extensions as need to the instanceExtensions vector<>

		VkInstanceCreateInfo instanceCreateInfo = {};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pNext = NULL;
		instanceCreateInfo.pApplicationInfo = &appInfo;
		if( instanceExtensions.size() > 0 )
		{
			if( instanceConstructor.enableValidationLayers )
			{
				instanceExtensions.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
			}
			instanceCreateInfo.enabledExtensionCount = ( uint32_t )instanceExtensions.size();
			instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
		}

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
			CONSOLE_LOG( LOG::ERRORLOG, "Failed to create Vulkan instance" );
			throw std::runtime_error( "Failed to create Vulkan instance" );
		}

		FindGPUs();
	}

	Instance::~Instance()
	{}

	bool Instance::OnCreate(  )
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
			CONSOLE_LOG( LOG::ERRORLOG, "ERROR! TODO: Implement something like VK_CHECK from vulkan_samples if you want more information and throw a run_time error" );
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
			CONSOLE_LOG( LOG::ERRORLOG, "ERROR! TODO: Implement something like VK_CHECK from vulkan_samples if you want more information and throw a run_time error" );
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
		CONSOLE_LOG( LOG::WARNING, "Couldn't find a discrete physical device, picking default GPU" );

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
		CONSOLE_LOG( LOG::WARNING, "Couldn't find a discrete physical device, picking default GPU" );
		return *m_gpus.at( 0 );
	}

}

