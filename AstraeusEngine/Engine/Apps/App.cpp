// Copyright (c) 2022 Malik Allen

#include "App.h"

#include "SceneManager.h"
#include "../Graphics/Core/RendererManager.h"
#include "../World/World.h"

#include <DebugLog.h>

IApp::IApp( const std::string& appName ) :
	m_appName( appName ),
	m_sceneManager( new SceneManager() ),
	m_rendererManager( new RendererManager() ),
	m_world( new World() )
{}

IApp::~IApp()
{
	if( m_rendererManager )
	{
		delete m_rendererManager;
		m_rendererManager = nullptr;
	}

	if( m_sceneManager )
	{
		delete m_sceneManager;
		m_sceneManager = nullptr;
	}

	if( m_world )
	{
		delete m_world;
		m_world = nullptr;
	}
}

bool IApp::OnCreate( const AppInfo& appInfo )
{
	if( m_world->OnCreate() == false )
	{
		DEBUG_LOG( LOG::INFO, "Failed to create app: failed to create world" );
		return false;
	}

	if( m_sceneManager->OnCreate() == false )
	{
		DEBUG_LOG( LOG::INFO, "Failed to create app: failed to create scene manager" );
		return false;
	}

	m_rendererManager->SetWorld( GetWorld() );
	m_rendererManager->SetSceneManager( m_sceneManager );
	if( m_rendererManager->OnCreate( appInfo.rendererInfo ) == false )
	{
		DEBUG_LOG( LOG::INFO, "Failed to create app: failed to create renderer manager" );
		return false;
	}
	

	return IApp_OnCreate();
}

void IApp::OnDestroy()
{
	IApp_OnDestroy();

	if( m_rendererManager )
	{
		m_rendererManager->OnDestroy();
	}
	if( m_sceneManager )
	{
		m_sceneManager->OnDestroy();
	}
	if( m_world )
	{
		m_world->OnDestroy();
	}
}

void IApp::Update( const float deltaTime )
{
	if( m_world )
	{
		m_world->Update( deltaTime );
	}
	if( m_sceneManager )
	{
		m_sceneManager->Update( deltaTime );
	}
	if( m_rendererManager )
	{
		m_rendererManager->Update( deltaTime );
	}
	IApp_Update( deltaTime );
}

const std::string& IApp::GetAppName() const
{
	return m_appName;
}


