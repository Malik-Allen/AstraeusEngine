// Copyright (c) 2022 Malik Allen

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

class IScene;

/*
*	Responsible for loading and unloding scenes passed to this SceneManager
*/
class SceneManager
{
public:

	SceneManager();
	~SceneManager();

	bool OnCreate();
	void OnDestroy();
	void Update( const float deltaTime );

	/*
	*	Loads the passed scene instance, destroying the current scene first, if one exists. Then calls OnCreate for the scene to load.
	*	@param	Scene:		The new scene to be loaded
	*	@return	bool:		Returns true if the new scene has been loaded, false if otherwise
	*/
	bool LoadScene( IScene* scene );

	inline IScene* GetCurrentScene() const
	{
		return m_currentScene;
	}

private:

	IScene* m_currentScene;
};

#endif // !SCENEMANAGER_H

