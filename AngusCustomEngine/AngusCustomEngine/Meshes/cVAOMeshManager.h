#ifndef _cVAOMeshManager_HG_
#define _cVAOMeshManager_HG_

#include <string>
#include <map>
#include <vector>
#include "sModelDrawInfo.h"

// This class is in charge of:
// - taking a cMesh object (loaded from file)
// - loading that mesh data into a vertex and index buffer, 
//   inside a Vertex Array Object (VAO)
// - looking up the rendering (drawing) information for rendering
// - allowing the mesh objects to be "dynamic" if needed

class cSimpleAssimpSkinnedMesh;	// Forward declare (to prevent assimp headers being included here)

class cVAOMeshManager
{
public:
	cVAOMeshManager();		// constructor
	~cVAOMeshManager();		// destructor

	enum eLoadWith
	{
		/*ORIGINAL,*/
		PLY5N,
		ASSIMP
	};
	//void changeLoaderToOriginal(void);
	void changeLoaderToPly5n(void);
	void changeLoaderToAssimp(void);

	// Takes the filename from the drawInfo, and loads the model into the cMesh object inside, then
	//  loads the mesh into the sModelDrawInfo, then into the GPU and VAO
	// Note: the shader program ID is needed to tie the buffer to the vertex layout of the shader
	bool LoadModelIntoVAO(sModelDrawInfo &drawInfo,
		unsigned int shaderProgramID);
	bool LoadModelIntoVAO_ASYNC(sModelDrawInfo &drawInfo,
		unsigned int shaderProgramID);

	// Takes an existing FBX Skinned mesh and loads the mesh part into a VAO
	bool LoadSkinnedMeshModelIntoVAO(cSimpleAssimpSkinnedMesh* pLoadedAssimpSkinnedMesh,
		unsigned int shaderProgramID);

	// Returns a pointer to an existing (loaded) skinned mesh model
	// Returns NULL if not present. 
	cSimpleAssimpSkinnedMesh* FindSkinnedMeshModelByName(std::string friendlyName);

	// Looks up draw info by name (meshFileName)			
	bool FindDrawInfoByModelName(sModelDrawInfo &drawInfo);

	void ShutDown(void);

	std::string GetLastError(bool bClear = true);

	// Update the mesh information, then re-copy to GPU
	// - This will RE-COPY the VERTEX infoarmation into the vertex buffer
	//   (will copy ENTIRE vertex buffer, OVERWRITING what's there)
	void UpdateModelVertexInformation(sModelDrawInfo &drawInfo);

	void SetBasePath(std::string basepath);

	// Rounds up to multiples of 64
	unsigned int m_roundUp(unsigned int numToRound, unsigned int multiple);

	void m_AppendTextToLastError(std::string text, bool addNewLineBefore = true);

	std::string m_basePath;

	eLoadWith m_fileLoader;

	//sModelDrawInfo * currentActiveDrawInfo;

	void m_LockModelNameMap(void);
	void m_UnlockModelNameMap(void);

	//LoadMeshIntoGPUBuffer
	// This takes the information from the cMesh, inside the sModelDrawInfo, and loads it into the VAO
	bool m_LoadMeshInfo_Into_VAO(sModelDrawInfo &drawInfo,
		unsigned int shaderProgramID);

	//unsigned int shaderProgramID;

	// This holds the model information
	std::map< std::string /*model name*/, sModelDrawInfo > m_mapModel_to_VAOID;

	std::vector<std::pair<sModelDrawInfo, unsigned int>> modelIntPairs;

	int currentPair = 0;

	bool m_LoadModelFromFile_ASYNC();

private:

	// LoadPlyFileData()
	bool m_LoadModelFromFile(sModelDrawInfo &drawInfo);
	//bool m_LoadModelFromFile_ASYNC( sModelDrawInfo &drawInfo );
	bool m_LoadModelFromFile_Ply5nLoader(sModelDrawInfo &drawInfo);
	//DWORD WINAPI m_LoadModelFromFile_Ply5nLoader_ASYNC( PVOID pParam );
	bool m_LoadModelFromFile_AssimpLoader(sModelDrawInfo &drawInfo);

	std::map< std::string /*friendly name*/, cSimpleAssimpSkinnedMesh* > m_mapNameToSMModels;

	std::string m_lastErrorString;
};

#endif		// _cVAOMeshManager_HG_
