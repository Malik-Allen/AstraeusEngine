#pragma once

#include "../../Core/Renderer.h"
#include <memory>
#include <vulkan/vulkan_core.h>

namespace Hephaestus
{
	class Instance;
	class Device;
	class PhysicalDevice;

	class Renderer : public IRenderer
	{
	public:
		Renderer();
		~Renderer();

		virtual bool OnCreate( const RendererInfo& rendererInfo ) override final;
		virtual void OnDestroy() override final;
		virtual void RenderScene( IScene* scene ) override final;

	private:
		std::unique_ptr<Instance> m_instance;
		VkSurfaceKHR m_surface;
		std::unique_ptr<Device> m_device;

		virtual void Update( const float deltaTime ) override final;
		virtual void Render( MeshComponent* mesh, TransformComponent* transform, Camera* camera ) override final;
	};
};


