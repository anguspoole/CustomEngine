#ifndef _cAnimationState_HG_
#define _cAnimationState_HG_

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class cAnimationState
{
public:
	cAnimationState() {};

	struct sStateDetails 
	{
		sStateDetails() : 
			currentTime(0.0f), 
			totalTime(0.0f),
			frameStepTime(0.0f){};
		std::string name;
		float currentTime;		// Time (frame) in current animation
		float totalTime;		// Total time animation goes
		float frameStepTime;	// Number of seconds to 'move' the animation	
		// Returns true if time had to be reset
		// (for checking to see if the animation has finished or not)
		// TODO: Deal with running the animation backwards, perhaps?? 
		bool IncrementTime(bool bResetToZero = true);
	};


	// Extent Values for skinned mesh
	// These can be updated per frame, from the "update skinned mesh" call
	glm::vec3 minXYZ_from_SM_Bones;
	glm::vec3 maxXYZ_from_SM_Bones;
	// Store all the bones for this model, buing updated per frame
	std::vector< glm::mat4x4 > vecObjectBoneTransformation;

	
	std::vector<sStateDetails> vecAnimationQueue;
	sStateDetails defaultAnimation;
};

#endif 

