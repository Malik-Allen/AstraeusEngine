#ifndef RENDERERMANAGER_H
#define RENDERERMANAGER_H

#include "Renderer.h"

class SceneManager;

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

private:
	IRenderer* m_currentRenderer;
	SceneManager* m_sceneManager;
};


#endif // !RENDERERMANAGER_H

