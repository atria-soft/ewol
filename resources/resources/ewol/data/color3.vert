#version 400 core

#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// Input :
layout (location = 0) in vec3 in_position;
layout (location = 3) in vec4 in_colors;
uniform mat4 in_MatrixTransformation;
uniform mat4 in_MatrixPosition;

// output :
varying vec4 io_color;

void main(void) {
	gl_Position = in_MatrixTransformation * in_MatrixPosition * vec4(in_position, 1.0);
	gl_Position = in_MatrixTransformation * vec4(in_position, 1.0);
	//gl_Position = vec4(in_position, 1.0);
	io_color = in_colors;
}
