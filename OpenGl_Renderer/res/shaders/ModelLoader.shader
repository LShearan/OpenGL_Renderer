#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out vec3 v_Position;
out vec3 v_Normal;
out vec2 v_TexCoord;

uniform mat4 u_MVP;
uniform mat4 u_ModelMatrix;

void main()
{
	gl_Position = u_MVP * vec4(position.xyz,1.f);
	v_Position = vec3(u_ModelMatrix * vec4(position, 1.f));
	v_Normal = mat3(transpose(inverse(u_ModelMatrix))) * normal;
	v_TexCoord = uv;
};

#shader fragment
#version 330 core



struct Light
{
	vec3 Position;
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

layout(location = 0) out vec4 colour;

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoord;

uniform vec3 u_ViewPos;
uniform Material u_Material;
uniform Light u_Light;

void main()
{
	// ambient 
	vec3 ambient = u_Light.Ambient * u_Material.Ambient;

	// diffuse 
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(u_Light.Position - v_Position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = u_Light.Diffuse * (diff * u_Material.Diffuse);

	//specular
	vec3 viewDir = normalize(u_ViewPos - v_Position);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Shininess);
	vec3 specular = u_Light.Specular * (spec * u_Material.Specular);
		
	vec3 result = ambient + diffuse + specular;

	colour = vec4(result, 1.f);
};