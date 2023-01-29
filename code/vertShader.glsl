#version 430

// this will receive the positions that were defined in the vbo and
// set up into 3 component vectors in the vao. The "in" in the declaration
// stands for "input" to the verted shader.  The layout (location=0)
// can be used to select a particular buffer.
layout (location=0) in vec4 position;
layout (location=1) in vec4 in_color;

out vec4 color;

// It is more efficient to do the matrix work of setting up the display in the shader
// on the GPU than in the application program.  OpenGL lets us set these variables
// up as "uniform" variables in the shader.  The following two variable hold
// the contents of the 4x4 modelview and projection matrices that were defined in the 
// display function of the application. 
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 lookAt_matrix;

void main(void)
{
// here we transform our point to the correct position and projection using
// matrix multiplication using operations tied to the vec4 data type 
	gl_Position = proj_matrix * lookAt_matrix * mv_matrix * position;
    color = in_color;
} 
