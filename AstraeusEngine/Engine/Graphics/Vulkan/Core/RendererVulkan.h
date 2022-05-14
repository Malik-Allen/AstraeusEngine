#ifndef RENDERERVULKAN_H
#define RENDERERVULKAN_H

#include "../../Core/Renderer.h"

namespace Vulkan
{
	class Renderer_Vulkan : public IRenderer
	{
	public:
		Renderer_Vulkan();
		~Renderer_Vulkan();

		virtual bool OnCreate( const RendererInfo& rendererInfo ) override final;
		virtual void OnDestroy() override final;
		virtual void RenderScene( IScene* scene ) override final;

	private:
		virtual void Render( MeshComponent* mesh, TransformComponent* transform, Camera* camera ) override final;
	};
};

#endif // !RENDERERVULKAN_H

