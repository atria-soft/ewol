#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif
// Input:
attribute vec3 in_coord3d;
attribute vec2 in_texture2d;
uniform mat4 in_MatrixTransformation;
uniform mat4 in_MatrixPosition;

// output:
varying vec2 io_texcoord;

void main(void) {
	// set texture output coord
	io_texcoord = in_texture2d;
	gl_Position = in_MatrixTransformation * in_MatrixPosition * vec4(in_coord3d, 1.0);
}
