#include "RendererVulkan.h"

#include <DebugLog.h>

namespace Vulkan
{
	Renderer_Vulkan::Renderer_Vulkan()
	{}

	Renderer_Vulkan::~Renderer_Vulkan()
	{}

	bool Renderer_Vulkan::OnCreate( const RendererInfo & rendererInfo )
	{
		if( rendererInfo.window == nullptr )
		{
			DEBUG_LOG( LOG::INFO, "Failed to create vulkan renderer: window is nullptr" );
			CONSOLE_LOG( LOG::INFO, "Failed to create vulkan renderer: window is nullptr" );
			return false;
		}

		return true;
	}

	void Renderer_Vulkan::OnDestroy()
	{}

	void Renderer_Vulkan::RenderScene( IScene * scene )
	{}

	void Renderer_Vulkan::Render( MeshComponent * mesh, TransformComponent * transform, Camera * camera )
	{}

}