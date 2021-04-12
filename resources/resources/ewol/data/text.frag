#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// Input :
uniform sampler2D in_texID;

varying vec2  io_texcoord;
varying vec4  io_color;
/*
void main(void) {
	gl_FragColor = io_color;
	vec2 tmpCoord = io_texcoord;
	tmpCoord = mod(tmpCoord, 1.0);
	vec4 map = texture2D(in_texID, tmpCoord);
	if (f_texcoord.x<1.0) {
		// normal font :
		gl_FragColor.a = gl_FragColor.a*map.a;
	} else if (f_texcoord.x<2.0) {
		// Italic font :
		gl_FragColor.a = gl_FragColor.a*map.r;
	} else if (f_texcoord.x<3.0) {
		// Bold font :
		gl_FragColor.a = gl_FragColor.a*map.g;
	} else {
		// bold italic font :
		gl_FragColor.a = gl_FragColor.a*map.b;
	}
}
*/

varying vec4  io_patern;

void main(void) {
	gl_FragColor = io_color;
	vec4 map = texture2D(in_texID, io_texcoord);
	float alphaCoef = dot(map, io_patern);
	gl_FragColor.a = gl_FragColor.a*alphaCoef;
}

