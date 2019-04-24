#ifndef _cParticleEmitter_HG_
#define _cParticleEmitter_HG_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#define GLM_ENABLE_EXPERIMENTAL		// To get glm quaternion stuff to compile
#include <glm/gtx/quaternion.hpp>	// Note strange folder
#include <vector>

struct sParticle
{
	sParticle();
	glm::vec3 position;
	glm::vec3 velocity;
	float scale;
	// Could also add orientation, too
	glm::quat qOrientation;
	glm::quat qOrientationChangePerFrame;
//	glm::vec3 EulerOrientation;
	//float lifetime;			// Second this will live
	float lifeRemaining;	// How long it's been alive
	// 
	float transparency;

	unsigned int particleIndex;
};

class cParticleEmitter
{
public:
	cParticleEmitter();
	~cParticleEmitter();

	// Added this so:
	// - the defaults are set
	// - there isn't a ton of parameters passed on Init()
	struct sParticleCreationParams
	{
		sParticleCreationParams() : 
			minLifeTime(0.0f), maxLifeTime(1.0f), 
			totalNumberOfParticles(100),
			position(glm::vec3(0.0f,0.0f,0.0f)),
			minInitialVelocity(glm::vec3(-1.0f,-1.0f,-1.0f)),
			maxInitialVelocity(glm::vec3(1.0f,1.0f,1.0f)),
			acceleration( glm::vec3(0.0f,0.0f,0.0f) ),

			minIntitialOrientationAngleEuler( glm::vec3(0.0f,0.0f,0.0f) ),
			maxIntitialOrientationAngleEuler( glm::vec3(0.0f,0.0f,0.0f) ),
			minOrientationChangeAngleEuler( glm::vec3(0.0f,0.0f,0.0f) ),
			maxOrientationChangeAngleEuler( glm::vec3(0.0f,0.0f,0.0f) ),

			minInitialScale(0.1f), maxInitialScale(1.0f),
			minScaleChange(0.0f), maxScaleChange(0.0f),

			minNumberNewParticles(0),  maxNumberNewParticles(1),
			minTimeBetweenParticleGenerationSeconds(0.0f), 
			maxTimeBetweenParticleGenerationSeconds(0.1f), 

			deathTransparencyFadeTimeSeconds(0.0f)
		{};		
		//
		float minLifeTime;
		float maxLifeTime; 
		unsigned int totalNumberOfParticles;		// 500
		glm::vec3 position;
		// 3 units per second   (-0.1, 0.5f, -0.1f) 
		//					   (+0.1, 1.0f, +0.1f)
		glm::vec3 minInitialVelocity;
		glm::vec3 maxInitialVelocity; 
		// Gravity, -ve gravity, thrust (0.0f, +0.1f, 0.0f)
		glm::vec3 acceleration;  

		glm::vec3 minIntitialOrientationAngleEuler;
		glm::vec3 maxIntitialOrientationAngleEuler;
		// Set these to 0,0,0 to have no rotation over time
		glm::vec3 minOrientationChangeAngleEuler;
		glm::vec3 maxOrientationChangeAngleEuler;

		float minInitialScale;
		float maxInitialScale;
		float minScaleChange;		// Make both 0.0 to not change scale
		float maxScaleChange;

		// There's a number of other things you could do here, too, like:
		// - Colour changes over time
		// - A set of textures that you could "blend" betwen (like with fire, going from blue to red (flame) then to black to grey (smoke)
		// 
		// Here, we are going to just fade (adjust transparency) over time, at the end of the particle's life
		float deathTransparencyFadeTimeSeconds;


		unsigned int minNumberNewParticles;		
		unsigned int maxNumberNewParticles;		
		// Number of seconds until the next particle is created
		float minTimeBetweenParticleGenerationSeconds;
		float maxTimeBetweenParticleGenerationSeconds;

	};

	// EXPENSIVE operation, called when created or when
	//  we want to drastically change the parameters
	void Init( sParticleCreationParams &params );

	// This will reset the entire generator to the initial state.
	// You can call this for an explosion by passing a large number. 
	void Reset(void);		// Uses initial number of particles as starting. 
	                        // Emitter is NOT running at start
	void Reset( bool EnableParticleGeneration = true );		// BAM!
	void Reset( unsigned int initialNumberOfParticles, bool EnableParticleGeneration = true);		// BAM!

	// Called every frame
	void Update(double deltaTime);
	
	// Note this is pass by reference, "returning" the particles that
	//	are "alive"
	void getAliveParticles( std::vector<sParticle> &vecParticles );

	// Passes in a vector of particles and sorts them "back to front" 
	// (far to close) from the camera. Simply sorts by distance from camera.
	void sortParticlesBackToFront( std::vector<sParticle> &vecParticles, glm::vec3 cameraEye );
	// Don't do something like this...
//	std::vector<sParticle> getAliveParticles( void );	// Slow (passes copied vector on stack every frame)
	// ...or this
//	std::vector<sParticle> &getAliveParticles( void );	// Can get reference to missing stack based object

	void ParticleGenerationOn(void);
	void ParticleGenerationOff(void);

	void UpdateParamsPosition(glm::vec3 position);


private:
	// Now particles are private
	std::vector<sParticle> vecParticles;

	bool m_bParticleGenerationOn;

	// This is called by Reset() and Update()
	void m_GenerateNewParticles(double deltaTime);

	// Resets a single particle to the "starting" state
	void m_ResetParticle(sParticle* pParticle);

	// Is checked by Update
	bool m_bIsInit;	// = false;

	// I'm putting this here so I don't have to include "globalStuff.h"
	// Inspired by: https://stackoverflow.com/questions/686353/c-random-float-number-generation
	float getRandInRange( float min, float max );
	unsigned int getRandInRange( unsigned int min, unsigned int max );
	glm::vec3 getRandInRange( glm::vec3 min, glm::vec3 max );


	sParticleCreationParams m_NewParticleParams;

	float m_NextParticleCountdownSeconds;

	//glm::vec3 m_position;

	//glm::vec3 m_minVelocity;
	//glm::vec3 m_maxVelocity;

	//glm::vec3 m_minOrientationAngleEuler;
	//glm::vec3 m_maxOrientationAngleEuler;

	//glm::vec3 m_acceleration;		// Gravity?
	//unsigned int m_minInitNumParticles;
	//unsigned int m_maxInitNumParticles;

	//float m_minLifeTime;
	//float m_maxLifeTime;
	//unsigned int m_numberOfParticles;


	bool m_findNextDeadParticle(sParticle* &pFoundParticle); 

};

#endif