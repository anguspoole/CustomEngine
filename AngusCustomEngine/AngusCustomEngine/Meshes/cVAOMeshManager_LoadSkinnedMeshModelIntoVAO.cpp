#include "cVAOMeshManager.h"

#include "AssimpFBX/cSimpleAssimpSkinnedMeshLoader_OneMesh.h"

// Takes an FBX Skinned mesh and loads the mesh part into a VAO
bool cVAOMeshManager::LoadSkinnedMeshModelIntoVAO( 
		cSimpleAssimpSkinnedMesh* pLoadedAssimpSkinnedMesh, 
		//sModelDrawInfo &theSMDrawInfo,		/*loads and returns this*/
	    unsigned int shaderProgramID )
{

	sModelDrawInfo theSMDrawInfo;

	theSMDrawInfo.meshFileName = pLoadedAssimpSkinnedMesh->fileName;
	theSMDrawInfo.friendlyName = pLoadedAssimpSkinnedMesh->friendlyName;

	// Copy the data from assimp format into the sModelDrawInfo format...

	theSMDrawInfo.numberOfVertices = pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->mNumVertices;
	theSMDrawInfo.numberOfTriangles = pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->mNumFaces;

	// We used the "triangulate" option when loading so all the primitives
	//	will be triangles, but BEWARE!
	theSMDrawInfo.numberOfIndices = theSMDrawInfo.numberOfTriangles * 3;

	
	// Allocate the vertex array (it's a c-style array)
	theSMDrawInfo.pMeshData = new cMesh();

	theSMDrawInfo.pMeshData->pVertices = new sVertex_xyz_rgba_n_uv2_bt_4Bones[theSMDrawInfo.numberOfVertices];
	
	// Danger Will Robinson! 
	// You don't really need to do this, but at least it will clear it to zero.
	// (compiler will complain that it's 'not safe', etc.)
	memset( theSMDrawInfo.pMeshData->pVertices, 0, 
			sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones) * theSMDrawInfo.numberOfVertices );


	for ( unsigned int vertIndex = 0; vertIndex != theSMDrawInfo.numberOfVertices; vertIndex++ )
	{
		theSMDrawInfo.pMeshData->pVertices[vertIndex].x
			= pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->mVertices[vertIndex].x;
		theSMDrawInfo.pMeshData->pVertices[vertIndex].y
			= pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->mVertices[vertIndex].y;
		theSMDrawInfo.pMeshData->pVertices[vertIndex].z
			= pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->mVertices[vertIndex].z;

		// Normals... 
		theSMDrawInfo.pMeshData->pVertices[vertIndex].nx
			= pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->mNormals[vertIndex].x;
		theSMDrawInfo.pMeshData->pVertices[vertIndex].ny
			= pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->mNormals[vertIndex].y;
		theSMDrawInfo.pMeshData->pVertices[vertIndex].nz
			= pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->mNormals[vertIndex].z;

		// Colours... 
		// (If there are no colours, make it hit pink)
		// Note: the method is because you could have more than one set of 
		//	vertex colours in the model (Why? Who the heck knows?) 
		if ( pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->HasVertexColors(0) )
		{
			theSMDrawInfo.pMeshData->pVertices[vertIndex].r
				= pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->mColors[vertIndex]->r;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].g
				= pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->mColors[vertIndex]->g;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].b
				= pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->mColors[vertIndex]->b;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].a
				= pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->mColors[vertIndex]->a;
		}
		else
		{	// hotpink	#FF69B4	rgb(255,105,180)
			theSMDrawInfo.pMeshData->pVertices[vertIndex].r = 1.0f;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].g = 105.0f/255.0f;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].b = 180.0f/255.0f;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].a = 1.0f;
		}

		//bi-normal  (or bi-tangent)
		theSMDrawInfo.pMeshData->pVertices[vertIndex].bx
			= pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->mBitangents[vertIndex].x;
		theSMDrawInfo.pMeshData->pVertices[vertIndex].by
			= pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->mBitangents[vertIndex].y;
		theSMDrawInfo.pMeshData->pVertices[vertIndex].bz
			= pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->mBitangents[vertIndex].z;

		// Tangent
		theSMDrawInfo.pMeshData->pVertices[vertIndex].tx
			= pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->mTangents[vertIndex].x;
		theSMDrawInfo.pMeshData->pVertices[vertIndex].ty
			= pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->mTangents[vertIndex].y;
		theSMDrawInfo.pMeshData->pVertices[vertIndex].tz
			= pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->mTangents[vertIndex].z;
		
		// uv2 (which are odd in assimp)
		// Note that there is an array of texture coordinates, 
		// up to 8 (actually). Usually, there's only 1
		if ( pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->HasTextureCoords(0) )	// 1st UV coords
		{
			// Assume there's 1... (at least)
			theSMDrawInfo.pMeshData->pVertices[vertIndex].u0 
			= pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->mTextureCoords[0]->x;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].v0
			= pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->mTextureCoords[0]->y;
		}
		if ( pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->HasTextureCoords(1) )	// 2nd UV coords
		{
			theSMDrawInfo.pMeshData->pVertices[vertIndex].u0 
			= pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->mTextureCoords[1]->x;
			theSMDrawInfo.pMeshData->pVertices[vertIndex].v0
			= pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->mTextureCoords[1]->y;
		}
		// TODO: add additional texture coordinates (mTextureCoords[1], etc.)


		// 4Bones: ids and weights
		theSMDrawInfo.pMeshData->pVertices[vertIndex].boneID[0] = pLoadedAssimpSkinnedMesh->vecVertexBoneData[vertIndex].ids[0];
		theSMDrawInfo.pMeshData->pVertices[vertIndex].boneID[1] = pLoadedAssimpSkinnedMesh->vecVertexBoneData[vertIndex].ids[1];
		theSMDrawInfo.pMeshData->pVertices[vertIndex].boneID[2] = pLoadedAssimpSkinnedMesh->vecVertexBoneData[vertIndex].ids[2];
		theSMDrawInfo.pMeshData->pVertices[vertIndex].boneID[3] = pLoadedAssimpSkinnedMesh->vecVertexBoneData[vertIndex].ids[3];

		theSMDrawInfo.pMeshData->pVertices[vertIndex].boneWeights[0] = pLoadedAssimpSkinnedMesh->vecVertexBoneData[vertIndex].weights[0];
		theSMDrawInfo.pMeshData->pVertices[vertIndex].boneWeights[1] = pLoadedAssimpSkinnedMesh->vecVertexBoneData[vertIndex].weights[1];
		theSMDrawInfo.pMeshData->pVertices[vertIndex].boneWeights[2] = pLoadedAssimpSkinnedMesh->vecVertexBoneData[vertIndex].weights[2];
		theSMDrawInfo.pMeshData->pVertices[vertIndex].boneWeights[3] = pLoadedAssimpSkinnedMesh->vecVertexBoneData[vertIndex].weights[3];

		
	}//for ( unsigned int vertIndex = 0;

	// And the triangles

	// Allocate the array to hold the indices (triangle) info


	// Allocate the array for that (indices NOT triangles)
	theSMDrawInfo.pMeshData->pIndices = new unsigned int[theSMDrawInfo.numberOfIndices];
	
	// Danger Will Robinson! 
	// You don't really need to do this, but at least it will clear it to zero.
	// (compiler will complain that it's 'not safe', etc.)
	memset( theSMDrawInfo.pMeshData->pIndices, 0, 
			sizeof(unsigned int) * theSMDrawInfo.numberOfIndices );


	unsigned int numTriangles = pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->mNumFaces;
	unsigned int triIndex = 0;		// Steps through the triangles.
	unsigned int indexIndex = 0;	// Setps through the indices (index buffer)
	for ( ; triIndex != theSMDrawInfo.numberOfTriangles; 
		  triIndex++, indexIndex += 3 )	// Note, every 1 triangle = 3 index steps
	{
		// Get the triangle at this triangle index...
		aiFace* pAIFace = &(pLoadedAssimpSkinnedMesh->pScene->mMeshes[0]->mFaces[triIndex]);

		theSMDrawInfo.pMeshData->pIndices[indexIndex + 0]				// Offset by 0 (zero)
			= pAIFace->mIndices[0];	// vertex 0

		theSMDrawInfo.pMeshData->pIndices[indexIndex + 1]				// Offset by 1
			= pAIFace->mIndices[1];	// vertex 1

		theSMDrawInfo.pMeshData->pIndices[indexIndex + 2]				// Offset by 2
			= pAIFace->mIndices[2];	// vertex 1
	}//for ( ; triIndex != numVertices; 


	// Calculate the extents on the mesh
	// (Note, because I'm a bone head, this is dupicated...)
	theSMDrawInfo.pMeshData->name = theSMDrawInfo.friendlyName;
	theSMDrawInfo.pMeshData->numberOfIndices = theSMDrawInfo.numberOfIndices;
	theSMDrawInfo.pMeshData->numberOfTriangles = theSMDrawInfo.numberOfTriangles;
	theSMDrawInfo.pMeshData->numberOfVertices = theSMDrawInfo.numberOfVertices;
	theSMDrawInfo.pMeshData->CalculateExtents();

	theSMDrawInfo.CopyExtentsFromContainedMeshData();

	// ...then pass it to the mesh manager
	if ( ! this->LoadModelIntoVAO( theSMDrawInfo, shaderProgramID ) )
	{
		return false;
	}
	
	this->m_mapNameToSMModels[ theSMDrawInfo.friendlyName ] = pLoadedAssimpSkinnedMesh;

	return true;
}


cSimpleAssimpSkinnedMesh* cVAOMeshManager::FindSkinnedMeshModelByName( std::string friendlyName )
{
	std::map< std::string /*friendly name*/, cSimpleAssimpSkinnedMesh* >::iterator itSM = this->m_mapNameToSMModels.find(friendlyName);

	if ( itSM == this->m_mapNameToSMModels.end() )
	{
		// Didn't find it
		return NULL;
	}

	return itSM->second;
}
