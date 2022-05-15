#pragma once

#include <vulkan/vulkan.h>

namespace Hephaestus
{
	class PhysicalDevice
	{
		PhysicalDevice( const PhysicalDevice& ) = delete;
		PhysicalDevice& operator=( const PhysicalDevice& ) = delete;
		PhysicalDevice( PhysicalDevice&& ) = delete;
		PhysicalDevice& operator=( PhysicalDevice&& ) = delete;
	public:
		PhysicalDevice();
		~PhysicalDevice();

	private:

	};

}