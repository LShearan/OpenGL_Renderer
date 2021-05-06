#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;

out vec3 v_Position;
out vec3 v_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * position;
	v_Position = vec3(u_Model * position);
	v_Normal = mat3(transpose(inverse(u_View * u_Model))) * normal.xyz;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec3 v_Position;
in vec3 v_Normal;

uniform vec3 u_ObjectColour;
uniform vec3 u_LightColour;
uniform vec3 u_LightPosition;

void main()
{
	// Ambien
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * u_LightColour;

	// Diffuse
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(u_LightPosition - v_Position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * u_LightColour;

	// specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(-v_Position); // the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * u_LightColour;


	vec3 result = (ambient + diffuse + specular) * u_ObjectColour;
	colour = vec4(result, 1.0);
};