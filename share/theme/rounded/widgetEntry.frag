#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform float  EW_sizeBorder;
uniform float  EW_sizePadding;
uniform vec2   EW_size;
uniform vec4   EW_posText;
uniform int    EW_state;

// transmit from the vertex shader
varying vec2  v_position; // interpolated position ...

// internal static define
float S_roundedRatio = 10.0;
vec4  S_colorBg = vec4(0.0);
vec4  S_colorFg = vec4(1.0,1.0,1.0,0.8);
vec4  S_colorBorder = vec4(0.0,0.0,0.0,1.0);


void main(void) {
	// position form center : 
	vec2 ratio = EW_size / 2.0;
	vec2 positionCenter = abs(v_position-ratio);
	vec2 ratioHight = ratio - EW_sizePadding;
	vec2 ratioLow   = ratioHight - (EW_sizeBorder+S_roundedRatio);
	vec2 circleMode = smoothstep(ratioLow, ratioHight, positionCenter);
	float tmpDist = sqrt(dot(circleMode,circleMode));
	
	//float distanceInternal = (S_roundedRatio-EW_sizeBorder/2.0)/(S_roundedRatio-EW_sizeBorder);
	//float distanceExternal = (S_roundedRatio+EW_sizeBorder/2.0)/(S_roundedRatio-EW_sizeBorder);;
	if(tmpDist <= 0.7 ) {
		gl_FragColor = S_colorFg;
	} else if(tmpDist <= 0.9) {
		float tmpVal = smoothstep(0.7, 0.9, tmpDist);
		if (tmpVal<=0.5) {
			gl_FragColor = S_colorBorder*(tmpVal*2.0) + S_colorFg*(1.0-tmpVal*2.0);
		} else {
			gl_FragColor = S_colorBorder*(1.0-(tmpVal-0.5)*2.0) + S_colorBg*((tmpVal-0.5)*2.0);
		}
	} else {
		gl_FragColor = S_colorBg;
	}
}

