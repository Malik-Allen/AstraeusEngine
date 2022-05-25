// Copyright (c) 2022 Malik Allen

#include "MeshComponent.h"

#include "../GraphicsAPI.h"
#include "MeshLoader.h"


MeshComponent::MeshComponent( const MeshComponentInfo& meshComponentInfo ) :
	ECS::Component( ID )
{
	LoadMesh( meshComponentInfo.m_meshFileName );
}

MeshComponent::~MeshComponent()
{
	for( IMesh* mesh : m_meshes )
	{
		if( mesh != nullptr )
		{
			delete mesh;
			mesh = nullptr;
		}
	}
	m_meshes.clear();
}

void MeshComponent::LoadMesh( const std::string& meshFileName )
{
	MeshData loadedMeshData = MeshLoader::LoadMesh( meshFileName );
	IMesh* mesh = nullptr;
	for( const auto& subMesh : loadedMeshData.subMeshes )
	{

#if GRAPHICS_API == GRAPHICS_OPENGL
		mesh = new Mesh_OpenGL( subMesh );

#elif GRAPHICS_API == GRAPHICS_VULKAN
		// mesh = new Mesh_Vulkan( subMesh );

#endif
		if( mesh != nullptr )
		{
			m_meshes.push_back( mesh );
		}

	}
}
