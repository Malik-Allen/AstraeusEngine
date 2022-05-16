#include "RendererManager.h"
#include "../../Apps/SceneManager.h"
#include "../../World/World.h"
#include "GraphicsAPI.h"

#include <DebugLog.h>

#if GRAPHICS_API == GRAPHICS_OPENGL
#include "../OpenGL/Core/RendererOpenGL.h"
#elif GRAPHICS_API == GRAPHICS_VULKAN
#include "../Vulkan/Core/Renderer.h"
#endif

RendererManager::RendererManager() :
	m_currentRenderer( nullptr ),
	m_sceneManager( nullptr ),
	m_world( nullptr )
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
	if( m_world == nullptr )
	{
		DEBUG_LOG( LOG::INFO, "Failed to create renderer manager: world object is nullptr, SetWorld(), before calling OnCreate()" );
		return false;
	}

#if GRAPHICS_API == GRAPHICS_OPENGL
	DEBUG_LOG( LOG::INFO, "Creating OpenGL Renderer..." );
	
	m_currentRenderer = m_world->GetECS()->RegisterSystem<OpenGL::Renderer_OpenGL>();

#elif GRAPHICS_API == GRAPHICS_VULKAN
	DEBUG_LOG( LOG::INFO, "Creating Vulkan Renderer..." );
	
	m_currentRenderer = m_world->GetECS()->RegisterSystem<Hephaestus::Renderer>();

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
		// Since IRenderer is a System that exists inside of an ECS, we don't need to call update, it will have access to the components it needs
			// And anything it doesn't have we can just run a ECS::Parser on it to get it
		// m_currentRenderer->RenderScene( m_sceneManager->GetCurrentScene() );
	}
}
