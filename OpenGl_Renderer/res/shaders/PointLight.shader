#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;


void main()
{
	colour = vec4(1.0);
};