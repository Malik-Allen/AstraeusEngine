#include "Engine.h"
#include "Timer/EngineClock.h"
#include "../Apps/App.h"
#include "../Devices/Window.h"

#include <DebugLog.h>

#include <iostream>


std::unique_ptr<Engine> Engine::g_engineInstance( nullptr );

Engine::Engine() :
	m_engineName( "" ),
	m_engineClock( nullptr ),
	m_isRunning( false ),
	m_isAppRunning( false ),
	m_fps( 120 ),
	m_app( nullptr ),
	m_window( nullptr )
{}

Engine::~Engine()
{}

// Initializes Engine Components
bool Engine::Init(
	const std::string& engineName,
	const unsigned int fps,
	const int windowWidth,
	const int windowHeight )
{
	DEBUG_INIT();
	m_engineName = engineName;

	DEBUG_LOG( LOG::INFO, "Initializing boot sequence... Please wait..." );
	CONSOLE_LOG( LOG::INFO, "Initializing boot sequence... Please wait..." );

	m_engineClock = new EngineClock();
	if( m_engineClock == nullptr )
	{
		DEBUG_LOG( LOG::FATAL, "Failed to create engine clock!" );
		CONSOLE_LOG( LOG::FATAL, "Failed to create engine clock!" );
		return false;
	}

	m_fps = fps;
	m_engineClock->SetFPS( m_fps );

	m_window = new Window();
	if( m_window->OnCreate( m_engineName, windowWidth, windowHeight ) == false )
	{
		DEBUG_LOG( LOG::FATAL, "Failed to create window!" );
		CONSOLE_LOG( LOG::FATAL, "Failed to create window!" );
		return false;
	}

	DEBUG_LOG( LOG::INFO, "Boot sequence complete!" );
	CONSOLE_LOG( LOG::INFO, "Boot sequence complete!" );

	PrintRuntimeInfo();

	m_isRunning = true;
	return m_isRunning;
}

// Loads Passed Application, beginning with creating its desired renderer
bool Engine::LoadApplication( IApp* app )
{
	DEBUG_LOG( LOG::INFO, "Loading application... Please wait..." );
	CONSOLE_LOG( LOG::INFO, "Loading application... Please wait..." );

	if( app == nullptr )
	{
		DEBUG_LOG( LOG::WARNING, "Failed to laod application: App is null" );
		CONSOLE_LOG( LOG::WARNING, "Failed to laod application: App is null" );
		return false;
	}

	if( IsRunning() == false )
	{
		DEBUG_LOG( LOG::WARNING, "Failed to laod application: Engine is not running, call Init() before loading the Application" );
		CONSOLE_LOG( LOG::WARNING, "Failed to laod application: Engine is not running, call Init() before loading the Application" );
		return false;
	}

	// If another app is running, close the application before opening a new one
	if( m_app != nullptr && IsAppRunning() )
	{
		ExitApp();
	}

	m_app = app;

	AppInfo appInfo;
	appInfo.rendererInfo = RendererInfo( m_app->GetAppName(), m_engineName, 1, true, m_window );
	if( m_app->OnCreate( appInfo ) == false )
	{
		DEBUG_LOG( LOG::ERRORLOG, "Failed to create application!" );
		CONSOLE_LOG( LOG::ERRORLOG, "Failed to create application!" );
		return false;
	}

	m_isAppRunning = true;

	DEBUG_LOG( LOG::INFO, "Load complete!");
	CONSOLE_LOG( LOG::INFO, "Load complete!");

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
		Sleep( m_engineClock->GetSleepTime( m_engineClock->GetFPS() ) );
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

	if( m_window )
	{
		m_window->OnDestroy();
		delete m_window;
		m_window = nullptr;
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

	m_window->ProcessEvents();
}

void Engine::PrintRuntimeInfo()
{
	DEBUG_LOG( LOG::INFO, m_engineName + " is running @ " + std::to_string( m_fps ) + " fps" );
	CONSOLE_LOG( LOG::INFO, m_engineName + " is running @ " + std::to_string( m_fps ) + " fps" );

	if( m_window != nullptr )
	{
		DEBUG_LOG( LOG::INFO, m_engineName + "'s Window is running @ " + std::to_string( m_window->GetWidth() ) + " x " + std::to_string( m_window->GetHeight() ) );
		CONSOLE_LOG( LOG::INFO, m_engineName + "'s Window is running @ " + std::to_string( m_window->GetWidth() ) + " x " + std::to_string( m_window->GetHeight() ) );
	}

	if( m_isAppRunning && m_app != nullptr )
	{
		DEBUG_LOG( LOG::INFO, m_engineName + " is running the " + m_app->GetAppName() + " application" );
		CONSOLE_LOG( LOG::INFO, m_engineName + " is running the " + m_app->GetAppName() + " application" );
	}
}



