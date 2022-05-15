#include "Renderer.h"
#include "Instance.h"
#include "../Debug/Debug.h"

#include <DebugLog.h>

namespace Hephaestus
{
	Renderer::Renderer() :
		m_instance( nullptr )
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
		
		m_instance = std::make_unique<Instance>();

		InstanceConstructor instanceConstructor( rendererInfo.appName, rendererInfo.engineName, 
			rendererInfo.version, rendererInfo.enableValidationLayers );

		if( m_instance->OnCreate( instanceConstructor ) != VK_SUCCESS )
		{
			DEBUG_LOG( LOG::INFO, "Failed to create vulkan renderer: could not create instance" );
			CONSOLE_LOG( LOG::INFO, "Failed to create vulkan renderer: could not create instance" );
			return false;
		}

		// If requested, we enable the default validation layers for debugging
		if( rendererInfo.enableValidationLayers )
		{
			// The report flags determine what type of messages for the layers will be displayed
			// For validating (debugging) an application the error and warning bits should suffice
			VkDebugReportFlagsEXT debugReportFlags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
			// Additional flags include performance info, loader and layer debug messages, etc.
			debug::setupDebugging( m_instance->GetVkInstance(), debugReportFlags, VK_NULL_HANDLE );
		}

		DEBUG_LOG( LOG::INFO, "Vulkan Renderer has been created!" );
		CONSOLE_LOG( LOG::INFO, "Vulkan Renderer has been created!" );

		return true;
	}

	void Renderer::OnDestroy()
	{
		m_instance->OnDestroy();
	}

	void Renderer::RenderScene( IScene* scene )
	{}

	void Renderer::Update( const float deltaTime )
	{}

	void Renderer::Render( MeshComponent* mesh, TransformComponent* transform, Camera* camera )
	{}

}