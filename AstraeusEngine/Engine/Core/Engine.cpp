#include "Engine.h"
#include "Timer/EngineClock.h"
#include "../Apps/App.h"
#include "../Devices/WindowManager.h"

#include <DebugLog.h>

#include <iostream>


std::unique_ptr<Engine> Engine::g_engineInstance( nullptr );

Engine::Engine() :
	m_properties( Engine_Properties() ),
	m_engineClock( nullptr ),
	m_isRunning( false ),
	m_isAppRunning( false ),
	m_app( nullptr ),
	m_windowManager( nullptr )
{}

Engine::~Engine()
{}

bool Engine::Init( const Engine_Properties& properties )
{
	DEBUG_INIT();
	m_properties = properties;

	DEBUG_LOG( LOG::INFO, "Initializing boot sequence... Please wait..." );

	m_engineClock = new EngineClock();
	if( m_engineClock == nullptr )
	{
		DEBUG_LOG( LOG::FATAL, "Failed to create engine clock!" );
		return false;
	}

	m_engineClock->SetFPS( m_properties.fps );

	Window_Properties windowProperties;
	windowProperties.name = m_properties.name;
	windowProperties.extent = m_properties.windowExtent;

	m_windowManager = std::make_unique<WindowManager>();
	if( m_windowManager->Create_Window( windowProperties ) == false )
	{
		DEBUG_LOG( LOG::FATAL, "Failed to create window!" );
		return false;
	}

	DEBUG_LOG( LOG::INFO, "Boot sequence complete!" );

	PrintRuntimeInfo();

	m_isRunning = true;
	return m_isRunning;
}

// Loads Passed Application, beginning with creating its desired renderer
bool Engine::LoadApplication( IApp* app )
{
	DEBUG_LOG( LOG::INFO, "Loading application... Please wait..." );

	if( app == nullptr )
	{
		DEBUG_LOG( LOG::WARNING, "Failed to laod application: App is null" );
		return false;
	}

	if( IsRunning() == false )
	{
		DEBUG_LOG( LOG::WARNING, "Failed to laod application: Engine is not running, call Init() before loading the Application" );
		return false;
	}

	// If another app is running, close the application before opening a new one
	if( m_app != nullptr && IsAppRunning() )
	{
		ExitApp();
	}

	m_app = app;

	AppInfo appInfo;
	appInfo.rendererInfo = RendererInfo( m_app->GetAppName(), m_properties.name, 1, false, m_windowManager->GetCurrentWindow() );
	if( m_app->OnCreate( appInfo ) == false )
	{
		DEBUG_LOG( LOG::ERRORLOG, "Failed to create application!" );
		return false;
	}

	m_isAppRunning = true;

	DEBUG_LOG( LOG::INFO, "Load complete!" );

	PrintRuntimeInfo();

	return m_isAppRunning;
}


// Runs current application
void Engine::Run()
{
	while( m_isRunning )
	{
		m_engineClock->UpdateFrameTicks();
		Update( m_engineClock->GetDeltaTime() );
		// DEBUG_LOG( LOG::INFO, "\tTick: {}", m_engineClock->GetDeltaTime() );
		if( !m_properties.unlockFPS )
		{
			// TODO: Replace Sleep! It's very inaccurate, find out how I can limit fps
			Sleep( m_engineClock->GetSleepTime( m_engineClock->GetFPS() ) );
		}
	}

	if( !m_isRunning )
	{
		OnDestroy();
	}
}


bool Engine::IsRunning() const
{
	return m_isRunning;
}

bool Engine::IsAppRunning() const
{
	return m_isAppRunning;
}

void Engine::Exit()
{
	m_isRunning = false;
}

void Engine::ExitApp()
{
	m_isAppRunning = false;

	if( m_app != nullptr )
	{
		m_app->OnDestroy();
	}
}

Engine* Engine::Get()
{
	if( g_engineInstance == nullptr )
	{
		g_engineInstance.reset( new Engine );
		return g_engineInstance.get();
	}
	return g_engineInstance.get();
}


void Engine::OnDestroy()
{
	ExitApp();
	if( m_app )
	{
		delete m_app;
		m_app = nullptr;
	}

	if( m_windowManager )
	{
		m_windowManager->OnDestroy();
	}

	if( m_engineClock )
	{
		delete m_engineClock;
		m_engineClock = nullptr;
	}

}

void Engine::Update( const float deltaTime )
{
	if( m_app != nullptr )
	{
		if( m_isAppRunning )
		{
			m_app->Update( deltaTime );
		}
		else
		{
			m_app->OnDestroy();
		}
	}

	m_windowManager->ProcessEvents();
}

void Engine::PrintRuntimeInfo()
{
	DEBUG_LOG( LOG::INFO, "{} is running @ {}fps", m_properties.name, m_properties.unlockFPS ? "an unlocked " : std::to_string( m_properties.fps ) );

	if( m_windowManager != nullptr )
	{
		m_windowManager->PrintWindowDetails();
	}

	if( m_isAppRunning && m_app != nullptr )
	{
		DEBUG_LOG( LOG::INFO, "{} is running", m_app->GetAppName() );
	}
}



