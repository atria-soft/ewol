#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// Input :
attribute vec3 EW_coord3d;
attribute vec4 EW_color;
uniform mat4 EW_MatrixTransformation;
uniform mat4 EW_MatrixPosition;

// output :
varying vec4 f_color;

void main(void) {
	gl_Position = EW_MatrixTransformation * EW_MatrixPosition * vec4(EW_coord3d, 1.0);
	f_color = EW_color;
}
