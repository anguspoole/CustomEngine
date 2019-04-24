#version 420 
// vertex01.glsl

//uniform mat4 MVP;		
uniform mat4 matModel;		// M
uniform mat4 matModelInvTrans;	// inverse(transpose(matModel))
uniform mat4 matView;		// V
uniform mat4 matProj;		// P

in vec4 vColour;		// rgba   	was "attribute"
in vec4 vPosition;		// xyzw		was "attribute"
in vec4 vNormal;		// normal xyz
in vec4 vUV_x2;			// Texture coordinates (2 of them)
in vec4 vTanXYZ;			// Tangent to the surface
in vec4 vBiNormXYZ;		// bi-normal (or bi-tangent) to the surface
//in float boneID[4];			//unsigned int boneID[4];
//in float boneWeight[4];		//float boneWeights[4];
in vec4 vBoneID;		// really passing it as 4 values
in vec4 vBoneWeight;	


out vec4 color;			// exit to fragment
out vec4 color2;			// exit to fragment
out vec4 vertPosWorld;	// "World space"
out vec4 vertNormal;	// "Model space"
out vec4 vertUV_x2;		// To the next shader stage
out vec4 vertTanXYZ;	// Tangent to the surface
out vec4 vertBiNormXYZ;	// bi-normal (or bi-tangent) to the surface

out vec4 lightPointTangent; 
out vec4 lightPointView;
out vec4 lightPointFrag;

uniform vec3 eyeLocation;		// This is in "world space"

struct sLight
{
	vec4 position;			
	vec4 diffuse;	
	vec4 specular;	// rgb = highlight colour, w = power
	vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	vec4 direction;	// Spot, directional lights
	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	                // 0 = pointlight
					// 1 = spot light
					// 2 = directional light
	vec4 param2;	// x = 0 for off, 1 for on
};

const int POINT_LIGHT_TYPE = 0;
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;

const int NUMBEROFLIGHTS = 10;
uniform sLight theLights[NUMBEROFLIGHTS];  	// 80 uniforms


// Note, we're in the VERTEX shader now, baby!
uniform sampler2D texHeightMap;
uniform sampler2D normalMap1Texture;
uniform bool bUseHeightMap;			// "turn on" the vertex displacement
//Normal map
uniform bool bHasNormalMap;
uniform float heightMapRatio;		// Increase the range of the displacement

// For skinned mesh
const int MAXNUMBEROFBONES = 100;
uniform mat4 bones[MAXNUMBEROFBONES];
// Pass the acutal number you are using to control how often this loops
uniform int numBonesUsed;			
uniform bool bIsASkinnedMesh;	// True to do the skinned mesh

void main()
{
	// Make a copy of the "model space" vertex
	vec3 posTemp = vPosition.xyz;
	
	// Apply vertex displacement?
	if ( bUseHeightMap )
	{
		// Note: I'm only sampling ONE of the colours, because it's black and white
		// Returns 0.0 to 1.0
		float height = texture( texHeightMap, vUV_x2.st ).r;
		
		height = height * heightMapRatio;
		
		posTemp.y = 0.0f;		// "Flatten" the mesh
		posTemp.y = height;		// Set the heigth
		// You could also "adjust" an existing mesh
	
	}//if ( bUseHeightMap )
	
    color = vColour;
	
	// Pass the texture coordinates out, unchanged.
	vertUV_x2 = vUV_x2;
	
	// Also pass the bi-tangent (bi-normal) and tangent to fragment
	vertTanXYZ = vTanXYZ;		// Tangent to the surface
	vertBiNormXYZ = vBiNormXYZ;		// bi-normal (or bi-tangent) to the surface

	
	// Skinned mesh or not?
	if ( bIsASkinnedMesh )
	{	
		//*****************************************************************
		// Before I do the full MVP (screen space) calculation, 
		// I'm going to adjust where the vertex is based on the 
		// the bone locations
		
		mat4 BoneTransform 
		               = bones[ int(vBoneID[0]) ] * vBoneWeight[0];
		BoneTransform += bones[ int(vBoneID[1]) ] * vBoneWeight[1];
		BoneTransform += bones[ int(vBoneID[2]) ] * vBoneWeight[2];
		BoneTransform += bones[ int(vBoneID[3]) ] * vBoneWeight[3];
		// You could also do bBoneID.x, .y, etc.
				
	//	matrixWorld = BoneTransform * matrixWorld;
		
		vec4 vertPositionFromBones = BoneTransform * vec4(posTemp.xyz, 1.0);
		//*****************************************************************
	
		mat4 MVP = matProj * matView * matModel;
		gl_Position = MVP * vertPositionFromBones;			// ON SCREEN
		
		vertPosWorld = matModel * vec4(posTemp, 1.0);
		
		
		
		// Transforms the normal into "world space"
		// Remove all scaling and transformation from model
		// Leaving ONLY rotation... 
		
		// This only leaves rotation (translation and scale are removed)
		mat4 matBoneTransform_InTrans = inverse(transpose(BoneTransform));
		
		vec4 vNormBone = matBoneTransform_InTrans * vec4(normalize(vNormal.xyz),1.0f);
		
		vertNormal = matModelInvTrans * vNormBone;
		
		
		// And then you do the same for the binormal and bitangent
		vec4 vTanXYZ_Bone = matBoneTransform_InTrans * vTanXYZ;
		vertTanXYZ = matModelInvTrans * vTanXYZ_Bone;
		
		vec4 vBiNormXYZ_Bone = matBoneTransform_InTrans * vBiNormXYZ;
		vertBiNormXYZ = matModelInvTrans * vBiNormXYZ_Bone;
		
		
		// Debug: make green if this flag is set
		color.rgb = vec3(0.0f, 1.0f, 0.0f);
	}
	else
	{
		// Is a regular mesh
		// Note these are 'backwards'
		mat4 MVP = matProj * matView * matModel;
		gl_Position = MVP * vec4(posTemp, 1.0f);			// ON SCREEN
		
		vertPosWorld = matModel * vec4(posTemp, 1.0f);
		
		// Transforms the normal into "world space"
		// Remove all scaling and transformation from model
		// Leaving ONLY rotation... 
		vertNormal = matModelInvTrans * vec4(normalize(vNormal.xyz),1.0f);
	
		for ( int index = 0; index < NUMBEROFLIGHTS; index++ )
		{	
			// ********************************************************
			// is light "on"
			if ( theLights[index].param2.x == 0.0f )
			{	// it's off
				continue;
			}


			if(bHasNormalMap)
			{
				mat3 normalMatrix = mat3(matModel);
				vec3 tan = normalize(normalMatrix * vTanXYZ.xyz);
				vec3 norm = normalize(normalMatrix * vBiNormXYZ.xyz);
				tan = normalize(tan - dot(tan, norm) * norm);
				vec3 bitan = cross(norm, tan);


				mat3 matTBN = transpose(mat3(tan, bitan, norm));    
				lightPointTangent.xyz = matTBN * theLights[index].position.xyz;
				lightPointView.xyz  = matTBN * eyeLocation.xyz;
				lightPointFrag.xyz  = matTBN * (vertPosWorld).xyz;			
			}
			else
			{
				lightPointTangent.xyz =  theLights[index].position.xyz;
				lightPointView.xyz  = eyeLocation.xyz;
				lightPointFrag.xyz  = vertPosWorld.xyz;			
			}
		}

	}//if ( bIsASkinnedMesh )
	
	color2 = color;
}



