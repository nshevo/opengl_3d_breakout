#version 330 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;	

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

uniform vec4 inputColor;

void main()
{
	vec4 lightColor = vec4(1.0f);
	vec3 lightPos = vec3(0.0f, 0.0f, 1.0f);
	vec3 viewPos = vec3(0.0f, 0.0f, 3.0f);

	// Ambient
	float ambientStrength = 0.1f;
	vec4 ambient = ambientStrength * lightColor;

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = diff * lightColor;

	// Specular
	float specularStrength = 1.0f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec4 specular = specularStrength * spec * lightColor;

	color = mix(diffuse * inputColor, diffuse * texture(diffuseMap, TexCoords), 0.3);

	if(texture(specularMap, TexCoords).x != 0.0){
		color = diffuse * 0.5 * (inputColor)
		+ 0.5 * texture(specularMap, TexCoords)
		+ specular * texture(specularMap, TexCoords);
		color.g=1.0f;
	}

}
