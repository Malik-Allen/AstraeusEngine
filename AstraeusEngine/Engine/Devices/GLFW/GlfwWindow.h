// Copyright (c) 2022 Malik Allen

#pragma once

#include "../Window.h"

struct GLFWwindow;

class GlfwWindow : public IWindow
{
public:
	GlfwWindow( const Window_Properties& properties );
	~GlfwWindow();

	virtual bool OnCreate() override final;
	virtual void OnDestroy() override final;
	virtual void ProcessEvents() override final;
	virtual void Close() override final;

	GLFWwindow* GetGLFW_Window() const
	{
		return m_glfwWindow;
	};

#if GRAPHICS_API == GRAPHICS_OPENGL
	bool InitGlad();

#elif GRAPHICS_API == GRAPHICS_VULKAN
	// TODO: Consider adding a swapchain reference to this class and some swapchain functionality
	virtual VkSurfaceKHR CreateSurface( Hephaestus::Instance& instance ) override final;
	virtual std::vector<const char*> GetSurfaceExtensions() override final;
#endif

private:
	GLFWwindow* m_glfwWindow;

	void SetPre_Attributes();
	void SetPost_Attributes();
};
