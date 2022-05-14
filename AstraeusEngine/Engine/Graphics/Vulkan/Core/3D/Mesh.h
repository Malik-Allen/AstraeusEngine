#pragma once

#include "../../../Core/3D/Mesh.h"

namespace Hephaestus
{
	class Mesh : public IMesh
	{
	public:
		Mesh();
		~Mesh();

		virtual void Render() override final;
		virtual void GenerateBuffers() override final;
	private:

	};
};


