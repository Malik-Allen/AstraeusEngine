#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

namespace Hephaestus
{

	struct InstanceConstructor
	{
		InstanceConstructor() :
			applicationName( "" ),
			engineName( "" ),
			version( 0 ),
			enableValidationLayers( false )
		{};

		InstanceConstructor(const std::string& _applicationName, const std::string& _engineName,
			const int _version, const bool _enableValidationLayers ) :
			applicationName( _applicationName ),
			engineName( _engineName ),
			version( _version ),
			enableValidationLayers( _enableValidationLayers )
		{}

		std::string applicationName;
		std::string engineName;
		int version;
		bool enableValidationLayers;
	};


	/*
	*	
	*/
	class Instance
	{
		Instance( const Instance& ) = delete;
		Instance& operator=( const Instance& ) = delete;
		Instance( Instance&& ) = delete;
		Instance& operator=( Instance&& ) = delete;
	public:
		Instance();
		~Instance();

		VkResult OnCreate( const InstanceConstructor& instanceConstructor );
		void OnDestroy();

		inline const VkInstance& GetVkInstance() const
		{
			return m_vkInstance;
		}

	private:
		VkInstance m_vkInstance;
	};

};