#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform bool HasTexture;
void main()
{
	if(HasTexture)
		FragColor = texture(texture_diffuse1, TexCoords);
	else
		FragColor = vec4(0.4,0.4,0.4,1);
}