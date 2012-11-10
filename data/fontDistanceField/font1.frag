#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// Input :
uniform sampler2D EW_texID;
uniform float     EW_SoftEdgeMin;
uniform float     EW_SoftEdgeMax;
uniform int       EW_SoftEdge;

varying vec2 f_texcoord;
varying vec4 f_color;

void main(void) {
	vec4 tmpcolor = texture2D(EW_texID, f_texcoord);
	vec4 outColor = vec4(0,0,0,0);
	/*
	// compare distance with 0.5 that represent the middle ...
	if (tmpcolor[3]>0.5) {
		outColor = f_color;
		outColor[3] = 1.0;
	} else if (tmpcolor[3]>0.49) {
		// antialiasing :
		outColor = f_color;
		outColor[3] = (tmpcolor[3]-0.49)*1.0/0.02;
	}
	*/
	outColor = f_color;// * tmpcolor[3];
	if (1==EW_SoftEdge) {
		outColor[3] = smoothstep(EW_SoftEdgeMin, EW_SoftEdgeMax, tmpcolor[3]);
	} else {
		if (tmpcolor[3]>0.5) {
			outColor[3] = 1.0;
		} else {
			outColor[3] = 0.0;
		}
	}
	//outColor = vec4(0,0,0,0);
	//outColor[3] = tmpcolor[3];
	gl_FragColor = outColor;
}

