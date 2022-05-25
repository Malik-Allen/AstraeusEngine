// Copyright (c) 2022 Malik Allen

#include "Engine/Core/Engine.h"
#include "Apps/TestFlight.h"

int main( int args, char* argv[] )
{
	Engine::Get()->Init( Engine_Properties( "Astraeus Engine", Extent( 1280, 720 ), 120, true ) );

	Engine::Get()->LoadApplication( new TestFlight() );

	Engine::Get()->Run();

	return 0;
}