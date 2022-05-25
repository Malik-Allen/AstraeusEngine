// Copyright (c) 2022 Malik Allen

#include "SceneManager.h"

#include "Scene.h"

#include <DebugLog.h>

SceneManager::SceneManager() :
	m_currentScene( nullptr )
{}

SceneManager::~SceneManager()
{
	if( m_currentScene )
	{
		delete m_currentScene;
		m_currentScene = nullptr;
	}
}

bool SceneManager::OnCreate()
{
	return true;
}

void SceneManager::OnDestroy()
{
	if( m_currentScene )
	{
		m_currentScene->OnDestroy();
	}
}

void SceneManager::Update( const float deltaTime )
{
	if( m_currentScene )
	{
		m_currentScene->Update( deltaTime );
	}
}

bool SceneManager::LoadScene( IScene* scene )
{
	if( scene == nullptr )
	{
		DEBUG_LOG( LOG::INFO, "Failed to load scene: scene is nullptr" );
		return false;
	}

	if( m_currentScene )
	{
		m_currentScene->OnDestroy();
		// TODO: Replace with a pooling alternative 
		delete m_currentScene;
		m_currentScene = nullptr;
	}

	m_currentScene = scene;
	m_currentScene->m_sceneManager = this;
	return m_currentScene->OnCreate();
}
