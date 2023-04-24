// Copyright (c) 2022 Malik Allen

#include "GlfwWindow.h"

#include <DebugLog.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#if GRAPHICS_API == GRAPHICS_OPENGL
// some opengl headers

#elif GRAPHICS_API == GRAPHICS_VULKAN
#include "../../Graphics/Vulkan/Core/Initialization/Instance.h"

#endif

GlfwWindow::GlfwWindow( const Window_Properties& properties ) :
	IWindow( properties ),
	m_glfwWindow( nullptr )
{}

GlfwWindow::~GlfwWindow()
{}

bool GlfwWindow::OnCreate()
{
	if( !glfwInit() )
	{
		DEBUG_LOG( LOG::FATAL, "Failed to init GLFW!" );
		return false;
	}

	SetPre_Attributes();

	m_glfwWindow = glfwCreateWindow( m_properties.extent.width, m_properties.extent.height, m_properties.name.c_str(), nullptr, nullptr );

	if( !m_glfwWindow )
	{
		DEBUG_LOG( LOG::FATAL, "Failed to create GLFW window!" );
		return false;
	}

	SetPost_Attributes();

	// AssignCallbacks();

	// glfwSetWindowUserPointer(window, vulkanInstance);

	return true;
}

void GlfwWindow::OnDestroy()
{
	glfwDestroyWindow( m_glfwWindow );
	glfwTerminate();
}

void GlfwWindow::ProcessEvents()
{
	glfwPollEvents();
}

void GlfwWindow::Close()
{}

#if GRAPHICS_API == GRAPHICS_OPENGL
bool GlfwWindow::InitGlad()
{
	if( !gladLoadGLLoader( ( GLADloadproc )glfwGetProcAddress ) )
	{
		DEBUG_LOG( LOG::FATAL, "Failed to init GLAD!" );
		return false;
	}
	return true;
}

#elif GRAPHICS_API == GRAPHICS_VULKAN

VkSurfaceKHR GlfwWindow::CreateSurface( Hephaestus::Instance& instance )
{
	if( instance.GetVkInstance() == VK_NULL_HANDLE )
	{
		return VK_NULL_HANDLE;
	}

	VkSurfaceKHR surface;

	VkResult errCode = glfwCreateWindowSurface( instance.GetVkInstance(), m_glfwWindow, NULL, &surface );

	if( errCode != VK_SUCCESS )
	{
		return nullptr;
	}

	return surface;
}

std::vector<const char*> GlfwWindow::GetSurfaceExtensions()
{
	uint32_t extensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions( &extensionCount );

	std::vector<const char*> surfaceExtensions( glfwExtensions, glfwExtensions + extensionCount );
	return surfaceExtensions;
}

#endif



void GlfwWindow::SetPre_Attributes()
{
	// Window hints can be use before the window has been created to give the window properties and functionality. 
	// Once a window has been created a new window hint will not affect it/ cannot be applied after the fact
#if GRAPHICS_API == GRAPHICS_OPENGL
	glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_API );
	glfwWindowHint( GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API );

#elif GRAPHICS_API == GRAPHICS_VULKAN
	glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
	//glfwWindowHint( GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API );
	//glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

#endif


}

void GlfwWindow::SetPost_Attributes()
{
#if GRAPHICS_API == GRAPHICS_OPENGL
	glfwMakeContextCurrent( m_glfwWindow );
	m_glfwWindow = glfwGetCurrentContext();

	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_RESIZABLE, GLFW_TRUE );
	glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE );
	glfwWindowHint( GLFW_DEPTH_BITS, 64 );

#elif GRAPHICS_API == GRAPHICS_VULKAN
	// Some vulkan functions

#endif

}



