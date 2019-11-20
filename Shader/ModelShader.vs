#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aTangent;
layout (location = 4) in vec2 aBitangent;

layout (location = 5) in mat4 aInstanceMatrix;

layout (location = 9) in ivec4 boneID;
layout (location = 10) in vec4 Boneweight;
//out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
	vec4 FragPosLightSpace;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;
uniform bool Use_Instance;
uniform bool Has_Bone;

uniform mat4 boneTransform[16];

void main()
{
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    vs_out.TexCoords = aTexCoords;
	vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);


   	if(Use_Instance)
	{
		gl_Position = projection * view * aInstanceMatrix * vec4(aPos, 1.0f); 
	}	
	else
	{
		if(Has_Bone)
		{
				mat4 BoneTransformation = boneTransform[boneID[0]]*Boneweight[0];
				BoneTransformation += boneTransform[boneID[1]]*Boneweight[1];
				BoneTransformation += boneTransform[boneID[2]]*Boneweight[2];
				BoneTransformation += boneTransform[boneID[3]]*Boneweight[3];

			gl_Position = projection * view * model * (BoneTransformation* vec4(aPos, 1.0f)); 
		}
		else
		{
			gl_Position = projection * view * model * vec4(aPos, 1.0f); 
		}
		
	}


		

}