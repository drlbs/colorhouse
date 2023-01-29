#version 430

in vec4 color;
out vec4 ex_color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 lookAt_matrix;

void main(void)
{	ex_color = color;
}
