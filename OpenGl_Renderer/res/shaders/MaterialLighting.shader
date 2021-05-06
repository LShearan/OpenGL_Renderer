#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 texCoords;

out vec3 v_Position;
out vec3 v_Normal;
out vec2 v_TexCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * position;
	v_Position = vec3(u_Model * position);
	v_Normal = mat3(transpose(inverse(u_View * u_Model))) * normal.xyz;
	v_TexCoords = texCoords;
};

#shader fragment
#version 330 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

layout(location = 0) out vec4 colour;

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoords;

uniform Material u_Material;
uniform Light u_Light;

void main()
{
	// Ambient
	vec3 ambient = u_Light.ambient * vec3(texture(u_Material.diffuse, v_TexCoords));

	// Diffuse
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(u_Light.position - v_Position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = u_Light.diffuse * diff * vec3(texture(u_Material.diffuse, v_TexCoords));

	// specular
	vec3 viewDir = normalize(u_Light.position - v_Position);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
	vec3 specular = u_Light.specular * spec * vec3(texture(u_Material.specular, v_TexCoords));


	vec3 result = ambient + diffuse + specular;
	colour = vec4(result, 1.0);
};