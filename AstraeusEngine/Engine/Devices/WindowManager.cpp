// Copyright (c) 2022 Malik Allen

#include "WindowManager.h"
#include "GLFW/GlfwWindow.h"

#include <DebugLog.h>

WindowManager::WindowManager() :
	m_currentWindow( nullptr )
{}

WindowManager::~WindowManager()
{}

bool WindowManager::Create_Window( Window_Properties properties )
{
	if( m_currentWindow.get() != nullptr )
	{
		DEBUG_LOG( LOG::WARNING, "Window already exists inside window manager, destroying current window!" );
		m_currentWindow->OnDestroy();
	}

#if WINDOW_API == WINDOW_GLFW
	DEBUG_LOG( LOG::INFO, "Creating GLFW Window..." );

	m_currentWindow = std::make_unique<GlfwWindow>( properties );
	if( m_currentWindow->OnCreate() == false )
	{
		DEBUG_LOG( LOG::FATAL, "Failed to create GlfwWindow!" );
		return false;
	}

#endif

	DEBUG_LOG( LOG::INFO, "Created window!" );
	return true;
}

void WindowManager::OnDestroy()
{
	if( m_currentWindow.get() != nullptr )
	{
		m_currentWindow->OnDestroy();
	}
}

void WindowManager::ProcessEvents()
{
	if( m_currentWindow.get() != nullptr )
	{
		m_currentWindow->ProcessEvents();
	}
}

IWindow* WindowManager::GetCurrentWindow() const
{
	return m_currentWindow.get();
}

void WindowManager::PrintWindowDetails()
{
	if( m_currentWindow.get() != nullptr )
	{
		DEBUG_LOG( LOG::INFO, "Window is running @ {} x {}", std::to_string( m_currentWindow->GetExtent().width ), std::to_string( m_currentWindow->GetExtent().height ) );
	}
}
