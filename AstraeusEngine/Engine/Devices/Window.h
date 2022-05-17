#ifndef WINDOW_H
#define WINDOW_H

#include "WindowTypes.h"
// GraphicsAPI specific fucntionality
#include "../Graphics/Core/GraphicsAPI.h"
#if GRAPHICS_API == GRAPHICS_OPENGL
// some opengl headers

#elif GRAPHICS_API == GRAPHICS_VULKAN
#include <vulkan/vulkan_core.h>
namespace Hephaestus
{
	class Instance;
};
#endif

#include <vector>


class IWindow
{
	IWindow( const IWindow& ) = delete;
	IWindow& operator=( const IWindow& ) = delete;
	IWindow( IWindow&& ) = delete;
	IWindow& operator=( IWindow&& ) = delete;
public:
	explicit IWindow( const Window_Properties& properties ) :
		m_properties( properties )
	{};

	virtual ~IWindow()
	{};

	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void ProcessEvents() = 0;
	virtual void Close() = 0;

	inline const Window_Properties& GetProperties() const
	{
		return m_properties;
	}

	inline const Extent& GetExtent() const
	{
		return m_properties.extent;
	}

#if GRAPHICS_API == GRAPHICS_OPENGL
	// some opengl functions

#elif GRAPHICS_API == GRAPHICS_VULKAN
	virtual VkSurfaceKHR CreateSurface( Hephaestus::Instance& instance ) = 0;
	virtual std::vector<const char*> GetSurfaceExtensions() = 0;
#endif

protected:
	Window_Properties m_properties;

};


#endif