#include "RendererOpenGL.h"

#include <DebugLog.h>

namespace OpenGL
{
	Renderer_OpenGL::Renderer_OpenGL()
	{}

	Renderer_OpenGL::~Renderer_OpenGL()
	{}

	bool Renderer_OpenGL::OnCreate( const RendererInfo & rendererInfo )
	{
		if( rendererInfo.window == nullptr )
		{
			DEBUG_LOG( LOG::INFO, "Failed to create vulkan renderer: window is nullptr" );
			CONSOLE_LOG( LOG::INFO, "Failed to create vulkan renderer: window is nullptr" );
			return false;
		}
		return true;
	}

	void Renderer_OpenGL::OnDestroy()
	{}

	void Renderer_OpenGL::RenderScene( IScene * scene )
	{}
}