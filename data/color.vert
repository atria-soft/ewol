#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// Input :
attribute vec2 EW_coord2d;
attribute vec4 EW_color;
uniform mat4 EW_MatrixTransformation;

// output :
varying vec4 f_color;

void main(void) {
	gl_Position = EW_MatrixTransformation * vec4(EW_coord2d, 0.0, 1.0);
	//gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(EW_coord2d, 0.0, 1.0);
	f_color = EW_color;
}
