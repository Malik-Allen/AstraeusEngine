// Copyright (c) 2022 Malik Allen

#ifndef SCENE_H
#define SCENE_H

/*
*
*/
class IScene
{
public:
	friend class SceneManager;

	IScene() :
		m_sceneManager( nullptr )
	{};

	virtual ~IScene()
	{};

	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update( const float deltaTime ) = 0;

protected:
	SceneManager* m_sceneManager;
};

#endif // !SCENE_H
