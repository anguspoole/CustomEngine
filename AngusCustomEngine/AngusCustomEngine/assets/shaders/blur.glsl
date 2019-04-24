#version 420 
//blur.glsl

//in vec2 TexCoords;
in vec4 vertUV_x2;

//uniform sampler2D image;
uniform sampler2D texPass1OutputTexture;
//uniform sampler2D texture00;
  
uniform bool horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

out vec4 FragColor;

void main()
{             
	vec2 tex_offset = 1.0 / textureSize(texPass1OutputTexture, 0); // gets size of single texel
	vec3 result = texture( texPass1OutputTexture, vertUV_x2.st).rgb * weight[0]; // current fragment's contribution
	if(horizontal)
	{
		for(int i = 1; i < 5; ++i)
		{
			result += texture(texPass1OutputTexture, vertUV_x2.st + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
			result += texture(texPass1OutputTexture, vertUV_x2.st - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
		}
	}
	else
	{
		for(int i = 1; i < 5; ++i)
		{
			result += texture(texPass1OutputTexture, vertUV_x2.st + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
			result += texture(texPass1OutputTexture, vertUV_x2.st - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
		}
	}
	FragColor = vec4(result, 1.0);
}

