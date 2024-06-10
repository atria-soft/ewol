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
varying vec4 io_color;
varying vec2 io_texcoord;

void main(void) {
	gl_Position = in_MatrixTransformation * vec4(in_coord3d, 1.0);
	// set texture output coord
	io_texcoord = in_texture2d;
	// set output color :
	io_color = in_color;
}