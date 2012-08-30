#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif
// Input :
attribute vec3 EW_coord3d;
attribute vec2 EW_texture2d;
attribute vec4 EW_color;
uniform mat4 EW_MatrixTransformation;

// output :
varying vec4 f_color;
varying vec2 f_texcoord;

void main(void) {
	gl_Position = EW_MatrixTransformation * vec4(EW_coord3d, 1.0);
	//gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(EW_coord3d, 1.0);
	// set texture output coord
	f_texcoord = EW_texture2d;
	// set output color :
	f_color = EW_color;
}
