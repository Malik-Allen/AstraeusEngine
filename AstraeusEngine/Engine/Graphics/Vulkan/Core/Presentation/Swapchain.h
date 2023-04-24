// Copyright (c) 2023 Malik Allen

#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>
#include <set>

namespace Hephaestus
{
	class Device;
	class PhysicalDevice;

	struct SwapchainSupportDetails
	{
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct SwapchainProperties
	{
		VkSwapchainKHR                oldSwapchain;
		uint32_t                      imageCount{ 3 };
		VkExtent2D                    extent{};
		VkSurfaceFormatKHR            surfaceFormat{};
		uint32_t                      arrayLayers;
		VkImageUsageFlags             imageUsage;
		VkSurfaceTransformFlagBitsKHR preTransform;
		VkCompositeAlphaFlagBitsKHR   compositeAlpha;
		VkPresentModeKHR              presentMode;
	};

	struct Swapchain_Constructor
	{
		explicit Swapchain_Constructor( 
			Device& device,
			PhysicalDevice& physicalDevice,
			VkSurfaceKHR surface,
			const VkExtent2D& extent = {},
			const uint32_t imageCount = 3,
			const VkSurfaceTransformFlagBitsKHR transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
			const VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR,
			const std::set<VkImageUsageFlagBits>& imageUsageFlags = { VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_IMAGE_USAGE_TRANSFER_SRC_BIT }
		) :
			device( device ),
			physicalDevice( physicalDevice ),
			surface( surface ),
			extent( extent ),
			imageCount( imageCount ),
			transform( transform ),
			presentMode( presentMode ),
			imageUsageFlags( imageUsageFlags )
		{};

		Device& device;
		PhysicalDevice& physicalDevice;
		VkSurfaceKHR surface;
		const VkExtent2D extent;
		const uint32_t imageCount;
		const VkSurfaceTransformFlagBitsKHR transform;
		const VkPresentModeKHR presentMode;
		const std::set<VkImageUsageFlagBits> imageUsageFlags;
	};

	/*
	*	Swapchain, a collection of image buffers used for presenting images to a surface
	*/
	class Swapchain
	{
		Swapchain( const Swapchain& ) = delete;
		Swapchain& operator=( const Swapchain& ) = delete;
		Swapchain& operator=( Swapchain&& ) = delete;

	public:
		Swapchain( const Swapchain_Constructor& SwapchainConstructor );
		Swapchain( Swapchain&& other ) noexcept;
		~Swapchain();

	private:
		Device& m_device;
		PhysicalDevice& m_physicalDevice;
		VkSurfaceKHR m_surface;
		VkSwapchainKHR m_swapchain;
		SwapchainSupportDetails m_supportDetails;
		SwapchainProperties m_properties;

		// A list of present modes in order of priority (vector[0] has high priority, vector[size-1] has low priority)
		std::vector<VkPresentModeKHR> m_presentModePriorityList = {
			VK_PRESENT_MODE_FIFO_KHR,
			VK_PRESENT_MODE_MAILBOX_KHR
		};

		// A list of surface formats in order of priority (vector[0] has high priority, vector[size-1] has low priority)
		std::vector<VkSurfaceFormatKHR> m_surfaceFormatPriorityList = {
			{VK_FORMAT_R8G8B8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR},
			{VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR},
			{VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR},
			{VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}
		};

		std::set<VkImageUsageFlagBits> imageUsageFlags;

	};
}
