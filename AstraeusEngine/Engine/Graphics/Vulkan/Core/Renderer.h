#pragma once

#include "../../Core/Renderer.h"
#include "Instance.h"

namespace Hephaestus
{
	class Renderer : public IRenderer
	{
	public:
		Renderer();
		~Renderer();

		virtual bool OnCreate( const RendererInfo& rendererInfo ) override final;
		virtual void OnDestroy() override final;
		virtual void RenderScene( IScene* scene ) override final;

	private:
		Instance m_instance;

		virtual void Update( const float deltaTime ) override final;
		virtual void Render( MeshComponent* mesh, TransformComponent* transform, Camera* camera ) override final;
	};
};


