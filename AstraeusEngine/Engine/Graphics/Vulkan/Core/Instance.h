#pragma once

#include <vulkan/vulkan_core.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace Hephaestus
{
	class PhysicalDevice;

	/*
	*
	*/
	struct Instance_Constructor
	{
		Instance_Constructor() :
			applicationName( "" ),
			engineName( "" ),
			version( 0 ),
			enableValidationLayers( false )
		{};

		Instance_Constructor( const std::string& _applicationName, const std::string& _engineName,
			const int _version, const bool _enableValidationLayers,
			const std::unordered_map<const char*, bool>& _requiredExtensions = {} ) :
			applicationName( _applicationName ),
			engineName( _engineName ),
			version( _version ),
			enableValidationLayers( _enableValidationLayers ),
			requiredExtensions( _requiredExtensions )
		{}

		std::string applicationName;
		std::string engineName;
		int version;
		bool enableValidationLayers;
		std::unordered_map<const char*, bool> requiredExtensions;
	};


	/*
	*	Wrapper class for VkInstance:
	*	Represents the connection from your application to the Vulkan runtime and therefore only should exist once in your application
	*/
	class Instance
	{
		Instance( const Instance& ) = delete;
		Instance& operator=( const Instance& ) = delete;
		Instance( Instance&& ) = delete;
		Instance& operator=( Instance&& ) = delete;
	public:
		Instance( const Instance_Constructor& instanceConstructor );
		~Instance();

		bool OnCreate();
		void OnDestroy();

		inline const VkInstance& GetVkInstance() const
		{
			return m_vkInstance;
		}

		/*
		* @brief Tries to find the first available discrete GPU that can render to the given surface
		* @param surface to test against
		* @returns A valid physical device
		*/
		PhysicalDevice& GetSuitableGPU( VkSurfaceKHR surface );

		/**
		* @brief Tries to find the first available discrete GPU
		* @returns A valid physical device
		*/
		PhysicalDevice& GetFirstGPU();

	private:
		VkInstance m_vkInstance;
		/* @brief The physical devices found on the machine*/
		std::vector<std::unique_ptr<PhysicalDevice>> m_gpus;
		/* @brief The enabled extensions*/
		std::vector<const char*> m_enabledExtensions;

		/*
		*	@brief Searches the instance for GPUs on the machine, stores them as physical devices
		*/
		void FindGPUs();

		
	};

};