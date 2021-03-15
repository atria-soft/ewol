#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// Input :
uniform sampler2D EW_texID;

varying vec2  f_texcoord;
varying vec4  f_color;
/*
void main(void) {
	gl_FragColor = f_color;
	vec2 tmpCoord = f_texcoord;
	tmpCoord = mod(tmpCoord, 1.0);
	vec4 map = texture2D(EW_texID, tmpCoord);
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

varying vec4  f_patern;

void main(void) {
	gl_FragColor = f_color;
	vec4 map = texture2D(EW_texID, f_texcoord);
	float alphaCoef = dot(map, f_patern);
	gl_FragColor.a = gl_FragColor.a*alphaCoef;
}

