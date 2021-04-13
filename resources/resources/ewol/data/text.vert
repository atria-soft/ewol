#version 400 core

#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// VBA Input :
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_textureCoords;
layout (location = 3) in vec4 in_colors;
uniform mat4 in_MatrixTransformation;

// output :
varying vec4  io_color;
varying vec2  io_texcoord;
/*
void main(void) {
	gl_Position = in_MatrixTransformation * vec4(in_coord2d, 0.0, 1.0);
	//gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(in_coord2d, 0.0, 1.0);
	// set texture output coordinates
	io_texcoord = in_textureCoords;
	// set output color :
	io_color = in_colors;
}
*/
varying vec4  io_patern;
void main(void) {
	gl_Position = in_MatrixTransformation * vec4(in_position, 1.0);
	//gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(in_coord2d, 0.0, 1.0);
	// set output color :
	io_color = in_colors;
	if (in_textureCoords.x<1.0) {
		// normal font :
		io_patern = vec4 (0.0, 0.0, 0.0, 1.0);
	} else if (in_textureCoords.x<2.0) {
		// Italic font :
		io_patern = vec4 (1.0, 0.0, 0.0, 0.0);
	} else if (in_textureCoords.x<3.0) {
		// Bold font :
		io_patern = vec4 (0.0, 1.0, 0.0, 0.0);
	} else {
		// bold italic font :
		io_patern = vec4 (0.0, 0.0, 1.0, 0.0);
	}
	// set texture output coord
	io_texcoord = mod(in_textureCoords, 1.0);
}

