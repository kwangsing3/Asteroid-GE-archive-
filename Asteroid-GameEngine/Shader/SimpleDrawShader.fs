#version 330 core

out vec4 FragColor;

in vec3 TexColor;

// texture samplers
//uniform sampler2D texture1;
uniform vec3 Color;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = vec4(0.4,0.4,0.4,1); //*lightColor;

}