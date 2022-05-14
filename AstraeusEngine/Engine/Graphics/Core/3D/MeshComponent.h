#ifndef MODEL_H
#define MODEL_H

#include <ECS.h>

#include <string>
#include <vector>

class Camera;
class IMesh;

struct MeshComponentInfo
{
	MeshComponentInfo() :
		m_meshFileName( "" ),
		m_shaderFileName( "" ),
		m_materialFileName( "" ),
		m_textureFileName( "" )
	{};

	MeshComponentInfo( const std::string& meshFileName, const std::string& shaderFileName,
		const std::string& materialFileName, const std::string& textureFileName ) :
		m_meshFileName( meshFileName ),
		m_shaderFileName( shaderFileName ),
		m_materialFileName( materialFileName ),
		m_textureFileName( textureFileName )
	{};

	std::string m_meshFileName;
	std::string m_shaderFileName;
	std::string m_materialFileName;
	std::string m_textureFileName;

};

class MeshComponent : public ECS::Component
{
public:
	static constexpr uint64_t ID = GENERATE_ID( "MeshComponent" );

	MeshComponent( const MeshComponentInfo& meshComponentInfo );
	~MeshComponent();

private:	
	std::vector<IMesh*> m_meshes;

	void LoadMesh( const std::string& meshFileName );
};


#endif // !MODEL_H
