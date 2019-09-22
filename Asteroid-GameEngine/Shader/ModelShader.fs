#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define DLight_Length 3
#define PLight_Length 3
#define SLight_Length 3

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

//in vec2 TexCoords;
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

uniform DirLight dirLight[DLight_Length];
uniform PointLight pointLights[PLight_Length];

uniform SpotLight spotLight;
uniform Material material;
uniform vec3 Color;

//Shadow texture
//uniform sampler2D diffuseTexture;
//uniform samplerCube depthMap;
//uniform vec3 lightPos;
uniform vec3 viewPos;
//uniform float far_plane;
//uniform bool shadows;
//Shadow texture

uniform sampler2D texture_diffuse1;
//uniform bool HasTexture;

void main()
{
	//vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize( fs_in.Normal);
    vec3 viewDir = normalize(viewPos -  fs_in.FragPos);

	// phase 1: directional lighting
	vec3 result;
	for(int i=0; i< DLight_Length;i++)
	{
	   result += CalcDirLight(dirLight[i], normal, viewDir);
	}
    // phase 2: point lights
    for(int i = 0; i < PLight_Length ; i++)
	{
	   //result+=vec3(0,0,0);
	   result += CalcPointLight(pointLights[i], normal,  fs_in.FragPos, viewDir);    
	} 
    // phase 3: spot light
    result += CalcSpotLight(spotLight, normal,  fs_in.FragPos, viewDir);    

	//vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color; 

	//if(HasTexture)
		//FragColor = texture(texture_diffuse1, fs_in.TexCoords);
	//else
		FragColor = vec4(result,1);
}







// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse,  fs_in.TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,  fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular,  fs_in.TexCoords));
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);

	float attenuation = 0;
	if(light.constant>0)
		attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   

    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
	float attenuation=0;
	if(light.constant>0)
		attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   

    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}