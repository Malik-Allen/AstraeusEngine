// Copyright (c) 2022 Malik Allen

#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "WindowTypes.h"

#include <memory>

class IWindow;

/*
*	Manages the creation and lifetime of a IWindow* for apps, engine, etc
*/
class WindowManager
{
public:
	WindowManager();
	~WindowManager();

	/*
	*	@brief	Creates a window using the passed window properties, immediately calling OnCreate() for the created window
	*	@param	Window_Properties:		Properties of the window that will be created
	*	@return	bool:		Returns true if the window has been created, returns false if otherwise
	*/
	bool Create_Window( Window_Properties properties );

	void OnDestroy();
	void ProcessEvents();

	/*Returns the current window if it has been created, if a window has not been created it will return nullptr*/
	IWindow* GetCurrentWindow() const;

	/*Prints the current window's details to logs*/
	void PrintWindowDetails();

private:
	std::unique_ptr<IWindow> m_currentWindow;
};

#endif // !WINDOWMANAGER_H

