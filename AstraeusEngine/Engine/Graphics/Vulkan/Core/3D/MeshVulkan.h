#ifndef MESHVULKAN_H
#define MESHVULKAN_H

#include "../../../Core/3D/Mesh.h"

class Mesh_Vulkan : public IMesh
{
public:
	Mesh_Vulkan();
	~Mesh_Vulkan();

	virtual void Render() override final;
	virtual void GenerateBuffers() override final;
private:

};

#endif // !MESHVULKAN_H

