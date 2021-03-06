//DrawOjbect_calls.cpp
#include "../Global/gOpenGL.h"		// For GLFW and glad (OpenGL calls)
#include "../Global/global.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtx/matrix_decompose.hpp>

#include "../Entity/cEntity.h"

#include "../Meshes/AssimpFBX/cAnimationState.h"

#include <iostream>


bool HACK_bTextureUniformLocationsLoaded = false;
GLint tex00_UniLoc = -1;
GLint tex01_UniLoc = -1;
GLint tex02_UniLoc = -1;
GLint tex03_UniLoc = -1;
GLint tex04_UniLoc = -1;
GLint tex05_UniLoc = -1;
GLint tex06_UniLoc = -1;
GLint tex07_UniLoc = -1;

GLint texBW_0_UniLoc = -1;
GLint texBW_1_UniLoc = -1;

// Texture sampler for off screen texture
GLint texPass1OutputTexture_UniLoc = -1;
//Texture sampler for reticle texture
GLint texPass1ReticleTexture_UniLoc = -1;
GLint normalMap1Texture_UniLoc = -1;


cCamera* currentCamera = NULL;

bool animationComplete = true;
std::string attackAnimation = "";

std::map<std::string /*name*/, cParticleEmitter* > g_map_pParticleEmitters;

// This is to change the full screen FBO objects when the window changes size
// See: http://www.glfw.org/docs/latest/window_guide.html#window_size
void window_size_callback(GLFWwindow* window, int width, int height)
{
	// If no FBO has been made, return
	if (g_pFBOMain)
	{
		if ((g_pFBOMain->width != width) || (g_pFBOMain->height != height))
		{
			// Window size has changed, so resize the offscreen frame buffer object
			std::string error;
			if (!g_pFBOMain->reset(width, height, error))
			{
				std::cout << "In window_size_callback(), the FBO.reset() call returned an error:" << std::endl;
				std::cout << "\t" << error << std::endl;
			}
			else
			{
				std::cout << "Offscreen FBO now: " << width << " x " << height << std::endl;
			}
		}
	}


	if (g_pFBOBlurA)
	{
		if ((g_pFBOBlurA->width != width) || (g_pFBOBlurA->height != height))
		{
			// Window size has changed, so resize the offscreen frame buffer object
			std::string error;
			if (!g_pFBOBlurA->reset(width, height, error))
			{
				std::cout << "In window_size_callback(), the FBO.reset() call returned an error:" << std::endl;
				std::cout << "\t" << error << std::endl;
			}
			else
			{
				std::cout << "Offscreen FBO now: " << width << " x " << height << std::endl;
			}
		}
	}

	if (g_pFBOBlurB)
	{
		if ((g_pFBOBlurB->width != width) || (g_pFBOBlurB->height != height))
		{
			// Window size has changed, so resize the offscreen frame buffer object
			std::string error;
			if (!g_pFBOBlurB->reset(width, height, error))
			{
				std::cout << "In window_size_callback(), the FBO.reset() call returned an error:" << std::endl;
				std::cout << "\t" << error << std::endl;
			}
			else
			{
				std::cout << "Offscreen FBO now: " << width << " x " << height << std::endl;
			}
		}
	}

	if (g_pFBOFinal)
	{
		if ((g_pFBOFinal->width != width) || (g_pFBOFinal->height != height))
		{
			// Window size has changed, so resize the offscreen frame buffer object
			std::string error;
			if (!g_pFBOFinal->reset(width, height, error))
			{
				std::cout << "In window_size_callback(), the FBO.reset() call returned an error:" << std::endl;
				std::cout << "\t" << error << std::endl;
			}
			else
			{
				std::cout << "Offscreen FBO now: " << width << " x " << height << std::endl;
			}
		}
	}

	return;
}



// Will bind the textures in use for this object on this draw call
void BindTextures(cEntity* pCurrentEntity, GLuint shaderProgramID,
	unsigned int RenderPassNumber, cFBO* fbo)
{

	::g_TheTextureManager->CheckForPendingTextureGPULoads();


	// This is pretty much a hack, so we should likely pass the shader object 
	// (pointer) to this function, and to the DrawObject call, too. 
	// (Another option would be to pass the shader MANAGER instead, so 
	//  that the functions can look up various things in the shader)
	//
	// For now, I'm going to get the uniform location here 
	// (to make it clear (maybe?) that we'll NEED those shader uniform locations)

	// So this is only called once... 
	if (!HACK_bTextureUniformLocationsLoaded)
	{
		tex00_UniLoc = glGetUniformLocation(shaderProgramID, "texture00");		// uniform sampler2D texture00;
		tex01_UniLoc = glGetUniformLocation(shaderProgramID, "texture01");		// uniform sampler2D texture01;
		tex02_UniLoc = glGetUniformLocation(shaderProgramID, "texture02");		// uniform sampler2D texture02;
		tex03_UniLoc = glGetUniformLocation(shaderProgramID, "texture03");		// uniform sampler2D texture03;
		tex04_UniLoc = glGetUniformLocation(shaderProgramID, "texture04");		// uniform sampler2D texture04;
		tex05_UniLoc = glGetUniformLocation(shaderProgramID, "texture05");		// uniform sampler2D texture05;
		tex06_UniLoc = glGetUniformLocation(shaderProgramID, "texture06");		// uniform sampler2D texture06;
		tex07_UniLoc = glGetUniformLocation(shaderProgramID, "texture07");		// uniform sampler2D texture07;

		texBW_0_UniLoc = glGetUniformLocation(shaderProgramID, "texBlendWeights[0]");	// uniform vec4 texBlendWeights[2];
		texBW_1_UniLoc = glGetUniformLocation(shaderProgramID, "texBlendWeights[1]");	// uniform vec4 texBlendWeights[2];

		HACK_bTextureUniformLocationsLoaded = true;


		texPass1OutputTexture_UniLoc = glGetUniformLocation(shaderProgramID, "texPass1OutputTexture");
		texPass1ReticleTexture_UniLoc = glGetUniformLocation(shaderProgramID, "texPass1ReticleTexture");
		normalMap1Texture_UniLoc = glGetUniformLocation(shaderProgramID, "normalMap1Texture");

	}//if(!HACK_bTextureUniformLocationsLoaded )



	if (pCurrentEntity->m_EntityMesh->b_HACK_UsesOffscreenFBO)
	{
		// Connect the texture for this object to the FBO texture
		// Pick texture unit 16 (just because - I randomly picked that)

		int FBO_Texture_Unit_Michael_Picked = 1;
		//if (pFBOObj->ID == 1)
		//{
		//	FBO_Texture_Unit_Michael_Picked++;
		//}

		// 0x84C0  (or 33984)		
		// Please bind to texture unit 34,000. Why gawd, why?
		glActiveTexture(GL_TEXTURE0 + FBO_Texture_Unit_Michael_Picked);

		// Connect the specific texture to THIS texture unit
//		glBindTexture( GL_TEXTURE_2D, g_FBO_colourTexture );
		if (RenderPassNumber == 1)
			glBindTexture(GL_TEXTURE_2D, fbo->colourTexture_1_ID);
		else
			glBindTexture(GL_TEXTURE_2D, fbo->colourTexture_0_ID);
		// Now pick to read from the normal (output from the 1st pass):
//		glBindTexture( GL_TEXTURE_2D, ::g_pFBOMain->normalTexture_1_ID );
//		glBindTexture( GL_TEXTURE_2D, ::g_pFBOMain->depthTexture_ID );
//		glBindTexture( GL_TEXTURE_2D, ::g_pFBOMain->vertexWorldPos_2_ID );


		// Set the sampler (in the shader) to ALSO point to texture unit 16
		// This one takes the unchanged texture unit numbers 
//		glUniform1i( tex00_UniLoc, FBO_Texture_Unit_Michael_Picked );
		glUniform1i(texPass1OutputTexture_UniLoc, FBO_Texture_Unit_Michael_Picked);


		// Set the blending to that it's 0th texture sampler
		// NOTE: it's only the 0th (1st) texture that we are mixing from
//		glUniform4f( texBW_0_UniLoc, 1.0f, 0.0f, 0.0f, 0.0f );		// <---- Note the 1.0f
//		glUniform4f( texBW_1_UniLoc, 0.0f, 0.0f, 0.0f, 0.0f );

		if (!pCurrentEntity->m_EntityMesh->vecTextures.empty())
		{
			if (pCurrentEntity->m_EntityMesh->vecTextures[0].strength > 0.2f)
			{
				glActiveTexture(GL_TEXTURE0 + 0);

				// Connect the specific texture to THIS texture unit
				std::string texName = pCurrentEntity->m_EntityMesh->vecTextures[0].name;

				GLuint texID = ::g_TheTextureManager->getTextureIDFromName(texName);

				glBindTexture(GL_TEXTURE_2D, texID);

				if (RenderPassNumber == 1)
					glUniform1i(texPass1OutputTexture_UniLoc, 1);
				else
					glUniform1i(texPass1OutputTexture_UniLoc, 0);
			}
			else
			{
				if (RenderPassNumber == 1)
				{
					glActiveTexture(GL_TEXTURE0 + 0);
					glBindTexture(GL_TEXTURE_2D, fbo->colourTexture_1_ID);
				}
				else
				{
					glActiveTexture(GL_TEXTURE0 + 0);
					glBindTexture(GL_TEXTURE_2D, fbo->colourTexture_0_ID);
				}

				glUniform1i(texPass1ReticleTexture_UniLoc, FBO_Texture_Unit_Michael_Picked);
			}
		}
		else
		{

			if (RenderPassNumber == 1)
			{
				glActiveTexture(GL_TEXTURE0 + 0);
				glBindTexture(GL_TEXTURE_2D, fbo->colourTexture_1_ID);
			}
			else if (RenderPassNumber != 5)
			{
				glActiveTexture(GL_TEXTURE0 + 0);
				glBindTexture(GL_TEXTURE_2D, fbo->colourTexture_0_ID);
			}

			if (RenderPassNumber == 5)
			{
				glActiveTexture(GL_TEXTURE0 + 0);
				glBindTexture(GL_TEXTURE_2D, g_pFBOMain->colourTexture_0_ID);

				glActiveTexture(GL_TEXTURE0 + FBO_Texture_Unit_Michael_Picked);
				glBindTexture(GL_TEXTURE_2D, fbo->colourTexture_0_ID);
			}
			glUniform1i(texPass1ReticleTexture_UniLoc, FBO_Texture_Unit_Michael_Picked);
		}



		// NOTE: Early return (so we don't set any other textures
		// Again; HACK!!
		return;
	}//if ( pCurrentMesh->b_HACK_UsesOffscreenFBO )
	//// ******************************************************************** 
	//// ******************************************************************** 
	//// ******************************************************************** 



	// Set all the blend weights (strengths) to zero
	float blendWeights[8] = { 0 };

	int texBindIndex = 0;
	for (texBindIndex = 0; texBindIndex != pCurrentEntity->m_EntityMesh->vecTextures.size(); texBindIndex++)
	{
		//int actualTexBindIndex = texBindIndex;

		//if ( ::g_pTheTextureManager->isTextureCubeMap(pCurrentMesh->vecTextures[texBindIndex].name) )
		//{
		//	actualTexBindIndex += BIND_INDEX_OFFSET_CUBE_TEXTURES;
		//}

		//// Bind to the the "texBindIndex" texture unit
		glActiveTexture(GL_TEXTURE0 + texBindIndex);

		// Connect the specific texture to THIS texture unit
		std::string texName = pCurrentEntity->m_EntityMesh->vecTextures[texBindIndex].name;

		GLuint texID = ::g_TheTextureManager->getTextureIDFromName(texName);

		glBindTexture(GL_TEXTURE_2D, texID);

		// Use a switch to pick the texture sampler and weight (strength)
		// BECAUSE the samplers can't be in an array
		switch (texBindIndex)
		{
		case 0:		// uniform sampler2D texture00  AND texBlendWeights[0].x;
			glUniform1i(tex00_UniLoc, texBindIndex);
			break;
		case 1:		// uniform sampler2D texture01  AND texBlendWeights[0].y;
			glUniform1i(tex01_UniLoc, texBindIndex);
			break;
		case 2:
			if (pCurrentEntity->m_EntityMesh->bHasNormalMap)
			{
				glUniform1i(normalMap1Texture_UniLoc, texBindIndex);
			}
			else
			{
				glUniform1i(tex02_UniLoc, texBindIndex);
			}
			break;
		case 3:
			glUniform1i(tex03_UniLoc, texBindIndex);
			break;
		case 4:		// uniform sampler2D texture04  AND texBlendWeights[1].x;
			glUniform1i(tex04_UniLoc, texBindIndex);
			break;
		case 5:
			glUniform1i(tex05_UniLoc, texBindIndex);
			break;
		case 6:
			glUniform1i(tex06_UniLoc, texBindIndex);
			break;
		case 7:
			glUniform1i(tex07_UniLoc, texBindIndex);
			break;
		}//switch ( texBindIndex )

		// Set the blend weight (strengty)
		blendWeights[texBindIndex] = pCurrentEntity->m_EntityMesh->vecTextures[texBindIndex].strength;

	}//for ( int texBindIndex

	// Set the weights (strengths) in the shader
	glUniform4f(texBW_0_UniLoc, blendWeights[0], blendWeights[1], blendWeights[2], blendWeights[3]);
	glUniform4f(texBW_1_UniLoc, blendWeights[4], blendWeights[5], blendWeights[6], blendWeights[7]);

	return;
}

void CalculateBonesAndAnimation(cEntity* pCurrentEntity, unsigned int numBonesUsed_UniLoc, unsigned int bones_UniLoc)
{
	// Also pass up the bone information...
	std::vector< glm::mat4x4 > vecFinalTransformation;	// Replaced by	theMesh.vecFinalTransformation
	std::vector< glm::mat4x4 > vecOffsets;

	//		cAnimationState* pAniState = pCurrentMesh->pAniState->;
			// Are there any animations in the queue?
	//		if ( pCurrentMesh->pAniState->vecAnimationQueue.empty() )

	//float customDuration = mapAnimToDuration[pCurrentEntity->m_EntityMesh->currentAnimation];
	if (pCurrentEntity->animTime > pCurrentEntity->m_EntityMesh->pSimpleSkinnedMesh->GetDuration(pCurrentEntity->m_EntityMesh->currentAnimation))
	{
		pCurrentEntity->animTime = 0.0f;
	}
	if (pCurrentEntity->animTime < 0.01f)
	{
		if (pCurrentEntity->m_EntityMesh->pAniState->vecAnimationQueue.size() > 0)
		{
			//get name of next animation
			pCurrentEntity->m_EntityMesh->currentAnimation = pCurrentEntity->m_EntityMesh->pAniState->vecAnimationQueue[0].name;
			//erase from queue
			pCurrentEntity->status = pCurrentEntity->m_EntityMesh->pAniState->vecAnimationQueue[0].status;
			pCurrentEntity->m_EntityMesh->pAniState->vecAnimationQueue.erase(pCurrentEntity->m_EntityMesh->pAniState->vecAnimationQueue.begin());
			pCurrentEntity->animTime = 0.00f;
		}
		else //queue is empty
		{
			if (pCurrentEntity->status == eEntityStatus::DEAD)
			{
				//pCurrentEntity->animTime = customDuration - 0.001f; //set the animation time to right before the end
			}
			else
			{
				//set to idle
				pCurrentEntity->status = eEntityStatus::IDLE;
				pCurrentEntity->m_EntityMesh->currentAnimation = pCurrentEntity->m_EntityMesh->pAniState->defaultAnimation.name;
			}
		}
		animationComplete = true;
	}

	pCurrentEntity->m_EntityMesh->pSimpleSkinnedMesh->BoneTransform(
		//0.0f,	// curFrameTime,
		pCurrentEntity->animTime,	// curFrameTime,	
		pCurrentEntity->m_EntityMesh->currentAnimation, // animationToPlay,
		vecFinalTransformation,		// Final bone transforms for mesh
		pCurrentEntity->m_EntityMesh->vecObjectBoneTransformation,  // final location of bones
		vecOffsets);                 // local offset for each bone

	if ((pCurrentEntity->status != eEntityStatus::DEAD))
	{
		pCurrentEntity->animTime += deltaTime;		// Frame time, but we are going at 60HZ
	}
	else if (pCurrentEntity->animTime + deltaTime < pCurrentEntity->m_EntityMesh->pSimpleSkinnedMesh->GetDuration(pCurrentEntity->m_EntityMesh->currentAnimation))
	{
		pCurrentEntity->animTime += deltaTime;
	}

	unsigned int numberOfBonesUsed = static_cast<unsigned int>(vecFinalTransformation.size());

	glUniform1i(numBonesUsed_UniLoc, numberOfBonesUsed);

	glm::mat4x4* pBoneMatrixArray = &(vecFinalTransformation[0]);

	//		std::cout << "bones_UniLoc: " << bones_UniLoc << std::endl;	std::cout.flush();
	glUniformMatrix4fv(bones_UniLoc, numberOfBonesUsed, GL_FALSE,
		(const GLfloat*)glm::value_ptr(*pBoneMatrixArray));

	// Update the extents of the skinned mesh from the bones...
	//	sMeshDrawInfo.minXYZ_from_SM_Bones(glm::vec3(0.0f)), 
	//  sMeshDrawInfo.maxXYZ_from_SM_Bones(glm::vec3(0.0f))
	for (unsigned int boneIndex = 0; boneIndex != numberOfBonesUsed; boneIndex++)
	{
		glm::mat4 boneLocal = pCurrentEntity->m_EntityMesh->vecObjectBoneTransformation[boneIndex];
		float scale = pCurrentEntity->m_EntityPhysics->nonUniformScale.x;
		glm::mat4 boneScale = boneLocal * scale;
		glm::mat4 boneRotation;
		pCurrentEntity->m_EntityPhysics->rigidBody->GetOrientation(boneRotation);

		boneRotation = boneRotation * boneScale;

		glm::vec4 boneBallLocation = boneRotation * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		// Update the extents of the mesh
		if (boneIndex == 0)
		{
			// For the 0th bone, just assume this is the extent
			pCurrentEntity->m_EntityMesh->minXYZ_from_SM_Bones = glm::vec3(boneBallLocation);
			pCurrentEntity->m_EntityMesh->maxXYZ_from_SM_Bones = glm::vec3(boneBallLocation);
		}
		else
		{	// It's NOT the 0th bone, so compare with current max and min
			if (pCurrentEntity->m_EntityMesh->minXYZ_from_SM_Bones.x < boneBallLocation.x) { pCurrentEntity->m_EntityMesh->minXYZ_from_SM_Bones.x = boneBallLocation.x; }
			if (pCurrentEntity->m_EntityMesh->minXYZ_from_SM_Bones.y < boneBallLocation.y) { pCurrentEntity->m_EntityMesh->minXYZ_from_SM_Bones.y = boneBallLocation.y; }
			if (pCurrentEntity->m_EntityMesh->minXYZ_from_SM_Bones.z < boneBallLocation.z) { pCurrentEntity->m_EntityMesh->minXYZ_from_SM_Bones.z = boneBallLocation.z; }

			if (pCurrentEntity->m_EntityMesh->maxXYZ_from_SM_Bones.x > boneBallLocation.x) { pCurrentEntity->m_EntityMesh->maxXYZ_from_SM_Bones.x = boneBallLocation.x; }
			if (pCurrentEntity->m_EntityMesh->maxXYZ_from_SM_Bones.y > boneBallLocation.y)
			{
				pCurrentEntity->m_EntityMesh->maxXYZ_from_SM_Bones.y = boneBallLocation.y;
			}
			if (pCurrentEntity->m_EntityMesh->maxXYZ_from_SM_Bones.z > boneBallLocation.z)
			{
				pCurrentEntity->m_EntityMesh->maxXYZ_from_SM_Bones.z = boneBallLocation.z;
			}
		}//if ( boneIndex == 0 )
	}

	std::map<int, cEntity*>::iterator equipIT;

	for (equipIT = pCurrentEntity->boneObjectMap.begin(); equipIT != pCurrentEntity->boneObjectMap.end(); equipIT++)
	{
		int boneID = equipIT->first;
		cEntity* equipment = equipIT->second;
		glm::mat4 boneT = pCurrentEntity->m_EntityMesh->vecObjectBoneTransformation[boneID];

		glm::mat4 boneRotation;
		pCurrentEntity->m_EntityPhysics->rigidBody->GetOrientation(boneRotation);

		equipment->m_EntityPhysics->rigidBody->SetOrientation(boneRotation * boneT);

		glm::vec3 offset = boneRotation * boneT * glm::vec4(equipment->m_EntityPhysics->offset, 1.0f);
		glm::vec3 newPos = offset * pCurrentEntity->m_EntityPhysics->nonUniformScale.x 
			+ pCurrentEntity->m_EntityPhysics->position;
		equipment->m_EntityPhysics->rigidBody->SetPosition(newPos);

	}

}

void DrawScene_Simple(std::vector<cEntity*> vec_pEntities,
	GLuint shaderProgramID,
	unsigned int passNumber, cFBO* fbo)
{
	//bool debugging = false;

	//if (!debugging)
	{
		for (unsigned int objIndex = 0;
			objIndex != (unsigned int)vec_pEntities.size();
			objIndex++)
		{
			cEntity* pCurrentEntity = vec_pEntities[objIndex];

			glm::mat4x4 matModel = glm::mat4(1.0f);			// mat4x4 m, p, mvp;

			DrawObject(pCurrentEntity, matModel, shaderProgramID, passNumber, fbo);

		}//for ( unsigned int objIndex = 0; 
	}

	return;
}

static float g_HACK_blend_ratio = 0.0f;

void resetHackTime(cEntity* entity)
{
	entity->animTime = 0.0f; //set to negative so that it will be reset to 0 in draw loop
}

void DrawObject(cEntity* pCurrentEntity,
	glm::mat4x4 &matModel,
	GLuint shaderProgramID,
	unsigned int RenderPassNumber, /*= 0*/
	cFBO* fbo)
{

	// Is this object visible
	if (!pCurrentEntity->m_EntityMesh->bIsVisible)
	{
		return;
	}



	// Set up the texture binding for this object
	BindTextures(pCurrentEntity, shaderProgramID, RenderPassNumber, fbo);



	//************************************
	//	matModel = glm::mat4x4(1.0f);		// mat4x4_identity(m);


	glm::mat4 matTranslation;

	//pCurrentMesh->rigidBody->GetTransform(matTranslation);

	if (pCurrentEntity->m_EntityPhysics->physObjType == cEntityPhysics::ePhysicsObjType::RIGID_BODY)
	{
		//glm::vec3 pos;
		pCurrentEntity->m_EntityPhysics->rigidBody->GetPosition(pCurrentEntity->m_EntityPhysics->position);

		if (pCurrentEntity->m_EntityMesh->pSimpleSkinnedMesh)
		{
			nPhysics::iShape* rbShape = pCurrentEntity->m_EntityPhysics->rigidBody->GetIShape();
			if (rbShape->GetShapeType() == nPhysics::eShapeType::SHAPE_TYPE_CAPSULE)
			{
				nPhysics::iCapsuleShape* capShape = dynamic_cast<nPhysics::iCapsuleShape*>(rbShape);
				pCurrentEntity->m_EntityPhysics->position.y -= capShape->GetHeight() + capShape->GetRadius();
			}
		}

		//pCurrentEntity->m_EntityPhysics->rigidBody->GetTransform(matTranslation);
		matTranslation = glm::translate(glm::mat4(1.0f), pCurrentEntity->m_EntityPhysics->position);
	}
	else if (pCurrentEntity->m_EntityPhysics->physObjType == cEntityPhysics::ePhysicsObjType::SOFT_BODY)
	{
		for (int i = 0; i < pCurrentEntity->m_EntityPhysics->softBody->NumNodes(); i++)
		{
			//Render a cloth node at each appropriate position
			cEntity * clothNode = findObjectByFriendlyName("ClothNode");
			float scale;
			pCurrentEntity->m_EntityPhysics->softBody->GetNodeRadius(i, scale);
			clothNode->m_EntityPhysics->setUniformScale(scale);
			pCurrentEntity->m_EntityPhysics->softBody->GetNodePosition(i, clothNode->m_EntityPhysics->position); //update position
			pCurrentEntity->m_EntityMesh->bIsVisible = true; //set visible to true
			glm::mat4x4 clothMatModel = glm::mat4x4(1.0f);		// mat4x4_identity(m);
			DrawObject(clothNode, clothMatModel, shaderProgramID, RenderPassNumber, fbo);
			//pCurrentMesh->bIsVisible = false; //reset visible to false
		}
		return;
	}
	else if (pCurrentEntity->m_EntityPhysics->physObjType == cEntityPhysics::ePhysicsObjType::SOFT_BODY_NODE)
	{
		matTranslation = glm::translate(glm::mat4(1.0f), pCurrentEntity->m_EntityPhysics->position);
	}
	else
	{
		matTranslation = glm::translate(glm::mat4(1.0f), pCurrentEntity->m_EntityPhysics->position);
	}

	matModel = matModel * matTranslation;		// matMove

	if (pCurrentEntity->m_EntityPhysics->physObjType == cEntityPhysics::ePhysicsObjType::RIGID_BODY)
	{
		//glm::mat4 rigidTrans;
		//pCurrentEntity->m_EntityPhysics->rigidBody->GetTransform(rigidTrans);

		glm::mat4 matQrotation(1.0f);
		pCurrentEntity->m_EntityPhysics->rigidBody->GetOrientation(matQrotation);
		matModel = matModel * matQrotation;
	}
	else
	{
		glm::quat qRotation = pCurrentEntity->m_EntityPhysics->getQOrientation();
		// Generate the 4x4 matrix for that
		glm::mat4 matQrotation = glm::mat4(qRotation);

		matModel = matModel * matQrotation;
	}

	// Calculate the inverse transpose before the scaling
	glm::mat4 matModelInvTrans = glm::inverse(glm::transpose(matModel));

	// And now scale

	glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
		glm::vec3(pCurrentEntity->m_EntityPhysics->nonUniformScale.x, pCurrentEntity->m_EntityPhysics->nonUniformScale.y, pCurrentEntity->m_EntityPhysics->nonUniformScale.z));
	matModel = matModel * matScale;

	//************************************

	glUseProgram(shaderProgramID);

	// HACK: We wil deal with these uniform issues later...

	// Loading the uniform variables here (rather than the inner draw loop)
//	GLint objectColour_UniLoc = glGetUniformLocation( shaderProgramID, "objectColour" );
	GLint objectDiffuse_UniLoc = glGetUniformLocation(shaderProgramID, "objectDiffuse");
	GLint objectSpecular_UniLoc = glGetUniformLocation(shaderProgramID, "objectSpecular");

	//uniform vec3 lightPos;
	//uniform float lightAtten;
	GLint lightPos_UniLoc = glGetUniformLocation(shaderProgramID, "lightPos");
	GLint lightBrightness_UniLoc = glGetUniformLocation(shaderProgramID, "lightBrightness");
	GLint useVertexColour_UniLoc = glGetUniformLocation(shaderProgramID, "useVertexColour");

	//	// uniform mat4 MVP;	THIS ONE IS NO LONGER USED	
	//uniform mat4 matModel;	// M
	//uniform mat4 matView;		// V
	//uniform mat4 matProj;		// P
	//GLint mvp_location = glGetUniformLocation(program, "MVP");
	GLint matModel_location = glGetUniformLocation(shaderProgramID, "matModel");
	GLint matModelInvTrans_location = glGetUniformLocation(shaderProgramID, "matModelInvTrans");
	GLint matView_location = glGetUniformLocation(shaderProgramID, "matView");
	GLint matProj_location = glGetUniformLocation(shaderProgramID, "matProj");

	GLint bDontUseLighting_UniLoc = glGetUniformLocation(shaderProgramID, "bDontUseLighting");
	GLint bHasNormalMap_UniLoc = glGetUniformLocation(shaderProgramID, "bHasNormalMap");

	glUniformMatrix4fv(matModel_location, 1, GL_FALSE, glm::value_ptr(matModel));
	glUniformMatrix4fv(matModelInvTrans_location, 1, GL_FALSE, glm::value_ptr(matModelInvTrans));


	// Set the object to "wireframe"
	// glPolygonMode( GL_FRONT_AND_BACK , GL_LINE );	//GL_FILL
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	//GL_FILL

	// ***************************************************

	// I'll do quick sort or whatever sexy sorts
	// One pass of bubble sort is fine...

	// Enable blend or "alpha" transparency
	glEnable(GL_BLEND);

	//glDisable( GL_BLEND );
	// Source == already on framebuffer
	// Dest == what you're about to draw
	// This is the "regular alpha blend transparency"
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLint wholeObjectAlphaTransparency_LocID = glGetUniformLocation(shaderProgramID,
		"wholeObjectAlphaTransparency");
	// Take the 4th value from the material and pass it as alpha
	glUniform1f(wholeObjectAlphaTransparency_LocID, pCurrentEntity->m_EntityMesh->materialDiffuse.a);

	// ****************************************************


	glUniform4f(objectDiffuse_UniLoc,
		pCurrentEntity->m_EntityMesh->materialDiffuse.r,
		pCurrentEntity->m_EntityMesh->materialDiffuse.g,
		pCurrentEntity->m_EntityMesh->materialDiffuse.b,
		pCurrentEntity->m_EntityMesh->materialDiffuse.a);
	glUniform4f(objectSpecular_UniLoc,
		pCurrentEntity->m_EntityMesh->materialSpecular.r,
		pCurrentEntity->m_EntityMesh->materialSpecular.g,
		pCurrentEntity->m_EntityMesh->materialSpecular.b,
		pCurrentEntity->m_EntityMesh->materialSpecular.a);

	if (pCurrentEntity->m_EntityMesh->bUseVertexColour)
	{
		glUniform1f(useVertexColour_UniLoc, (float)GL_TRUE);
	}
	else
	{
		glUniform1f(useVertexColour_UniLoc, (float)GL_FALSE);
	}

	if (pCurrentEntity->m_EntityMesh->bDontLight)
	{
		glUniform1f(bDontUseLighting_UniLoc, (float)GL_TRUE);
	}
	else
	{
		glUniform1f(bDontUseLighting_UniLoc, (float)GL_FALSE);
	}

	if (pCurrentEntity->m_EntityMesh->bHasNormalMap)
	{
		glUniform1f(bHasNormalMap_UniLoc, (float)GL_TRUE);
	}
	else
	{
		glUniform1f(bHasNormalMap_UniLoc, (float)GL_FALSE);
	}

	if (pCurrentEntity->m_EntityMesh->bIsWireFrame)
	{
		// Yes, draw it wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);	// Discared "back facing" triangles
		//glDisable( GL_DEPTH );		// Enables the KEEPING of the depth information
		//glDisable( GL_DEPTH_TEST );	// When drawing, checked the existing depth
	}
	else
	{
		// No, it's "solid" (or "Filled")
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);	// Discared "back facing" triangles
		//glEnable( GL_DEPTH );		// Enables the KEEPING of the depth information
		//glEnable( GL_DEPTH_TEST );	// When drawing, checked the existing depth
	}

	// *****************************************************************
	//  ___ _   _                  _ __  __        _    
	// / __| |_(_)_ _  _ _  ___ __| |  \/  |___ __| |_  
	// \__ \ / / | ' \| ' \/ -_) _` | |\/| / -_|_-< ' \ 
	// |___/_\_\_|_||_|_||_\___\__,_|_|  |_\___/__/_||_|
	//                                                  
	GLint bIsASkinnedMesh_LocID = glGetUniformLocation(shaderProgramID,
		"bIsASkinnedMesh");

	// Is this a skinned mesh model or a "regular" static one?
	sModelDrawInfo modelInfo;
	if (pCurrentEntity->m_EntityMesh->pSimpleSkinnedMesh == NULL)
	{
		// It's a "regular" mesh
//		modelInfo.meshFileName = pCurrentMesh->meshName;

		// For now, pick the 0th one

		float camObjectDistance = glm::distance(currentCamera->eye,
			pCurrentEntity->m_EntityPhysics->position);

		// Look for a model that is the closest to the camera but within that
		// "minDistance" value
		// Assume it's the 1st one (0th one)
		std::vector<sLODInfo>::iterator itMesh = pCurrentEntity->m_EntityMesh->vecLODMeshs.begin();
		modelInfo.meshFileName = itMesh->meshName;

		for (; itMesh != pCurrentEntity->m_EntityMesh->vecLODMeshs.end(); itMesh++)
		{
			// is this model CLOSER than the min distance to draw? 
			if (camObjectDistance < itMesh->minDistance)
			{
				// This one is closer, so draw it
				modelInfo.meshFileName = itMesh->meshName;
			}
		}



		glUniform1f(bIsASkinnedMesh_LocID, (float)GL_FALSE);
	}
	else
	{
		GLint numBonesUsed_UniLoc = glGetUniformLocation(shaderProgramID, "numBonesUsed");

		// UniformLoc_bonesArray is the getUniformLoc of "bones[0]" from
		//	uniform mat4 bones[MAXNUMBEROFBONES] 
		// in the shader
		GLint bones_UniLoc = glGetUniformLocation(shaderProgramID, "bones");

		CalculateBonesAndAnimation(pCurrentEntity, numBonesUsed_UniLoc, bones_UniLoc);

		// It ++IS++ skinned mesh
		modelInfo.meshFileName = pCurrentEntity->m_EntityMesh->pSimpleSkinnedMesh->fileName;

		glUniform1f(bIsASkinnedMesh_LocID, (float)GL_TRUE);

	}//if ( pCurrentMesh->pSimpleSkinnedMesh == NULL )

	//if ( pCurrentMesh->bIsALightVolume )
	//{
	//	// Draw the back facing polygons, too
	//	glDisable( GL_CULL_FACE );	// Draw back facing triangles, as well
	//}
	//else
	//{
	//	glEnable( GL_CULL_FACE );	// Discared "back facing" triangles
	//}



	if (g_VAOMeshManager->FindDrawInfoByModelName(modelInfo))
	{
		//glDrawArrays(GL_TRIANGLES, 0, bunnyInfo.numberOfIndices );

		glBindVertexArray(modelInfo.VAO_ID);

		//glDrawElements( GL_TRIANGLES, 
		//				modelInfo.numberOfIndices,
		//				GL_UNSIGNED_INT, 
		//				0 );		// Specifies an offset of the first index in the array in the data store 
		//							//	of the buffer currently bound to the GL_ELEMENT_ARRAY_BUFFER target.

		glDrawElementsBaseVertex(GL_TRIANGLES,
			modelInfo.numberOfIndices,
			GL_UNSIGNED_INT,
			0,
			0);

		glBindVertexArray(0);

	}
	else
	{
		//		std::cout << pCurrentMesh->meshName << " was not found" << std::endl;
		std::cout << pCurrentEntity->m_EntityMesh->vecLODMeshs[0].meshName << " was not found" << std::endl;
	}


	for (unsigned int childEntityIndex = 0; childEntityIndex != pCurrentEntity->vec_pChildrenEntities.size(); childEntityIndex++)
	{
		//glm::mat4 matWorldParent = matModel;
		glm::mat4 matWorldParent = glm::mat4(1.0f);
		DrawObject(pCurrentEntity->vec_pChildrenEntities[childEntityIndex], matWorldParent, shaderProgramID, RenderPassNumber, fbo);
	}

	/*if (fbo != NULL)
	{
		makeBlur(shaderProgramID, fbo);
	}*/

	return;
}//void DrawObject(void)

// Draws any particle emitters that are active
void updateAndDrawParticles(double deltaTime,
	GLuint shaderProgramID,
	glm::vec3 cameraEye)
{

	// These GetUniformLocation() calls should NOT be in the draw call
	// (you should get these at the start and cache them in the cShaderObject, perhaps)
	GLint bIsParticleImposter_UniLoc = glGetUniformLocation(shaderProgramID, "bIsParticleImposter");
	GLint ParticleImposterAlphaOverride_UniLoc = glGetUniformLocation(shaderProgramID, "ParticleImposterAlphaOverride");
	GLint ParticleImposterBlackThreshold_UniLoc = glGetUniformLocation(shaderProgramID, "ParticleImposterBlackThreshold");

	// Black threshold is where the imposter will discard 
	// i.e. At or below this value, the imposter isn't draw. 
	// (range is from 0.0 to 1.0)
	glUniform1f(ParticleImposterBlackThreshold_UniLoc, 0.25f);

	// STARTOF: Star shaped smoke particle
	std::map<std::string /*name*/, cParticleEmitter* >::iterator itPE_Smoke01
		= ::g_map_pParticleEmitters.find("Smoke01");

	if (itPE_Smoke01 != ::g_map_pParticleEmitters.end())
	{

		cParticleEmitter* pPE_Smoke01 = itPE_Smoke01->second;

		// Update the particle emitter
		cEntity* pParticleMesh = findObjectByFriendlyName("SmokeObjectStar");
		glm::mat4 matParticleIndentity = glm::mat4(1.0f);
		glm::vec3 oldPosition = pParticleMesh->m_EntityPhysics->position;
		glm::quat oldOrientation = pParticleMesh->m_EntityPhysics->getQOrientation();
		glm::vec3 oldScale = pParticleMesh->m_EntityPhysics->nonUniformScale;

		pParticleMesh->m_EntityPhysics->setMeshOrientationEulerAngles(0.0f, 0.0f, 0.0f);
		pParticleMesh->m_EntityMesh->bIsVisible = true;


		// Set up the shader
		glUniform1f(bIsParticleImposter_UniLoc, (float)GL_TRUE);


		pPE_Smoke01->Update(deltaTime);

		std::vector<sParticle> vecParticlesToDraw;
		pPE_Smoke01->getAliveParticles(vecParticlesToDraw);

		pPE_Smoke01->sortParticlesBackToFront(vecParticlesToDraw, cameraEye);

		unsigned int numParticles = (unsigned int)vecParticlesToDraw.size();
		//			std::cout << "Drawing " << numParticles << " particles" << std::end;

		unsigned int count = 0;
		for (unsigned int index = 0; index != numParticles; index++)
		{
			if (vecParticlesToDraw[index].lifeRemaining > 0.0f)
			{
				// Draw it
				pParticleMesh->m_EntityPhysics->position = vecParticlesToDraw[index].position;
				pParticleMesh->m_EntityPhysics->setUniformScale(vecParticlesToDraw[index].scale);
				pParticleMesh->m_EntityPhysics->setQOrientation(vecParticlesToDraw[index].qOrientation);

				// This is for the "death" transparency
				glUniform1f(ParticleImposterAlphaOverride_UniLoc, vecParticlesToDraw[index].transparency);

				DrawObject(pParticleMesh, matParticleIndentity, shaderProgramID, 0, g_pFBOMain);
				count++;
			}
		}
		//			std::cout << "Drew " << count << " particles" << std::endl;
		pParticleMesh->m_EntityMesh->bIsVisible = false;
		pParticleMesh->m_EntityPhysics->position = oldPosition;
		pParticleMesh->m_EntityPhysics->setQOrientation(oldOrientation);
		pParticleMesh->m_EntityPhysics->nonUniformScale = oldScale;
		glUniform1f(bIsParticleImposter_UniLoc, (float)GL_FALSE);
		glUniform1f(ParticleImposterAlphaOverride_UniLoc, 1.0f);
		// ***************************************************************************
	}
	// ENDOF: Star shaped smoke particle


	// STARTOF: flat 2D smoke particle
	std::map<std::string /*name*/, cParticleEmitter* >::iterator itPE_Smoke02
		= ::g_map_pParticleEmitters.find("Fire01");

	if (itPE_Smoke02 != ::g_map_pParticleEmitters.end())
	{

		cParticleEmitter* pPE_Smoke02 = itPE_Smoke02->second;

		// Update the particle emitter
		cEntity* pParticleMesh = findObjectByFriendlyName("FireObjectStar");
		glm::mat4 matParticleIndentity = glm::mat4(1.0f);
		glm::vec3 oldPosition = pParticleMesh->m_EntityPhysics->position;
		glm::quat oldOrientation = pParticleMesh->m_EntityPhysics->getQOrientation();
		glm::vec3 oldScale = pParticleMesh->m_EntityPhysics->nonUniformScale;

		pParticleMesh->m_EntityPhysics->setMeshOrientationEulerAngles(0.0f, 0.0f, 0.0f);
		pParticleMesh->m_EntityMesh->bIsVisible = true;


		// Set up the shader
		glUniform1f(bIsParticleImposter_UniLoc, (float)GL_TRUE);


		pPE_Smoke02->Update(deltaTime);

		std::vector<sParticle> vecParticlesToDraw;
		pPE_Smoke02->getAliveParticles(vecParticlesToDraw);

		pPE_Smoke02->sortParticlesBackToFront(vecParticlesToDraw, cameraEye);

		unsigned int numParticles = (unsigned int)vecParticlesToDraw.size();
		//			std::cout << "Drawing " << numParticles << " particles" << std::end;

		unsigned int count = 0;
		for (unsigned int index = 0; index != numParticles; index++)
		{
			if (vecParticlesToDraw[index].lifeRemaining > 0.0f)
			{
				// Draw it
				pParticleMesh->m_EntityPhysics->position = vecParticlesToDraw[index].position;
				pParticleMesh->m_EntityPhysics->setUniformScale(vecParticlesToDraw[index].scale);
				pParticleMesh->m_EntityPhysics->setQOrientation(vecParticlesToDraw[index].qOrientation);

				// This is for the "death" transparency
				glUniform1f(ParticleImposterAlphaOverride_UniLoc, vecParticlesToDraw[index].transparency);

				DrawObject(pParticleMesh, matParticleIndentity, shaderProgramID, 0, g_pFBOMain);
				count++;
			}
		}
		//			std::cout << "Drew " << count << " particles" << std::endl;
		pParticleMesh->m_EntityMesh->bIsVisible = false;
		pParticleMesh->m_EntityPhysics->position = oldPosition;
		pParticleMesh->m_EntityPhysics->setQOrientation(oldOrientation);
		pParticleMesh->m_EntityPhysics->nonUniformScale = oldScale;
		glUniform1f(bIsParticleImposter_UniLoc, (float)GL_FALSE);
		glUniform1f(ParticleImposterAlphaOverride_UniLoc, 1.0f);
		// ***************************************************************************
	}
	// ENDOF: Star shaped smoke particle




	// STARTOF: flat 2D plasma explosion
	std::map<std::string /*name*/, cParticleEmitter* >::iterator itPE_Plasma_01
		= ::g_map_pParticleEmitters.find("PlasmaExplosion");

	if (itPE_Plasma_01 != ::g_map_pParticleEmitters.end())
	{

		cParticleEmitter* pPE_Plasma_01 = itPE_Plasma_01->second;

		// Update the particle emitter
		cEntity* pParticleMesh = findObjectByFriendlyName("PlasmaRingImposterObject");
		glm::mat4 matParticleIndentity = glm::mat4(1.0f);
		glm::vec3 oldPosition = pParticleMesh->m_EntityPhysics->position;
		glm::quat oldOrientation = pParticleMesh->m_EntityPhysics->getQOrientation();
		glm::vec3 oldScale = pParticleMesh->m_EntityPhysics->nonUniformScale;

		pParticleMesh->m_EntityPhysics->setMeshOrientationEulerAngles(0.0f, 0.0f, 0.0f);
		pParticleMesh->m_EntityMesh->bIsVisible = true;


		// Set up the shader
		glUniform1f(bIsParticleImposter_UniLoc, (float)GL_TRUE);


		pPE_Plasma_01->Update(deltaTime);

		std::vector<sParticle> vecParticlesToDraw;
		pPE_Plasma_01->getAliveParticles(vecParticlesToDraw);

		pPE_Plasma_01->sortParticlesBackToFront(vecParticlesToDraw, cameraEye);

		unsigned int numParticles = (unsigned int)vecParticlesToDraw.size();
		//			std::cout << "Drawing " << numParticles << " particles" << std::end;

		unsigned int count = 0;
		for (unsigned int index = 0; index != numParticles; index++)
		{
			if (vecParticlesToDraw[index].lifeRemaining > 0.0f)
			{
				// Draw it
				pParticleMesh->m_EntityPhysics->position = vecParticlesToDraw[index].position;
				pParticleMesh->m_EntityPhysics->setUniformScale(vecParticlesToDraw[index].scale);
				pParticleMesh->m_EntityPhysics->setQOrientation(vecParticlesToDraw[index].qOrientation);

				// This is for the "death" transparency
				glUniform1f(ParticleImposterAlphaOverride_UniLoc, vecParticlesToDraw[index].transparency);

				DrawObject(pParticleMesh, matParticleIndentity, shaderProgramID, 0, g_pFBOMain);
				count++;
			}
		}
		//			std::cout << "Drew " << count << " particles" << std::endl;
		pParticleMesh->m_EntityMesh->bIsVisible = false;
		pParticleMesh->m_EntityPhysics->position = oldPosition;
		pParticleMesh->m_EntityPhysics->setQOrientation(oldOrientation);
		pParticleMesh->m_EntityPhysics->nonUniformScale = oldScale;
		glUniform1f(bIsParticleImposter_UniLoc, (float)GL_FALSE);
		glUniform1f(ParticleImposterAlphaOverride_UniLoc, 1.0f);
		// ***************************************************************************
	}
	// ENDOF: flat 2D plasma explosion
	return;
}
