#include "MeshLoader.h"

#include "Mesh.h"

#include <DebugLog.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

MeshData MeshLoader::LoadMesh( const std::string& meshFileName )
{
	std::string relativeFilePath = "./Resources/Models/" + meshFileName;
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;
	DEBUG_LOG( LOG::INFO, "Loading SubMesh @ " + relativeFilePath );
	CONSOLE_LOG( LOG::INFO, "Loading SubMesh @ " + relativeFilePath );

	if( !tinyobj::LoadObj( &attrib, &shapes, &materials, &warn, &err, relativeFilePath.c_str() ) )
	{
		DEBUG_LOG( LOG::ERRORLOG, "Failed to load submesh: Cannot open mesh file @ " + relativeFilePath );
		CONSOLE_LOG( LOG::ERRORLOG, "Failed to load submesh: Cannot open mesh file @ " + relativeFilePath );
		DEBUG_LOG( LOG::ERRORLOG, warn + err );
		CONSOLE_LOG( LOG::ERRORLOG, warn + err );
		throw std::runtime_error( warn + err );
	}

	MeshData meshData;

	for( const auto& shape : shapes )
	{
		SubMesh subMesh;
		for( auto index : shape.mesh.indices )
		{
			Vertex v;
			v.position.x = attrib.vertices[3 * index.vertex_index + 0];
			v.position.y = attrib.vertices[3 * index.vertex_index + 1];
			v.position.z = attrib.vertices[3 * index.vertex_index + 2];

			v.normal.x = attrib.normals[3 * index.normal_index + 0];
			v.normal.y = attrib.normals[3 * index.normal_index + 1];
			v.normal.z = attrib.normals[3 * index.normal_index + 2];

			v.texCoords.x = attrib.texcoords[2 * index.texcoord_index + 0];
			v.texCoords.y = attrib.texcoords[2 * index.texcoord_index + 0];

			v.colour = glm::vec3( 1.0f );

			subMesh.vertexList.push_back( v );
		}
		meshData.subMeshes.push_back( subMesh );
	}

	return meshData;
}
