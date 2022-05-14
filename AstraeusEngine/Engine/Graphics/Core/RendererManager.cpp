#include "RendererManager.h"
#include "../../Apps/SceneManager.h"
#include "GraphicsAPI.h"

#include <DebugLog.h>

#if GRAPHICS_API == GRAPHICS_OPENGL
#include "../OpenGL/Core/RendererOpenGL.h"
#elif GRAPHICS_API == GRAPHICS_VULKAN
#include "../Vulkan/Core/RendererVulkan.h"
#endif

RendererManager::RendererManager() :
	m_currentRenderer( nullptr ),
	m_sceneManager( nullptr )
{}

RendererManager::~RendererManager()
{
	if( m_currentRenderer )
	{
		delete m_currentRenderer;
		m_currentRenderer = nullptr;
	}
}

bool RendererManager::OnCreate( const RendererInfo& rendererInfo )
{
#if GRAPHICS_API == GRAPHICS_OPENGL
	DEBUG_LOG( LOG::INFO, "Creating OpenGL Renderer..." );
	CONSOLE_LOG( LOG::INFO, "Creating OpenGL Renderer..." );

	m_renderer = new OpenGL::Renderer_OpenGL();

#elif GRAPHICS_API == GRAPHICS_VULKAN
	DEBUG_LOG( LOG::INFO, "Creating Vulkan Renderer..." );
	CONSOLE_LOG( LOG::INFO, "Creating Vulkan Renderer..." );

	m_currentRenderer = new Vulkan::Renderer_Vulkan();

#endif

	if( m_currentRenderer->OnCreate( rendererInfo ) == false )
	{
		return false;
	}

	m_currentRenderer->m_rendererManager = this;

	return true;
}

void RendererManager::OnDestroy()
{
	if( m_currentRenderer )
	{
		m_currentRenderer->OnDestroy();
	}
}

void RendererManager::Update( const float deltaTime )
{
	if( m_sceneManager != nullptr )
	{
		m_currentRenderer->RenderScene( m_sceneManager->GetCurrentScene() );
	}
}
