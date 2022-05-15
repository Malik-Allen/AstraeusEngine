#pragma once

#include "../Instance.h"
#include "PhysicalDevice.h"

#include <vulkan/vulkan.h>
#include <memory>

namespace Hephaestus
{
	/*
	* 
	*/
	struct DeviceConstructor
	{
		explicit DeviceConstructor( PhysicalDevice& _physicalDevice ) :
			physicalDevice( _physicalDevice )
		{};

		const PhysicalDevice& physicalDevice;
	};

	/*
	* 
	*/
	class Device
	{
		Device( const Device& ) = delete;
		Device& operator=( const Device& ) = delete;
		Device( Device&& ) = delete;
		Device& operator=( Device&& ) = delete;
	public:
		Device();
		~Device();

		bool OnCreate( const DeviceConstructor& deviceConstructor );
		void OnDestroy();

	private:
		VkDevice m_logicaldevice;
		PhysicalDevice* m_physicalDevice;
	};
}