// Copyright (c) 2022 Malik Allen

#ifndef RENDERERMANAGER_H
#define RENDERERMANAGER_H

#include "Renderer.h"

class SceneManager;
class World;

/*
*	Renderer Manager is responsible for creating and registering the IRenderer of the macro-defined graphics api
*	Passing along RendererInformation upon its creation
*/
class RendererManager
{
public:
	RendererManager();
	~RendererManager();

	bool OnCreate( const RendererInfo& rendererInfo );
	void OnDestroy();
	void Update( const float deltaTime );

	inline void SetSceneManager( SceneManager* sceneManager )
	{
		m_sceneManager = sceneManager;
	}

	inline void SetWorld( World* world )
	{
		m_world = world;
	}

private:
	IRenderer* m_currentRenderer;
	SceneManager* m_sceneManager;
	World* m_world;
};


#endif // !RENDERERMANAGER_H

