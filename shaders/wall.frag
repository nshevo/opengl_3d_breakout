#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;
uniform sampler2D diffuseMap;

void main()
{

	vec3 lightColor = vec3(1.0f);
	vec3 lightPos = vec3(0.0f, 0.0f, 1.0f);
	vec3 viewPos = vec3(0.0f, 0.0f, 3.0f);

	// Ambient
	vec3 ambient = 0.5 * vec3(texture(diffuseMap, TexCoords));

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = lightColor * diff * vec3(texture(diffuseMap, TexCoords));

	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 180);
	vec3 specular = 0.05 * spec * lightColor;
	color = vec4(ambient + diffuse + specular, 1.0f);
}