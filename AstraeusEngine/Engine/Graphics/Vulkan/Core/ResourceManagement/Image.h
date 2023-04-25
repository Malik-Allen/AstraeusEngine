// Copyright (c) 2023 Malik Allen

#pragma once

namespace Hephaestus
{
	struct Image_Constructor
	{
		explicit Image_Constructor()
		{};
	};

	/**
	 * @brief 
	*/
	class Image
	{
		Image( const Image& ) = delete;
		Image& operator=( const Image& ) = delete;
		Image& operator=( Image&& ) = delete;

	public:
		Image( const Image_Constructor& ImageConstructor );
		Image( Image&& other ) noexcept;
		~Image();

	private:

	};
}