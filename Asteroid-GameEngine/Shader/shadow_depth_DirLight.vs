#version 330 core
layout (location = 0) in vec3 aPos;

layout (location = 5) in mat4 aInstanceMatrix;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;
uniform bool Use_Instance;
void main()
{
	if(Use_Instance)
		 gl_Position =  lightSpaceMatrix * aInstanceMatrix * vec4(aPos, 1.0);
	else
		 gl_Position =  lightSpaceMatrix * model * vec4(aPos, 1.0);
}