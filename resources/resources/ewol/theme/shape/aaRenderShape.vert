#version 400 core

#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// Input:
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_textureCoords;

uniform vec3 in_offsetScale;

uniform mat4 in_matrixTransformation;
uniform mat4 in_matrixProjection;
uniform mat4 in_matrixView;

// output:
out vec2 io_textureCoords;

void main(void) {
	vec4 position = vec4(in_position.x + sign(in_position.x)*in_offsetScale.x,
	                     in_position.y + sign(in_position.y)*in_offsetScale.y,
	                     in_position.z + sign(in_position.z)*in_offsetScale.z, 1.0);
	gl_Position = in_matrixProjection * in_matrixView * in_matrixTransformation * position;
	io_textureCoords = in_textureCoords;
}
