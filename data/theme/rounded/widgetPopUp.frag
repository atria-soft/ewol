#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

struct displayProperty {
	vec2 size;
	vec2 origin;
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
vec4  S_colorBg = vec4(0.5,0.5,0.5,0.8);
vec4  S_colorFg = vec4(1.0,1.0,1.0,0.8);
vec4  S_colorBorder = vec4(0.0,0.0,0.0,1.0);
float S_sizePadding  =  3.0; // must not be NULL
float S_sizeBorder   =  2.0; //==> this id for 1 px border
float S_roundedRatio = 10.0;


void main(void) {
	// position form center : 
	vec2 ratio = EW_widgetProperty.insideSize / 2.0;
	
	// prevent origin moving ... 
	vec2 position = v_position - EW_widgetProperty.insidePos;
	
	/* generate a central simetry
	  ____       _____
	      \     /
	       \   /
	        \ /
	         -
	*/
	vec2 positionCenter = abs(position-ratio);
	// This is a clip to remove center of the display of the widget
	vec2 ratioLow   = ratio - (S_roundedRatio+S_sizePadding);
	vec2 circleMode = smoothstep(ratioLow, ratio, positionCenter)*(S_roundedRatio+S_sizePadding);
	// Calculate the distance of the radius
	float tmpDist = float(int(sqrt(dot(circleMode,circleMode))));
	// Generate the internal rampe for the the imput drawing
	float tmpVal = smoothstep(S_roundedRatio - S_sizeBorder*1.5,
	                          S_roundedRatio + S_sizeBorder*1.5,
	                          tmpDist);
	// set Background
	gl_FragColor = S_colorBg;
	// set foreground
	gl_FragColor = gl_FragColor*tmpVal + S_colorFg*(1.0-tmpVal);
	// set border
	float tmpVal2 = abs(tmpVal-0.5)*2.0;
	gl_FragColor = gl_FragColor*tmpVal2 + S_colorBorder*(1.0-tmpVal2);
	
}

