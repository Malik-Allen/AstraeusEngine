#include "Instance.h"

#include <GLFW/glfw3.h>
#include <iostream>

namespace Hephaestus
{
	Instance::Instance() :
		m_vkInstance(VK_NULL_HANDLE)
	{}

	Instance::~Instance()
	{}

	VkResult Instance::OnCreate( const InstanceInfo & instanceInfo )
	{
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = instanceInfo.applicationName.c_str();
		appInfo.pEngineName = instanceInfo.engineName.c_str();
		appInfo.apiVersion = instanceInfo.version;

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
			if( instanceInfo.enableValidationLayers )
			{
				instanceExtensions.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
			}
			instanceCreateInfo.enabledExtensionCount = ( uint32_t )instanceExtensions.size();
			instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
		}

		if( instanceInfo.enableValidationLayers )
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
		return vkCreateInstance( &instanceCreateInfo, nullptr, &m_vkInstance );
	}

	void Instance::OnDestroy()
	{
		vkDestroyInstance( m_vkInstance, nullptr );
	}

}

