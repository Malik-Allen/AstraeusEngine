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
	virtual VkSurfaceKHR CreateSurface( Hephaestus::Instance& instance ) override final;

#endif

private:
	GLFWwindow* m_glfwWindow;

	void SetPre_Attributes();
	void SetPost_Attributes();
};
