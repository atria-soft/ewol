#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif
// Input :
attribute vec3 in_coord3d;
attribute vec2 in_texture2d;
attribute vec3 in_normal;
uniform mat4 in_MatrixTransformation;
uniform mat4 in_MatrixPosition;

// output :
varying vec2 io_texcoord;
varying vec3 v_ecNormal;

void main(void) {
	gl_Position = in_MatrixTransformation * in_MatrixPosition * vec4(in_coord3d, 1.0);
	// set texture output coord
	io_texcoord = in_texture2d;
	mat4 MatrixPosition = in_MatrixPosition;
	MatrixPosition[3][0] = 0.0;
	MatrixPosition[3][1] = 0.0;
	MatrixPosition[3][2] = 0.0;
	v_ecNormal = vec3(MatrixPosition * vec4(in_normal, 1.0) );
}
