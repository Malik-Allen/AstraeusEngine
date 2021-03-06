// Copyright (c) 2022 Malik Allen

#ifndef APP_H
#define APP_H

#include "../Graphics/Core/Renderer.h"	// RendererInfo struct

#include <string>

class SceneManager;
class RendererManager;
class World;
class Window;

/*
*	Information about the application, provided during the creation on an application
*/
struct AppInfo
{
	AppInfo() :
		rendererInfo( RendererInfo() )
	{};

	RendererInfo rendererInfo;
};

/*
*	The App interface will allow you to create an application and have it manage what is created and destroying during the lifetime of the application
*	Inherited classes will be able to spawn actors, create and register system
*/
class IApp
{
	friend class Engine;

	IApp( const IApp& ) = delete;
	IApp& operator=( const IApp& ) = delete;
	IApp( IApp&& ) = delete;
	IApp& operator = ( IApp&& ) = delete;

public:
	explicit IApp( const std::string& appName );
	virtual ~IApp();

	bool OnCreate( const AppInfo& appInfo );
	void OnDestroy();
	void Update( const float deltaTime );

	// Returns the name of this application
	const std::string& GetAppName() const;

	inline SceneManager* GetSceneManager() const
	{
		return m_sceneManager;
	}

	inline World* GetWorld() const
	{
		return m_world;
	}

private:
	std::string		m_appName;

	virtual bool IApp_OnCreate() = 0;
	virtual void IApp_OnDestroy() = 0;
	virtual void IApp_Update( const float deltaTime ) = 0;

private:
	SceneManager* m_sceneManager;
	RendererManager* m_rendererManager;
	World* m_world;
};



#endif // !APP_H

