#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in float id;

out vec3 v_Position;
out vec3 v_Normal;
out vec2 v_TexCoord;
out float v_TexID;

uniform mat4 u_MVP;
uniform mat4 u_ModelMatrix;

void main()
{
	gl_Position = u_MVP * vec4(position.xyz, 1.f);
	v_Position = vec3(u_ModelMatrix * vec4(position, 1.f));
	v_Normal = mat3(transpose(inverse(u_ModelMatrix))) * normal;
	v_TexCoord = uv;
	v_TexID = id;
};


#shader fragment
#version 330 core

struct Material
{
	sampler2D Diffuse;
	sampler2D Specular;
};

struct Light
{
	vec3 Position;
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

layout(location =0) out vec4 u_Colour;

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoord;
in float v_TexID;

uniform vec3 u_ViewPos;
uniform Light u_Light;
uniform sampler2D u_Diffuse;
uniform sampler2D u_Specular;

void main()
{
	// ambient 
	vec3 ambient = u_Light.Ambient * vec3(texture(u_Diffuse,v_TexCoord));

	// diffuse 
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(u_Light.Position - v_Position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = u_Light.Diffuse * diff * vec3(texture(u_Diffuse, v_TexCoord));

	//specular
	vec3 viewDir = normalize(u_ViewPos - v_Position);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.f);
	vec3 specular = u_Light.Specular * spec * vec3(texture(u_Specular, v_TexCoord));

	vec3 result = ambient + diffuse + specular;

	u_Colour = vec4(result, 1.f);

}