#include "Renderer.h"
#include "Instance.h"
#include "Devices/Device.h"
#include "../Debug/Debug.h"

#include "../../../Devices/Window.h"

#include <DebugLog.h>

namespace Hephaestus
{
	Renderer::Renderer() :
		m_instance( nullptr ),
		m_surface( VK_NULL_HANDLE ),
		m_device( nullptr )
	{}

	Renderer::~Renderer()
	{}

	bool Renderer::OnCreate( const RendererInfo& rendererInfo )
	{
		if( rendererInfo.window == nullptr )
		{
			// TODO: Update Log macro to take multiple arguments like printf("", ... args)
				// Change DEBUG_LOG to OUTPUT_LOG
				// Create macro that prints log to both output and console
			DEBUG_LOG( LOG::INFO, "Failed to create vulkan renderer: window is nullptr" );
			CONSOLE_LOG( LOG::INFO, "Failed to create vulkan renderer: window is nullptr" );
			return false;
		}

		// TODO: Have this take place in IRenderer, so we don't have to assign this or worry about it whenever we create a new renderer class
		m_window = rendererInfo.window;

		Instance_Constructor instanceConstructor( rendererInfo.appName, rendererInfo.engineName,
			rendererInfo.version, rendererInfo.enableValidationLayers );
		m_instance = std::make_unique<Instance>( instanceConstructor );

		// If requested, we enable the default validation layers for debugging
		if( rendererInfo.enableValidationLayers )
		{
			// The report flags determine what type of messages for the layers will be displayed
			// For validating (debugging) an application the error and warning bits should suffice
			VkDebugReportFlagsEXT debugReportFlags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
			// Additional flags include performance info, loader and layer debug messages, etc.
			debug::setupDebugging( m_instance->GetVkInstance(), debugReportFlags, VK_NULL_HANDLE );
		}

		m_surface = m_window->CreateSurface( *m_instance.get() );

		auto& gpu = m_instance->GetSuitableGPU( m_surface );
		
		Device_Constructor deviceConstructor( gpu, m_surface );
		m_device = std::make_unique<Device>( deviceConstructor );

		DEBUG_LOG( LOG::INFO, "Vulkan Renderer has been created!" );
		CONSOLE_LOG( LOG::INFO, "Vulkan Renderer has been created!" );

		return true;
	}

	void Renderer::OnDestroy()
	{
		if( m_device )
		{
			m_device->OnDestroy();
		}
		
		vkDestroySurfaceKHR( m_instance->GetVkInstance(), m_surface, nullptr );

		if( m_instance )
		{
			m_instance->OnDestroy();
		}
	}

	void Renderer::RenderScene( IScene* scene )
	{}

	void Renderer::Update( const float deltaTime )
	{}

	void Renderer::Render( MeshComponent* mesh, TransformComponent* transform, Camera* camera )
	{}

}