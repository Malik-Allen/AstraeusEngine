// Copyright (c) 2023 Malik Allen

#pragma once

#include <vulkan/vulkan_core.h>

namespace Hephaestus
{
	class Image;
	class Device;

	struct ImageView_Constructor
	{
		explicit ImageView_Constructor(
			Device& device,
			Image& image,
			VkImageViewType viewType,
			VkFormat format = VK_FORMAT_UNDEFINED,
			uint32_t baseMipLevel = 0,
			uint32_t baseArrayLayer = 0,
			uint32_t mipLevels = 0,
			uint32_t arrayLayers = 0
		) :
			device( device ),
			image( image ),
			viewType( viewType ),
			format( format ),
			baseMipLevel( baseMipLevel ),
			baseArrayLayer( baseArrayLayer ),
			mipLevels( mipLevels ),
			arrayLayers( arrayLayers )
		{};

		Device& device;
		Image& image;
		VkImageViewType viewType;
		VkFormat format;
		uint32_t baseMipLevel;
		uint32_t baseArrayLayer;
		uint32_t mipLevels;
		uint32_t arrayLayers;
	};

	/**
	 * @brief The ImageView is used as a set a paramters that can be used to look at a set of data in a particular way, format, dimensionality, and range of mipmap levels and array levels
	*/
	class ImageView
	{
		ImageView( const ImageView& ) = delete;
		ImageView& operator=( const ImageView& ) = delete;
		ImageView& operator=( ImageView&& ) = delete;

	public:
		ImageView( const ImageView_Constructor& ImageViewConstructor );
		ImageView( ImageView&& other ) noexcept;
		~ImageView();

	private:
		Device& m_device;
		VkImageView m_imageView;
		Image* m_image;
		VkFormat m_format;
		VkImageSubresourceRange m_subresourceRange{};
	};
}