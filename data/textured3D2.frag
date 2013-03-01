#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// Input :
uniform sampler2D EW_texID;

varying vec2 f_texcoord;

void main(void) {
	gl_FragColor = texture2D(EW_texID, f_texcoord);
	//gl_FragColor = vec4(1.0,1.0,0.2,0.6);
}
