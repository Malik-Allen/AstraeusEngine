#pragma once

#include "../Instance.h"
#include "PhysicalDevice.h"

#include <vulkan/vulkan_core.h>
#include <memory>

namespace Hephaestus
{
	/*
	*
	*/
	struct Device_Constructor
	{
		explicit Device_Constructor( PhysicalDevice& _physicalDevice, VkSurfaceKHR _surface ) :
			physicalDevice( _physicalDevice ),
			surface( _surface )
		{};

		const PhysicalDevice& physicalDevice;
		VkSurfaceKHR surface;
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
		Device( const Device_Constructor& deviceConstructor );
		~Device();

		bool OnCreate();
		void OnDestroy();

		inline const VkDevice& GetVkDevice() const
		{
			return m_logicaldevice;
		}

	private:
		VkDevice m_logicaldevice;
		const PhysicalDevice& m_physicalDevice;
		VkSurfaceKHR m_surface;

	};
}