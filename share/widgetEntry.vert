#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// Input :
attribute vec2 EW_coord2d;
uniform mat4   EW_MatrixTransformation;

// output :
varying vec2  v_position;       // This will be passed into the fragment shader.

void main(void) {
	gl_Position = EW_MatrixTransformation * vec4(EW_coord2d, 0.0, 1.0);
	// transmit position of the curent element (intermolated ...)
	v_position = EW_coord2d;
}
