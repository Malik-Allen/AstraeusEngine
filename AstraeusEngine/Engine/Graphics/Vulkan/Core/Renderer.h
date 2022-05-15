#pragma once

#include "../../Core/Renderer.h"
#include <memory>

namespace Hephaestus
{
	class Instance;

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

		virtual void Update( const float deltaTime ) override final;
		virtual void Render( MeshComponent* mesh, TransformComponent* transform, Camera* camera ) override final;
	};
};


