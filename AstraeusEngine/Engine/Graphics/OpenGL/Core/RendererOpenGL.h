// Copyright (c) 2022 Malik Allen

#ifndef RENDEREROPENGL_H
#define RENDEREROPENGL_H

#include "../../Core/Renderer.h"

namespace OpenGL
{
	class Renderer_OpenGL : public IRenderer
	{
	public:
		Renderer_OpenGL();
		~Renderer_OpenGL();

		virtual bool OnCreate( const RendererInfo& rendererInfo ) override final;
		virtual void OnDestroy() override final;
		virtual void RenderScene( IScene* scene ) override final;

	private:

	};
}

#endif // !RENDEREROPENGL_H

