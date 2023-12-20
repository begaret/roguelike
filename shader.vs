#version 330 core

layout(location = 0) in vec2 a_xy;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec3 a_fg;
layout(location = 3) in vec3 a_bg;

uniform mat4 u_p;

out vec2 v_uv;
out vec3 v_fg;
out vec3 v_bg;

void main()
{
	gl_Position = u_p * vec4(a_xy, 0.0, 1.0);
	v_uv = a_uv;
	v_fg = a_fg;
	v_bg = a_bg;
}

