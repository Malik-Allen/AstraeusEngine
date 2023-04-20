// Copyright (c) 2022 Malik Allen

#ifndef ENGINE_H
#define ENGINE_H

#include <memory>	// unique_ptr<>
#include <string>
#include "../Devices/WindowTypes.h"

class EngineClock;
class WindowManager;
class IApp;

/*
* 
*/
struct Engine_Properties
{
	explicit Engine_Properties() :
		name( "" ),
		windowExtent( Extent() ),
		fps( 0 ),
		unlockFPS( false )
	{};

	explicit Engine_Properties( const std::string& _name, const Extent _windowExtent, 
		const unsigned int _fps, const bool _unlockFps = false ) :
		name( _name ),
		windowExtent( _windowExtent ),
		fps( _fps ),
		unlockFPS( _unlockFps )
	{};

	std::string name;
	Extent windowExtent;
	unsigned int fps;
	bool unlockFPS;
};

// Singleton Engine Class
class Engine
{

public:

	// Initialize Engine at passed fps, inits window at passed window width and height
	bool Init( const Engine_Properties& properties );

	// Loads Passed Application, beginning with creating its desired renderer
	bool LoadApplication( IApp* app );

	// Runs current application
	void Run();

	// Returns true if engine is running
	bool IsRunning() const;

	// Returns true if current app is running
	bool IsAppRunning() const;

	// Exits Engine
	void Exit();

	// Exists App
	void ExitApp();

	// Get Instance of Engine
	static Engine* Get();

private:

	// The Engine class should not be copied or moved hence removing the functionality
	Engine( const Engine& ) = delete;
	Engine& operator=( const Engine& ) = delete;
	Engine( Engine&& ) = delete;
	Engine& operator = ( Engine&& ) = delete;

	Engine();
	~Engine();

	static std::unique_ptr<Engine> g_engineInstance;
	friend std::default_delete<Engine>;

	void OnDestroy();
	void Update( const float deltaTime );

	Engine_Properties	m_properties;

	std::unique_ptr<EngineClock> m_engineClock;

	bool				m_isRunning;
	bool				m_isAppRunning;

	// TODO: Make this a shared ptr, (optional) make an app manager
	IApp* m_app;

	std::unique_ptr<WindowManager> m_windowManager;

	// Prints the engine's current running stats (fps, app, etc.) to the console log
	void PrintRuntimeInfo();
};

#endif

// TODO: Add a config file reader for Engine Config, Renderer, ECS 
// - Renderer: If I can do a compile-time check for config file, then I can choose a graphics api in the renderer instead of changing a macro directly
// - ECS :Right now it just spawns 10000 entities by default but it should be customizable at run-time, I'm thinking some static functionality so I can provide the values to intiate the ECS