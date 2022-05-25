// Copyright (c) 2022 Malik Allen

#include "Actor.h"

#include "../World.h"
#include "../../Graphics/Core/3D/TransformComponent.h"

#include <DebugLog.h>

Actor::Actor() :
	m_entity( ECS::EntityId() ),
	m_world( nullptr ),
	m_parent( nullptr ),
	m_transformComponent( nullptr )
{}

Actor::~Actor()
{
	for( Actor* child : m_children )
	{
		if( child != nullptr )
		{
			delete child;
			child = nullptr;
		}
	}
	m_children.clear();
}

void Actor::AddChild( Actor* child )
{
	if( child == nullptr )
	{
		DEBUG_LOG( LOG::INFO, "Failed to add child to actor: child is nullptr" );
		return;
	}

	// Remove existing parent if one already exists
	if( child->m_parent != nullptr )
	{
		child->m_parent->RemoveChild( child );
	}

	child->m_parent = this;
	m_children.push_back( child );
}

const std::vector<Actor*>& Actor::GetChildActors() const
{
	return m_children;
}

bool Actor::OnCreate( ActorSpawnInfo actorSpawnInfo /*= ActorSpawnInfo()*/ )
{
	if( m_world != nullptr )
	{
		m_transformComponent = AddComponent<TransformComponent>( actorSpawnInfo.position, 0.0f, actorSpawnInfo.rotation, actorSpawnInfo.scale );
		if( m_transformComponent == nullptr )
		{
			DEBUG_LOG( LOG::INFO, "Failed to create actor: transform component is nullptr" );
			return false;
		}
	}
	return Actor_OnCreate( actorSpawnInfo );
}

void Actor::OnDestroy()
{
	for( Actor* child : m_children )
	{
		if( child != nullptr )
		{
			child->OnDestroy();
		}
	}
	Actor_OnDestroy();
}

void Actor::Update( const float deltaTime )
{
	for( Actor* child : m_children )
	{
		child->Update( deltaTime );
	}
	Actor_Update( deltaTime );
}

bool Actor::Actor_OnCreate( ActorSpawnInfo actorSpawnInfo )
{
	return true;
}

void Actor::Actor_OnDestroy()
{}

void Actor::Actor_Update( const float deltaTime )
{}

void Actor::RemoveChild( Actor* child )
{
	if( child == nullptr )
	{
		return;
	}

	size_t size = m_children.size();
	for( size_t i = 0; i < size; i++ )
	{
		if( m_children[i] != nullptr )
		{
			if( m_children[i]->m_entity == child->m_entity )
			{
				// Now that we found the child to remove, we will replace it with the last child in the children vector
				m_children[i] = m_children[size - 1];
				m_children.pop_back();
			}
		}
	}

	child->m_parent = nullptr;
}


