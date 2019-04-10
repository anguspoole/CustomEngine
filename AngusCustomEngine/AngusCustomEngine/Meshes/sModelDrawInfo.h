#ifndef _sModelDrawInfo_HG_
#define _sModelDrawInfo_HG_

// This is used by the mesh manager to actually draw objects that are already loaded into vertex buffers.
// I can hold a copy of the original mesh, if needed.

#include <string>

#include "cMesh.h"

struct sModelDrawInfo
{
	sModelDrawInfo();

	std::string meshFileName;
	std::string friendlyName;	// The one we want to use for actual drawing

	unsigned int VAO_ID;					//***

	unsigned int vertexBufferID;
	unsigned int vertexBuffer_start_index;
	unsigned int numberOfVertices;

	unsigned int indexBufferID;
	unsigned int indexBuffer_start_index;
	unsigned int numberOfIndices;			//***
	unsigned int numberOfTriangles;

	//	// This is the vertex information as read from the file
	//	sPlyVertex* pVerticesFromFile;	// = 0
	//	// These are the original triangle values from file,
	//	//	but in the 'triangle' (or face) format
	//	sPlyTriangle* pTriangles;	// = 0;
	//
	//	// This is the vertex information being passed to the GPU
	//	// (is in format that the shader needs)
	////	sVert_xyz_rgb* pVerticesToVBO;	// = new sVert_xyz_rgb[ARRAYSIZE]
	////	sVert_xyz_rgb_Nxyz* pVerticesToVBO;	// = new sVert_xyz_rgb[ARRAYSIZE]
	//	sVert_xyz_rgb_Nxyz_uv2* pVerticesToVBO;	// = new sVert_xyz_rgb[ARRAYSIZE]
	//	// This buffer is indices as a 1D array (that the GPU needs)
	//	unsigned int* pIndices;		// = new unsigned int[ARRAYSIZE]

	float maxX, maxY, maxZ;
	float minX, minY, minZ;
	float extentX, extentY, extentZ;
	float maxExtent;
	void CopyExtentsFromContainedMeshData(void);

	// ******************************************
	// This is to draw "debug lines" in the scene
	// HACK: Can make this dynamic
	bool bVertexBufferIsDynamic;	// = false by default
	bool bIsIndexedModel;			// = true by default

	// If this is not NULL, then there will be mesh information here.
	// This is if the mesh information needs to be updated.
	cMesh* pMeshData;
};

#endif 
