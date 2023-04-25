// Copyright (c) 2023 Malik Allen

#include "ImageView.h"
#include "../Initialization/Device.h"

#include <DebugLog.h>


namespace Hephaestus
{
	inline bool IsDepthOnlyformat( VkFormat format )
	{
		return format == VK_FORMAT_D16_UNORM ||
			format == VK_FORMAT_D32_SFLOAT;
	}

	inline bool IsDepthStencilFormat( VkFormat format )
	{
		return format == VK_FORMAT_D16_UNORM_S8_UINT ||
			format == VK_FORMAT_D24_UNORM_S8_UINT ||
			format == VK_FORMAT_D32_SFLOAT_S8_UINT ||
			IsDepthOnlyformat( format );
	}

	ImageView::ImageView( const ImageView_Constructor& imageViewConstructor ) :
		m_device( imageViewConstructor.device ),
		m_imageView( VK_NULL_HANDLE ),
		m_image( &imageViewConstructor.image ),
		m_format( imageViewConstructor.format )
	{
		if( m_format == VK_FORMAT_UNDEFINED )
		{
			// TODO: m_format = m_image->GetFormat();
		}

		m_subresourceRange.baseMipLevel = imageViewConstructor.baseMipLevel;
		m_subresourceRange.baseArrayLayer = imageViewConstructor.baseArrayLayer;
		m_subresourceRange.levelCount = imageViewConstructor.mipLevels /*TODO: == 0 ? m_image->GetSubresource().mipLevel : imageViewConstructor.mipLevels*/;
		m_subresourceRange.layerCount = imageViewConstructor.arrayLayers /*TODO: == 0 ? m_image->GetSubresource().arrayLayer : imageViewConstructor.arrayLayers*/;

		if( IsDepthStencilFormat( imageViewConstructor.format ) )
		{
			m_subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		}
		else
		{
			m_subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}

		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		// TODO: createInfo.image = m_image->GetVkImage();
		createInfo.viewType = imageViewConstructor.viewType;
		createInfo.format = imageViewConstructor.format;
		createInfo.subresourceRange = m_subresourceRange;

		VkResult result = vkCreateImageView( m_device.GetVkDevice(), &createInfo, nullptr, &m_imageView );
		if( result != VK_SUCCESS )
		{
			DEBUG_LOG( LOG::ERRORLOG, "Failed to create image view!" );
			throw std::runtime_error( "Failed to create image view!" );
		}
	}

	ImageView::ImageView( ImageView&& other ) noexcept :
		m_device( other.m_device ),
		m_imageView( other.m_imageView ),
		m_image( other.m_image ),
		m_format( other.m_format ),
		m_subresourceRange( other.m_subresourceRange )
	{
		other.m_imageView = VK_NULL_HANDLE;
	}

	ImageView::~ImageView()
	{
		if( m_imageView != VK_NULL_HANDLE )
		{
			vkDestroyImageView( m_device.GetVkDevice(), m_imageView, nullptr);
		}
	}
}