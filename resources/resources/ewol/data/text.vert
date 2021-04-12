#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// Input :
attribute vec3 in_coord3d;
attribute vec2 in_texture2d;
attribute vec4 in_color;
uniform mat4 in_MatrixTransformation;

// output :
varying vec4  io_color;
varying vec2  io_texcoord;
/*
void main(void) {
	gl_Position = in_MatrixTransformation * vec4(in_coord2d, 0.0, 1.0);
	//gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(in_coord2d, 0.0, 1.0);
	// set texture output coord
	io_texcoord = in_texture2d;
	// set output color :
	io_color = in_color;
}
*/
varying vec4  io_patern;
void main(void) {
	gl_Position = in_MatrixTransformation * vec4(in_coord3d, 1.0);
	//gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(in_coord2d, 0.0, 1.0);
	// set output color :
	io_color = in_color;
	if (in_texture2d.x<1.0) {
		// normal font :
		io_patern = vec4 (0.0, 0.0, 0.0, 1.0);
	} else if (in_texture2d.x<2.0) {
		// Italic font :
		io_patern = vec4 (1.0, 0.0, 0.0, 0.0);
	} else if (in_texture2d.x<3.0) {
		// Bold font :
		io_patern = vec4 (0.0, 1.0, 0.0, 0.0);
	} else {
		// bold italic font :
		io_patern = vec4 (0.0, 0.0, 1.0, 0.0);
	}
	// set texture output coord
	io_texcoord = mod(in_texture2d, 1.0);
}

