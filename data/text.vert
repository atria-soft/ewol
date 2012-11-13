#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// Input :
attribute vec2 EW_coord2d;
attribute vec2 EW_texture2d;
attribute vec4 EW_color;
uniform mat4 EW_MatrixTransformation;

// output :
varying vec4  f_color;
varying vec2  f_texcoord;
/*
void main(void) {
	gl_Position = EW_MatrixTransformation * vec4(EW_coord2d, 0.0, 1.0);
	//gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(EW_coord2d, 0.0, 1.0);
	// set texture output coord
	f_texcoord = EW_texture2d;
	// set output color :
	f_color = EW_color;
}
*/
varying vec4  f_patern;
void main(void) {
	gl_Position = EW_MatrixTransformation * vec4(EW_coord2d, 0.0, 1.0);
	//gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(EW_coord2d, 0.0, 1.0);
	// set output color :
	f_color = EW_color;
	if (EW_texture2d.x<1.0) {
		// normal font :
		f_patern = vec4 (0.0, 0.0, 0.0, 1.0);
	} else if (EW_texture2d.x<2.0) {
		// Italic font :
		f_patern = vec4 (1.0, 0.0, 0.0, 0.0);
	} else if (EW_texture2d.x<3.0) {
		// Bold font :
		f_patern = vec4 (0.0, 1.0, 0.0, 0.0);
	} else {
		// bold italic font :
		f_patern = vec4 (0.0, 0.0, 1.0, 0.0);
	}
	// set texture output coord
	f_texcoord = mod(EW_texture2d, 1.0);
}

