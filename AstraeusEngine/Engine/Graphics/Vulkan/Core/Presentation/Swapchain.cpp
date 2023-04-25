// Copyright (c) 2023 Malik Allen

#include "Swapchain.h"
#include "../Initialization/PhysicalDevice.h"
#include "../Initialization/Device.h"
#include "../Utilities/Helpers/Helpers.h"

#include <vulkan/vk_enum_string_helper.h>
#include <DebugLog.h>

namespace Hephaestus
{
	inline uint32_t ChooseImageCount(
		uint32_t requestImageCount,
		uint32_t minImageCount,
		uint32_t maxImageCount )
	{
		if( maxImageCount != 0 )
		{
			requestImageCount = std::min( requestImageCount, maxImageCount );
		}

		requestImageCount = std::max( requestImageCount, minImageCount );

		return requestImageCount;
	}

	inline VkExtent2D ChooseExtent(
		VkExtent2D        requestExtent,
		const VkExtent2D& minImageExtent,
		const VkExtent2D& maxImageExtent,
		const VkExtent2D& currentExtent )
	{
		if( currentExtent.width == 0xFFFFFFFF )
		{
			return requestExtent;
		}

		if( requestExtent.width < 1 || requestExtent.height < 1 )
		{
			DEBUG_LOG( LOG::WARNING, "Image extent ({}, {}) not supported. Selecting ({}, {}).", requestExtent.width, requestExtent.height, currentExtent.width, currentExtent.height );
			return currentExtent;
		}

		requestExtent.width = std::max( requestExtent.width, minImageExtent.width );
		requestExtent.width = std::min( requestExtent.width, maxImageExtent.width );

		requestExtent.height = std::max( requestExtent.height, minImageExtent.height );
		requestExtent.height = std::min( requestExtent.height, maxImageExtent.height );

		return requestExtent;
	}

	inline uint32_t ChooseImageArrayLayers(
		uint32_t requestImageArrayLayers,
		uint32_t maxImageArrayLayers )
	{
		requestImageArrayLayers = std::min( requestImageArrayLayers, maxImageArrayLayers );
		requestImageArrayLayers = std::max( requestImageArrayLayers, 1u );

		return requestImageArrayLayers;
	}

	inline VkSurfaceFormatKHR ChooseSurfaceFormat(
		const VkSurfaceFormatKHR               requestedSurfaceFormat,
		const std::vector<VkSurfaceFormatKHR>& availableSurfaceFormats,
		const std::vector<VkSurfaceFormatKHR>& surfaceFormatPriorityList )
	{
		// Try to find the requested surface format in the supported surface formats
		auto surfaceFormatIt = std::find_if(
			availableSurfaceFormats.begin(),
			availableSurfaceFormats.end(),
			[&requestedSurfaceFormat] ( const VkSurfaceFormatKHR& surface )
		{
			if( surface.format == requestedSurfaceFormat.format &&
				surface.colorSpace == requestedSurfaceFormat.colorSpace )
			{
				return true;
			}

			return false;
		} );

		// If the requested surface format isn't found, then try to request a format from the priority list
		if( surfaceFormatIt == availableSurfaceFormats.end() )
		{
			for( auto& surfaceFormat : surfaceFormatPriorityList )
			{
				surfaceFormatIt = std::find_if(
					availableSurfaceFormats.begin(),
					availableSurfaceFormats.end(),
					[&surfaceFormat] ( const VkSurfaceFormatKHR& surface )
				{
					if( surface.format == surfaceFormat.format &&
						surface.colorSpace == surfaceFormat.colorSpace )
					{
						return true;
					}

					return false;
				} );
				if( surfaceFormatIt != availableSurfaceFormats.end() )
				{
					DEBUG_LOG( LOG::WARNING, "Surface format ({}) not supported. Selecting ({}).", string_VkFormat( requestedSurfaceFormat.format ), string_VkFormat( ( *surfaceFormatIt ).format ) );
					return *surfaceFormatIt;
				}
			}

			// If nothing found, default the first supporte surface format
			surfaceFormatIt = availableSurfaceFormats.begin();
			DEBUG_LOG( LOG::WARNING, "Surface format ({}) not supported. Selecting ({}).", string_VkFormat( requestedSurfaceFormat.format ), string_VkFormat( ( *surfaceFormatIt ).format ) );
		}
		else
		{
			DEBUG_LOG( LOG::INFO, "Surface format selected: {}", string_VkFormat( requestedSurfaceFormat.format ) );
		}

		return *surfaceFormatIt;
	}

	inline bool ValidateFormatFeature( VkImageUsageFlagBits imageUsage, VkFormatFeatureFlags supportedFeatures )
	{
		switch( imageUsage )
		{
		case VK_IMAGE_USAGE_STORAGE_BIT:
			return VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT & supportedFeatures;
		default:
			return true;
		}
	}

	inline std::set<VkImageUsageFlagBits> ChooseImageUsage(
		const std::set<VkImageUsageFlagBits>& requestedImageUsageFlags,
		VkImageUsageFlags supportedImageUsage,
		VkFormatFeatureFlags supportedFeatures )
	{
		std::set<VkImageUsageFlagBits> validatedImageUsageFlags;
		for( auto flag : requestedImageUsageFlags )
		{
			if( ( flag & supportedImageUsage ) && ValidateFormatFeature( flag, supportedFeatures ) )
			{
				validatedImageUsageFlags.insert( flag );
			}
			else
			{
				DEBUG_LOG( LOG::WARNING, "Image usage ({}) requested but not supported.", string_VkImageUsageFlagBits( flag ) );
			}
		}

		if( validatedImageUsageFlags.empty() )
		{
			// Pick the first format from list of defaults, if supported
			static const std::vector<VkImageUsageFlagBits> imageUsageFlags = {
				VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
				VK_IMAGE_USAGE_STORAGE_BIT,
				VK_IMAGE_USAGE_SAMPLED_BIT,
				VK_IMAGE_USAGE_TRANSFER_DST_BIT };

			for( VkImageUsageFlagBits imageUsage : imageUsageFlags )
			{
				if( ( imageUsage & supportedImageUsage ) && ValidateFormatFeature( imageUsage, supportedFeatures ) )
				{
					validatedImageUsageFlags.insert( imageUsage );
					break;
				}
			}
		}

		if( !validatedImageUsageFlags.empty() )
		{
			// Log image usage flags used
			std::string usage_list;
			for( VkImageUsageFlagBits imageUsage : validatedImageUsageFlags )
			{
				usage_list += string_VkImageUsageFlagBits( imageUsage ) + std::string( " " );
			}
			DEBUG_LOG( LOG::INFO, "Image usage flags: {}", usage_list );
		}
		else
		{
			DEBUG_LOG( LOG::ERRORLOG, "No compatible image usage found." );
			throw std::runtime_error( "No compatible image usage found." );
		}

		return validatedImageUsageFlags;
	}

	inline VkImageUsageFlags CompositeImageFlags( std::set<VkImageUsageFlagBits>& imageUsageFlags )
	{
		VkImageUsageFlags imageUsage{};
		for( auto flag : imageUsageFlags )
		{
			imageUsage |= flag;
		}
		return imageUsage;
	}

	inline VkSurfaceTransformFlagBitsKHR ChooseTransform(
		VkSurfaceTransformFlagBitsKHR requestTransform,
		VkSurfaceTransformFlagsKHR    supportedTransform,
		VkSurfaceTransformFlagBitsKHR currentTransform )
	{
		if( requestTransform & supportedTransform )
		{
			return requestTransform;
		}

		DEBUG_LOG( LOG::ERRORLOG, "Surface transform '{}' not supported. Selecting '{}'.", string_VkSurfaceTransformFlagBitsKHR( requestTransform ), string_VkSurfaceTransformFlagsKHR( currentTransform ) );

		return currentTransform;
	}

	inline VkCompositeAlphaFlagBitsKHR ChooseCompositeAlpha( VkCompositeAlphaFlagBitsKHR requestCompositeAlpha, VkCompositeAlphaFlagsKHR supportedCompositeAlpha )
	{
		if( requestCompositeAlpha & supportedCompositeAlpha )
		{
			return requestCompositeAlpha;
		}

		static const std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphaFlags = {
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR
		};

		for( VkCompositeAlphaFlagBitsKHR compositeAlpha : compositeAlphaFlags )
		{
			if( compositeAlpha & supportedCompositeAlpha )
			{
				DEBUG_LOG( LOG::WARNING, "Composite alpha '{}' not supported. Selecting '{}.", string_VkCompositeAlphaFlagBitsKHR( requestCompositeAlpha ), string_VkCompositeAlphaFlagBitsKHR( compositeAlpha ) );
				return compositeAlpha;
			}
		}

		DEBUG_LOG( LOG::ERRORLOG, "No compatible composite alpha found." );
		throw std::runtime_error( "No compatible composite alpha found." );
	}

	Swapchain::Swapchain( const Swapchain_Constructor& swapchainConstructor ) :
		m_device( swapchainConstructor.device ),
		m_physicalDevice( swapchainConstructor.physicalDevice ),
		m_surface( swapchainConstructor.surface ),
		m_swapchain( VK_NULL_HANDLE )
	{
		VkSurfaceCapabilitiesKHR surfaceCapabilities{};
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR( m_physicalDevice.GetVkPhysicalDevice(), m_surface, &surfaceCapabilities );

		VkResult result;

		uint32_t surfaceFormatCount{ 0U };
		result = vkGetPhysicalDeviceSurfaceFormatsKHR( m_physicalDevice.GetVkPhysicalDevice(), m_surface, &surfaceFormatCount, nullptr );
		if( result != VK_SUCCESS )
		{
			DEBUG_LOG( LOG::ERRORLOG, "Failed to get surface format count!" );
			throw std::runtime_error( "Failed to get surface format count!" );
		}

		m_supportDetails.formats.resize( surfaceFormatCount );
		result = vkGetPhysicalDeviceSurfaceFormatsKHR( m_physicalDevice.GetVkPhysicalDevice(), m_surface, &surfaceFormatCount, m_supportDetails.formats.data() );
		if( result != VK_SUCCESS )
		{
			DEBUG_LOG( LOG::ERRORLOG, "Failed to get surface formats!" );
			throw std::runtime_error( "Failed to get surface formats!" );
		}

		DEBUG_LOG( LOG::INFO, "Surface supports the following surface formats:" );
		for( auto& surfaceFormat : m_supportDetails.formats )
		{
			DEBUG_LOG( LOG::INFO, "\t{}, {}", string_VkFormat( surfaceFormat.format ), string_VkColorSpaceKHR( surfaceFormat.colorSpace ) );
		}

		uint32_t presentModeCount{ 0U };
		result = vkGetPhysicalDeviceSurfacePresentModesKHR( m_physicalDevice.GetVkPhysicalDevice(), m_surface, &presentModeCount, nullptr );
		if( result != VK_SUCCESS )
		{
			DEBUG_LOG( LOG::ERRORLOG, "Failed to get surface present mode count!" );
			throw std::runtime_error( "Failed to get surface present mode count!" );
		}

		m_supportDetails.presentModes.resize( presentModeCount );
		result = vkGetPhysicalDeviceSurfacePresentModesKHR( m_physicalDevice.GetVkPhysicalDevice(), m_surface, &presentModeCount, m_supportDetails.presentModes.data() );
		if( result != VK_SUCCESS )
		{
			DEBUG_LOG( LOG::ERRORLOG, "Failed to get surface present modes!" );
			throw std::runtime_error( "Failed to get surface present modes!" );
		}

		DEBUG_LOG( LOG::INFO, "Surface supports the following present modes:" );
		for( auto& presentMode : m_supportDetails.presentModes )
		{
			DEBUG_LOG( LOG::INFO, "\t{}", string_VkPresentModeKHR( presentMode ) );
		}

		// Choose the best properties based on the sufrace capabilities
		m_properties.imageCount = ChooseImageCount( swapchainConstructor.imageCount, surfaceCapabilities.minImageCount, surfaceCapabilities.maxImageCount );
		m_properties.extent = ChooseExtent( swapchainConstructor.extent, surfaceCapabilities.minImageExtent, surfaceCapabilities.maxImageExtent, surfaceCapabilities.currentExtent );
		m_properties.arrayLayers = ChooseImageArrayLayers( 1U, surfaceCapabilities.maxImageArrayLayers );
		m_properties.surfaceFormat = ChooseSurfaceFormat( m_properties.surfaceFormat, m_supportDetails.formats, m_surfaceFormatPriorityList );

		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties( m_physicalDevice.GetVkPhysicalDevice(), m_properties.surfaceFormat.format, &formatProperties );
		imageUsageFlags = ChooseImageUsage( swapchainConstructor.imageUsageFlags, surfaceCapabilities.supportedUsageFlags, formatProperties.optimalTilingFeatures );
		m_properties.imageUsage = CompositeImageFlags( imageUsageFlags );
		m_properties.preTransform = ChooseTransform( swapchainConstructor.transform, surfaceCapabilities.supportedTransforms, surfaceCapabilities.currentTransform );
		m_properties.compositeAlpha = ChooseCompositeAlpha( VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR, surfaceCapabilities.supportedCompositeAlpha );
		m_properties.presentMode = swapchainConstructor.presentMode;


		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_surface;

		createInfo.minImageCount = m_properties.imageCount;
		createInfo.imageExtent = m_properties.extent;
		createInfo.presentMode = m_properties.presentMode;
		createInfo.imageFormat = m_properties.surfaceFormat.format;
		createInfo.imageColorSpace = m_properties.surfaceFormat.colorSpace;
		createInfo.imageArrayLayers = m_properties.arrayLayers;
		createInfo.imageUsage = m_properties.imageUsage;
		createInfo.preTransform = m_properties.preTransform;
		createInfo.compositeAlpha = m_properties.compositeAlpha;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		result = vkCreateSwapchainKHR( m_device.GetVkDevice(), &createInfo, nullptr, &m_swapchain );
		if( result != VK_SUCCESS )
		{
			DEBUG_LOG( LOG::ERRORLOG, "Failed to create swapchain!" );
			throw std::runtime_error( "Failed to create swapchain!" );
		}
	}

	Swapchain::Swapchain( Swapchain&& other ) noexcept :
		m_device( other.m_device ),
		m_physicalDevice( other.m_physicalDevice ),
		m_surface( other.m_surface ),
		m_swapchain( other.m_swapchain ),
		m_properties( other.m_properties )
	{
		other.m_swapchain = VK_NULL_HANDLE;
		other.m_surface = VK_NULL_HANDLE;
	}

	Swapchain::~Swapchain()
	{
		if( m_swapchain != VK_NULL_HANDLE )
		{
			vkDestroySwapchainKHR( m_device.GetVkDevice(), m_swapchain, nullptr );
		}
	}
}