#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif
// Input :
attribute vec3 EW_coord3d;
attribute vec2 EW_texture2d;
attribute vec3 EW_normal;
uniform mat4 EW_MatrixTransformation;
uniform mat4 EW_MatrixPosition;

// output :
varying vec2 f_texcoord;

void main(void) {
	gl_Position = EW_MatrixTransformation * EW_MatrixPosition * vec4(EW_coord3d, 1.0);
	// set texture output coord
	f_texcoord = EW_texture2d;
}
