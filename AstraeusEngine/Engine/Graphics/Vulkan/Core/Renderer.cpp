#include "Renderer.h"

#include <DebugLog.h>

namespace Hephaestus
{
	Renderer::Renderer() :
		m_instance( Instance() )
	{}

	Renderer::~Renderer()
	{}

	bool Renderer::OnCreate( const RendererInfo& rendererInfo )
	{
		if( rendererInfo.window == nullptr )
		{
			DEBUG_LOG( LOG::INFO, "Failed to create vulkan renderer: window is nullptr" );
			CONSOLE_LOG( LOG::INFO, "Failed to create vulkan renderer: window is nullptr" );
			return false;
		}

		InstanceInfo instanceInfo( rendererInfo.appName, rendererInfo.engineName, rendererInfo.version, rendererInfo.enableValidationLayers );
		if( m_instance.OnCreate( instanceInfo ) != VK_SUCCESS )
		{
			DEBUG_LOG( LOG::INFO, "Failed to create vulkan renderer: could not create instance" );
			CONSOLE_LOG( LOG::INFO, "Failed to create vulkan renderer: could not create instance" );
			return false;
		}

		return true;
	}

	void Renderer::OnDestroy()
	{
		m_instance.OnDestroy();
	}

	void Renderer::RenderScene( IScene* scene )
	{}

	void Renderer::Update( const float deltaTime )
	{}

	void Renderer::Render( MeshComponent* mesh, TransformComponent* transform, Camera* camera )
	{}

}