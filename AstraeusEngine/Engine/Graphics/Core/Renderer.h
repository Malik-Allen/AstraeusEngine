#ifndef RENDERER
#define RENDERER

#include "../Core/3D/MeshComponent.h"
#include "../Core/3D/TransformComponent.h"

#include <string>

class Window;
class Camera;
class IScene;

struct RendererInfo
{
	RendererInfo( const std::string& _appName, const std::string& _engineName,
		const int _version, const bool _enableValidationLayers, Window* _window ) :
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
	Window* window;
};

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
	Window* m_window;
	Camera* m_camera;

private:
	RendererManager* m_rendererManager;

	/*
	* 
	*/
	virtual void Render( MeshComponent* mesh, TransformComponent* transform, Camera* camera ) = 0;
};

#endif // !RENDERER

