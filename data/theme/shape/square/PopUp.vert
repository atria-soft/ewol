#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// Input :
attribute vec2 EW_coord2d;
attribute vec2 EW_widgetPropertyPos;
uniform mat4   EW_MatrixTransformation;

// output :
varying vec2  v_propPos;

void main(void) {
	gl_Position = EW_MatrixTransformation * vec4(EW_coord2d, 0.0, 1.0);
	v_propPos = EW_widgetPropertyPos;
}
