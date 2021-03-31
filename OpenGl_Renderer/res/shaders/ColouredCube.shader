#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 colour;

out vec4 v_Colour;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_Colour = colour;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec4 v_Colour;

uniform vec4 u_Colour;

void main()
{
	colour = v_Colour;
};