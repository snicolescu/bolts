#version 330 core

uniform mat4 u_wvp;
uniform mat4 u_test;
uniform vec3 u_color;

in vec3 a_posL;

out vec3 v_color;

void main()
{
	v_color = u_color;
	gl_Position = u_wvp * vec4(a_posL, 1.0);
}