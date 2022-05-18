#include "Engine/Core/Engine.h"
#include "Apps/TestFlight.h"

int main( int args, char* argv[] )
{
	Engine::Get()->Init( "Astraeus Engine", 120, 1280, 720 );

	Engine::Get()->LoadApplication( new TestFlight() );

	Engine::Get()->Run();

	return 0;
}