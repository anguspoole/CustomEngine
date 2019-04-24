#include "cParticleEmitter.h"

#include <iostream>
#include <algorithm>		// For sorting the particles

sParticle::sParticle()
{
	this->position = glm::vec3(0.0f);
	this->velocity = glm::vec3(0.0f);
	this->qOrientation = glm::quat(glm::vec3(0.0f));
	this->lifeRemaining = 0.0f;
	// Used for the "death fade" at the end of life
	this->transparency = 1.0f;
//	this->lifetime = 0.0f; // Optional, might not need this
	this->particleIndex = 0;
	return;
}
	
cParticleEmitter::cParticleEmitter()
{
	this->m_bIsInit = false;

	this->m_bParticleGenerationOn = false;

	this->m_NextParticleCountdownSeconds = 0.0f;

	return;
}

cParticleEmitter::~cParticleEmitter()
{
	return;
}

	// EXPENSIVE operation, called when created or when
	//  we want to drastically change the parameters
void cParticleEmitter::Init( sParticleCreationParams &params )		// 50
{
	// Clear any existing particles...
	this->vecParticles.clear();

	this->m_NewParticleParams = params;

	// This is really slow...
	for ( unsigned int index = 0; index != this->m_NewParticleParams.totalNumberOfParticles; index++ )
	{
		sParticle newParticle;			// NEW
		newParticle.lifeRemaining = -1.0f;
		newParticle.particleIndex = index;
		newParticle.position = this->m_NewParticleParams.position;

		this->vecParticles.push_back( newParticle );
	}

	for ( unsigned int index = 0; index != this->m_NewParticleParams.totalNumberOfParticles; index++ )
	{
		this->vecParticles[index].particleIndex = index;
	}


	this->m_bIsInit = true;

	return;
}

// Uses initial number of particles as starting. 
 // Emitter is NOT running at start
void cParticleEmitter::Reset(void)
{
	unsigned int numParticles = getRandInRange( this->m_NewParticleParams.minNumberNewParticles, 
												this->m_NewParticleParams.maxNumberNewParticles );
	this->Reset(numParticles, false);

	return;
}


void cParticleEmitter::Reset( bool EnableParticleGeneration /*=true*/ )
{
	unsigned int numParticles = getRandInRange( this->m_NewParticleParams.minNumberNewParticles, 
												this->m_NewParticleParams.maxNumberNewParticles );
	this->Reset(numParticles, EnableParticleGeneration);

	return;
}


// BAM!
void cParticleEmitter::Reset( unsigned int initialNumberOfParticles, bool EnableParticleGeneration /*=true*/)
{
	if ( ! this->m_bIsInit )
	{
		return;
	}

	// Set all the particles in motion
	unsigned int totalParticles = (unsigned int)this->vecParticles.size();
	unsigned int index = 0;

	// "kill" all the particles
	for ( ; index != totalParticles; index++ )
	{
		this->vecParticles[index].lifeRemaining = -1.0f;
	}

	this->m_bParticleGenerationOn = EnableParticleGeneration;

	// This initial call takes 0 seconds
	//this->m_GenerateNewParticles(0);


	if ( initialNumberOfParticles > this->m_NewParticleParams.totalNumberOfParticles )
	{
		initialNumberOfParticles = this->m_NewParticleParams.totalNumberOfParticles;
	}

	sParticle* pCurParticle = NULL;
	for ( int count = 0; count != initialNumberOfParticles; count++ )
	{
		if ( this->m_findNextDeadParticle(pCurParticle) )
		{
			this->m_ResetParticle(pCurParticle);
		}
		else
		{
			// No more dead particles to "recycle"
			break;		// Break out of the for loop (??)
		}
	}//for ( int count = 0


	return;
}//void cParticleEmitter::Reset(void)


// This is called by Reset() and Update()
void cParticleEmitter::m_GenerateNewParticles(double deltaTime)
{
	if ( this->m_bParticleGenerationOn == false ) 
	{
		return;
	}

	// Has enough time passed between particle generation
	if ( this->m_NextParticleCountdownSeconds > 0.0f )
	{
		// No, so wait a little more
		this->m_NextParticleCountdownSeconds -= (float)deltaTime;
		return;
	}

	// Enoguh time has passes, to make the particles

	// Generate new particles...
	// Create X particles to start 
	// How many particle are we creating this time? 
	unsigned int numParticles = this->getRandInRange( this->m_NewParticleParams.minNumberNewParticles, 
													  this->m_NewParticleParams.maxNumberNewParticles );
	sParticle* pCurParticle = NULL;

	for ( int count = 0; count != numParticles; count++ )
	{
		if ( this->m_findNextDeadParticle(pCurParticle) )
		{
			this->m_ResetParticle(pCurParticle);
		}
		else
		{
			// No more dead particles to "recycle"
			break;		// Break out of the for loop (??)
		}
	}//for ( int count = 0

	// Pick a new "time to wait between particles" 
	this->m_NextParticleCountdownSeconds = this->getRandInRange( this->m_NewParticleParams.minTimeBetweenParticleGenerationSeconds, 
																 this->m_NewParticleParams.maxTimeBetweenParticleGenerationSeconds );

	return;
}


void cParticleEmitter::m_ResetParticle(sParticle* pParticle)
{
	if ( ! this->m_bIsInit )
	{
		return;
	}

	// Pick a random lifetime
	pParticle->lifeRemaining = this->getRandInRange( this->m_NewParticleParams.minLifeTime, 
													 this->m_NewParticleParams.maxLifeTime );

	// Pick a random velocity
	pParticle->velocity = this->getRandInRange( this->m_NewParticleParams.minInitialVelocity, 
												this->m_NewParticleParams.maxInitialVelocity );

	pParticle->qOrientation = glm::quat( 
		/* Pick Euler angle, which is converted to quaternion in constructor */
		glm::vec3( this->getRandInRange( this->m_NewParticleParams.minIntitialOrientationAngleEuler.x, 
		                                 this->m_NewParticleParams.maxIntitialOrientationAngleEuler.x ), 
                   this->getRandInRange( this->m_NewParticleParams.minIntitialOrientationAngleEuler.y, 
		                                 this->m_NewParticleParams.maxIntitialOrientationAngleEuler.y ), 
                   this->getRandInRange( this->m_NewParticleParams.minIntitialOrientationAngleEuler.z, 
		                                 this->m_NewParticleParams.maxIntitialOrientationAngleEuler.z ) ) );

	// Rotation change over time
	pParticle->qOrientationChangePerFrame = glm::quat( 
		/* Pick Euler angle, which is converted to quaternion in constructor */
		glm::vec3( this->getRandInRange( this->m_NewParticleParams.minOrientationChangeAngleEuler.x, 
		                                 this->m_NewParticleParams.maxOrientationChangeAngleEuler.x ), 
                   this->getRandInRange( this->m_NewParticleParams.minOrientationChangeAngleEuler.y, 
		                                 this->m_NewParticleParams.maxOrientationChangeAngleEuler.y ), 
                   this->getRandInRange( this->m_NewParticleParams.minOrientationChangeAngleEuler.z, 
		                                 this->m_NewParticleParams.maxOrientationChangeAngleEuler.z ) ) );


	pParticle->scale = this->getRandInRange( this->m_NewParticleParams.minInitialScale, 
											 this->m_NewParticleParams.maxInitialScale );

	pParticle->position = this->m_NewParticleParams.position;

	pParticle->transparency = 1.0f;

	return;
}


void cParticleEmitter::Update(double deltaTime)
{
	if ( ! this->m_bIsInit )
	{
		return;
	}

	// Update each "living" particle using Euler (or whatever)

	float fDeltaTime = (float)deltaTime;

	unsigned int totalParticles = (unsigned int)this->vecParticles.size();
	unsigned int index = 0;

	// Reference to particle (to make reading easier)
	//sParticle& curParticle = this->vecParticles[index];
	// Sort of like this, but a little safer (for stupid human programmers
	sParticle* pCurParticle = &( this->vecParticles[index] );


	for ( ; index != totalParticles; index++ )
	{
		//curParticle = this->vecParticles[index];
		pCurParticle = &( this->vecParticles[index] );

		// Is the particle alive? 
//		if ( curParticle.lifeRemaining > 0.0f )
		if ( pCurParticle->lifeRemaining > 0.0f )
		{
			// Is alive, so update
			// Velocity from accel
			pCurParticle->velocity += (fDeltaTime * this->m_NewParticleParams.acceleration);

			pCurParticle->position += (fDeltaTime * pCurParticle->velocity);

			// Update scale
			float scaleChange = getRandInRange( this->m_NewParticleParams.minScaleChange, 
												this->m_NewParticleParams.maxScaleChange );
			pCurParticle->scale += scaleChange;

			// Update rotation
			// This is a SLERP, so we pick a value between the min angle (no change) 
			//  to the maximum angle (qOrientationChangePerFrame), spherically interpolated 
			//  by how much time has passed THIS FRAME (fDeltaTime)
			glm::quat qAngleAdjustThisFrame = glm::slerp( glm::quat(glm::vec3(0.0f,0.0f,0.0f)),	// Angle of 0 degrees on all axes
														  pCurParticle->qOrientationChangePerFrame, 
														  fDeltaTime );
			
			// We "add" this tiny oriention change to the quaternion, which is really 
			//  multiplying (just like with matrix values)
			pCurParticle->qOrientation = pCurParticle->qOrientation * qAngleAdjustThisFrame;

			// Are we near the end of life (and are to fade)?
			if ( pCurParticle->lifeRemaining < this->m_NewParticleParams.deathTransparencyFadeTimeSeconds )
			{
				// Calculate how much transparency is left
				// (note: At this point, the lifeRemaining will ALWAYS be less than the deathTransparencyFadeTimeSeconds)

				pCurParticle->transparency = 1.0f - ( (this->m_NewParticleParams.deathTransparencyFadeTimeSeconds - pCurParticle->lifeRemaining) / this->m_NewParticleParams.deathTransparencyFadeTimeSeconds);
				// Clamp to 0.0f (if life is -ve)
				pCurParticle->transparency = fmax( pCurParticle->transparency, 0.0f );
			}

			// Take some life away...
			pCurParticle->lifeRemaining -= fDeltaTime;
		}
	}

	this->m_GenerateNewParticles(deltaTime);


	return;
}

// Inspired by: https://stackoverflow.com/questions/686353/c-random-float-number-generation
float cParticleEmitter::getRandInRange( float min, float max )
{
	float value = 
	min + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX/(static_cast<float>(max-min) )));
	return value;
}

unsigned int cParticleEmitter::getRandInRange( unsigned int min, unsigned int max )
{
	// You could do this with the modulus operator (since it's an int)
	float value = 
	min + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX/(static_cast<float>(max-min) )));
	return (unsigned int)value;
}


glm::vec3 cParticleEmitter::getRandInRange( glm::vec3 min, glm::vec3 max )
{
	glm::vec3 returnVal;

	returnVal.x = this->getRandInRange( min.x, max.x );
	returnVal.y = this->getRandInRange( min.y, max.y );
	returnVal.z = this->getRandInRange( min.z, max.z );

	return returnVal;
}

bool cParticleEmitter::m_findNextDeadParticle(sParticle* &pFoundParticle)
{
	unsigned int totalParticles = (unsigned int)this->vecParticles.size();
	unsigned int index = 0;

	for ( ; index != totalParticles; index++ )
	{
		if ( this->vecParticles[index].lifeRemaining <= 0.0f )
		{
			// It's dead, Jim...
			pFoundParticle = &(this->vecParticles[index]);

			return true;
		}
	}

	// DIDN'T find any...
	return false;
}


void cParticleEmitter::getAliveParticles( std::vector<sParticle> &vecParticles )
{		
	vecParticles.clear();
	vecParticles.reserve(this->vecParticles.size());

	unsigned int numParticles = (unsigned int)this->vecParticles.size();
	unsigned int count = 0;
	for ( unsigned int index = 0; index != numParticles; index++ )
	{
		if ( this->vecParticles[index].lifeRemaining > 0.0f )
		{
			vecParticles.push_back( this->vecParticles[index] );
		}
	}

	return;
}

	// Don't do something like this...
//	std::vector<sParticle> getAliveParticles( void );	// Slow (passes copied vector on stack every frame)
	// ...or this
//	std::vector<sParticle> &getAliveParticles( void );	// Can get reference to missing stack based object

void cParticleEmitter::ParticleGenerationOn(void)
{
	this->m_bParticleGenerationOn = true;
	return;
}

void cParticleEmitter::ParticleGenerationOff(void)
{
	this->m_bParticleGenerationOn = false;
	return;
}

void cParticleEmitter::UpdateParamsPosition(glm::vec3 position)
{
	this->m_NewParticleParams.position = position;
}

// Predicate function for the sort...
class cPredParticleIsCloser
{
public:
	cPredParticleIsCloser( glm::vec3 cameraEye_ )
	{
		this->cameraEye = cameraEye_;
		return;
	}
	glm::vec3 cameraEye;
	// The predicate method (note strange name)
	// Think of it as "is P1 less than P2" or something like that;
	//  returns true or false depending on comparison.
	bool operator() ( const sParticle& P1, const sParticle &P2 )
	{
		float P1_dist_to_camera = glm::distance( P1.position, this->cameraEye );
		float P2_dist_to_camera = glm::distance( P2.position, this->cameraEye );

		if ( P1_dist_to_camera < P2_dist_to_camera )
		{
			// P1 is closer
			return true;
		}
		// P2 is closer
		return false;
	}
};

// Passes in a vector of particles and sorts them "back to front" 
// (far to close) from the camera. Simply sorts by distance from camera.
void cParticleEmitter::sortParticlesBackToFront( std::vector<sParticle> &vecParticles, glm::vec3 cameraEye )
{
	// For the algorithms in the stl, you can pass either a 
	//  "predicate function" (comparison function), or a
	//  "functor" (class with the comparison function named "operator()")
	// A function doesn't have any internal state, like the position of the camera, 
	//  so I'm using a functor here so I can save the camera location

	// Create the "functor" so that it "remembers" the location of the camera
	cPredParticleIsCloser functionCamera(cameraEye);

	// Pass this object into the sort code
	std::sort( vecParticles.begin(), vecParticles.end(), functionCamera );

	// You'd likely see it written like this (even though it's identical to the above)
	//std::sort( vecParticles.begin(), vecParticles.end(), cPredParticleIsCloser(cameraEye) );

	return;
}
