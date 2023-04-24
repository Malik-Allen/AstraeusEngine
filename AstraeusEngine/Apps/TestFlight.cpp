#include "TestFlight.h"
#include "../Engine/World/Objects/Actor.h"
#include "../Engine/Graphics/Core/3D/MeshComponent.h"
#include "../Engine/Graphics/Core/3D/TransformComponent.h"
#include <DebugLog.h>


TestFlight::TestFlight() :
	IApp( "Test Flight" )
{}

TestFlight::~TestFlight()
{}

bool TestFlight::IApp_OnCreate()
{
	Actor* actor1 = GetWorld()->SpawnActor( ActorSpawnInfo( glm::vec3( 20.0f, -13.0f, 0.123f ), glm::vec3( -90.0f, 0.0f, 45.0f ), glm::vec3( 3.0f ) ) );

	//actor1->AddComponent<MeshComponent>( MeshComponentInfo() );

	actor1->AddChild( GetWorld()->SpawnActor() );

	//MeshComponent* meshComponent = actor1->FindComponentByClass<MeshComponent>();

	TransformComponent* transform = actor1->FindComponentByClass<TransformComponent>();

	DEBUG_LOG( LOG::INFO, "TransformComponent:\tPos: {}\tRot: {}\tScale: {}", 
		glm::to_string( transform->GetPosition() ), glm::to_string( transform->GetRotation() ), glm::to_string( transform->GetScale() ) 
	);

	return true;
}

void TestFlight::IApp_OnDestroy()
{}

void TestFlight::IApp_Update( const float deltaTime )
{}
