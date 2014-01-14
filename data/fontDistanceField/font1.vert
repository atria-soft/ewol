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
	// set texture output coord
	f_texcoord = EW_texture2d;
	// set output color :
	f_color = EW_color;
}


/*
// Input :
attribute vec3 EW_coord3d;
attribute vec2 EW_texture2d;
attribute vec4 EW_color;
uniform mat4 EW_MatrixTransformation;
uniform int EW_texHeight;
uniform int EW_texWidth;

// output :
varying vec4 f_color;
varying vec2 f_texcoord;
varying float f_onestepu;
varying float f_onestepv;

void main(void) {
	gl_Position = EW_MatrixTransformation * vec4(EW_coord3d, 1.0);
	// set texture output coord
	f_texcoord = EW_texture2d;
	f_onestepu = 1.0 / float(EW_texWidth);
	f_onestepv = 1.0 / float(EW_texHeight);
	// set output color :
	f_color = EW_color;
}

*/
