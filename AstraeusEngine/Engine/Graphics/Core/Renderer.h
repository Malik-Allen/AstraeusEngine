// Copyright (c) 2022 Malik Allen

#ifndef RENDERER
#define RENDERER

#include "../Core/3D/MeshComponent.h"
#include "../Core/3D/TransformComponent.h"

#include <string>

class IWindow;
class Camera;
class IScene;
class RendererManager;

/*
* Renderer details and information, provided on creation of renderer
*/
struct RendererInfo
{
	RendererInfo( const std::string& _appName, const std::string& _engineName,
		const int _version, const bool _enableValidationLayers, IWindow* _window ) :
		appName( _appName ),
		engineName( _engineName ),
		version( _version ),
		enableValidationLayers( _enableValidationLayers ),
		window( _window )
	{};

	RendererInfo()
	{
		appName = "";
		engineName = "";
		version = 0;
		enableValidationLayers = false;
		window = nullptr;
	};

	std::string appName;
	std::string	engineName;
	int	version;
	bool enableValidationLayers;
	IWindow* window;
};

/*
*	The IRenderer System is responsible for renderering mesh components
*	When this class is inherited you will be able to customize your own renderer to a specific graphics api etc. with full control of order of operation when it comes to rendering
*	
*/
class IRenderer : public ECS::System<MeshComponent, TransformComponent>
{
	friend class RendererManager;

	/*IRenderer( const IRenderer& ) = delete;
	IRenderer& operator=( const IRenderer& ) = delete;
	IRenderer( IRenderer&& ) = delete;
	IRenderer& operator = ( IRenderer&& ) = delete;*/

public:
	static constexpr uint64_t ID = GENERATE_ID( "IRenderer" );

	IRenderer() :
		ECS::System<MeshComponent, TransformComponent>( ID ),
		m_camera( nullptr ),
		m_window( nullptr ),
		m_rendererManager( nullptr )
	{};

	virtual ~IRenderer()
	{};

	virtual bool OnCreate( const RendererInfo& rendererInfo ) = 0;
	virtual void OnDestroy() = 0;
	virtual void RenderScene( IScene* scene ) = 0;

	inline void SetCamera( Camera* camera )
	{
		m_camera = camera;
	};

protected:
	IWindow* m_window;
	Camera* m_camera;

private:
	RendererManager* m_rendererManager;

	/*
	*	ISystem: Update(), ECS::World->SystemManager->Update()
	*/
	virtual void Update( const float deltaTime ) override = 0;

	/*
	*	Performs the render for the passed mesh component
	*	@param	MeshComponent:		The mesh component to render
	*	@param	TransformComponent:		The transform component to use for the location of the mesh
	*	@param	Camera:		The camera that the mesh will be renderered to
	*/
	virtual void Render( MeshComponent* mesh, TransformComponent* transform, Camera* camera ) = 0;
};

#endif // !RENDERER

