#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

struct displayProperty {
	vec2 size;
	vec2 insidePos;
	vec2 insideSize;
};

struct widgetStateProperty {
	int   stateOld;
	int   stateNew;
	float transition;
};

uniform displayProperty     EW_widgetProperty;

uniform widgetStateProperty EW_status;

// transmit from the vertex shader
varying vec2  v_position; // interpolated position ...

// internal static define
vec4  S_colorBg = vec4(0.0);
vec4  S_colorFg = vec4(0.5,0.5,0.5,0.3);
vec4  S_colorBorder = vec4(0.0,0.0,0.0,1.0);
float S_sizePadding  =  3.0; // must not be NULL
float S_sizeBorder   =  1.0; //==> this id for 1 px border
float S_roundedRatio = 10.0;

void main(void) {
	// position form center : 
	vec2 ratio = EW_widgetProperty.size / 2.0;
	vec2 positionCenter = abs(v_position-ratio);
	vec2 ratioHight = ratio - S_sizePadding;
	vec2 ratioLow   = ratioHight - (S_sizeBorder+S_roundedRatio);
	vec2 circleMode = smoothstep(ratioLow, ratioHight, positionCenter);
	float tmpDist = sqrt(dot(circleMode,circleMode));
	
	//float distanceInternal = (S_roundedRatio-S_sizeBorder/2.0)/(S_roundedRatio-S_sizeBorder);
	//float distanceExternal = (S_roundedRatio+S_sizeBorder/2.0)/(S_roundedRatio-S_sizeBorder);;
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

