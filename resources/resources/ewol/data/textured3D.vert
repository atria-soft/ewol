#version 400 core

#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// VBA Input :
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_textureCoords;
layout (location = 3) in vec4 in_colors;
uniform mat4 in_matrixTransformation;
uniform mat4 in_matrixProjection;
uniform mat4 in_matrixView;

// output :
varying vec4 io_color;
varying vec2 io_texcoord;

void main(void) {
	gl_Position = in_matrixProjection * in_matrixView * in_matrixTransformation * vec4(in_position, 1.0);
	//gl_Position = in_MatrixTransformation * vec4(in_position, 1.0);
	// set texture output coord
	io_texcoord = in_textureCoords;
	// set output color :
	io_color = in_colors;
}
