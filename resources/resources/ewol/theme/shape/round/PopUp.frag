#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// transmit from the vertex shader
varying vec2 v_propPos;

uniform vec4 EW_background;
uniform vec4 EW_foreground;
uniform vec4 EW_border;

void main(void) {
	// vec2 circleMode = smoothstep(ratioLow, ratio, positionCenter)*(S_roundedRatio+S_sizePadding);
	// Calculate the distance of the radius
	float tmpDist = sqrt(dot(v_propPos,v_propPos));
	// Generate the internal rampe for the the imput drawing
	float tmpVal = smoothstep(0.6, 0.7, tmpDist);
	// set Background
	gl_FragColor = mix(EW_foreground, EW_background, tmpVal);
	// Generate the internal rampe for the the imput drawing
	float tmpValBorder = 0.7 - abs(tmpDist - 0.7);
	float tmpBorder = smoothstep(0.5, 0.7, tmpValBorder);
	// set Border
	gl_FragColor = mix(gl_FragColor, EW_border, tmpBorder);
}