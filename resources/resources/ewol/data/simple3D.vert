#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// Input :
attribute vec3 in_coord3d;
uniform vec4 in_color;
uniform mat4 in_MatrixTransformation;

// output :
varying vec4 io_color;

void main(void) {
	gl_Position = in_MatrixTransformation * vec4(in_coord3d, 1.0);
	//gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(in_coord2d, 0.0, 1.0);
	io_color = in_color;
}
