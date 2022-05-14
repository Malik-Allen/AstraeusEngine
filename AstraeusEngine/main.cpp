#include "Engine/Core/Engine.h"

#include "Engine/Apps/App.h"
#include "Engine/World/World.h"

class TestRun : public IApp
{
public:
	TestRun() : IApp( "Test Run" )
	{};
	~TestRun()
	{};

	virtual bool IApp_OnCreate()
	{
		
		GetWorld()->SpawnActor( ActorSpawnInfo() );
		
		return true;
	};

	virtual void IApp_OnDestroy()
	{};

	virtual void IApp_Update( const float deltaTime )
	{};
private:

};

int main( int args, char* argv[] )
{
	Engine::Get()->Init( "Astraeus Engine", 120, 1280, 720 );

	Engine::Get()->LoadApplication( new TestRun() );

	Engine::Get()->Run();

	return 0;
}