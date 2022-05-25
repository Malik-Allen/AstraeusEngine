// Copyright (c) 2022 Malik Allen

#ifndef MESHLOADER_H
#define MESHLOADER_H

#include "Mesh.h"

#include <string>
#include <vector>

/*
*	Wrapper class that handles reading mesh files
*/
class MeshLoader
{
	MeshLoader() = delete;	// Static class, no constructor needed
	MeshLoader( const MeshLoader& ) = delete;
	MeshLoader& operator=( const MeshLoader& ) = delete;
	MeshLoader( MeshLoader&& ) = delete;
	MeshLoader& operator=( MeshLoader&& ) = delete;

public:

	// Loads  from the passed  file name, if the file does not exist or is unreadable, this function returns null
	static MeshData LoadMesh( const std::string& meshFileName );

};


#endif // !MESHLOADER_H
