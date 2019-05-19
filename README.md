# CustomEngine
Engine I made using C++ code and knowledge learned during the fanshawe game development course, GDP2018-19

By: Angus Poole

## About
Build/Compile in Release/x64

This project uses Bullet physics, and OpenGL 3.5. This was my final project for the course. The entire engine was coded with C++.

## Features
* Framebuffer Objects: this project uses multiple FBOs to perform bloom.
* Bloom/blur: this can be hard to notice, but a bloom/blur effect is applied. It is most noticeable on the blue paint after slaying an enemy. This is done by rendering the scene to an FBO, performing a gaussian blur using horizontal and vertical blurring a number of times, and then rendering the result to a final FBO.
* Depth Map: a depth map is used on the ground texture (noticeable when the circle of light is directly visible on it)
* Bullet Physics: bullet is used to create collision shapes, rigid bodies, etc. A custom collision callback function was created, and collision masks are used as well. All graphics and controls are based around the object's transformation matrix in the bullet physics world.
* AI: enemies will rotate and move towards the player if the player is close enough, attack if the player is closer still, and can take damage and die.
* Animations: using FBX files with characters/animations obtained from Mixamo, the player and the enemies are all animated.
* Paint Splatters: when an enemy dies, paint globs hit the ground and "splatter". This is done by spawning a small plane object at the collision position, randomly selecting one of two black and white patterns as a texture, looping over that texture and discarding the "white enough" pixels, then colouring the remaining pixels blue or yellow.