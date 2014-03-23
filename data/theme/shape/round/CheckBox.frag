#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

struct widgetStateProperty {
	int   activate;
	int   stateOld;
	int   stateNew;
	float transition;
};

uniform widgetStateProperty EW_status;

// transmit from the vertex shader
varying vec2  v_position; // interpolated position ...
varying vec2  v_propPos;
varying vec4  v_colorTansition;
varying vec4  v_colorBorder;
varying vec4  v_colorBackground;
varying vec4  v_colorInside;

// internal static define
float S_sizePadding  =  3.0; // must not be NULL
float S_sizeBorder   =  1.0; //==> this id for 1 px border
float S_roundedRatio = 10.0;


void main(void) {
	// position form center : 
	vec2 ratio = EW_widgetProperty.size / 2.0;
	
	// prevent origin moving ... 
	vec2 position = v_position - EW_widgetProperty.origin;
	
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
	gl_FragColor = v_colorBackground;
	// set foreground
	gl_FragColor = gl_FragColor*tmpVal + v_colorTansition*(1.0-tmpVal);
	// set border
	float tmpVal2 = abs(tmpVal-0.5)*2.0;
	gl_FragColor = gl_FragColor*tmpVal2 + v_colorBorder*(1.0-tmpVal2);
	
	// prevent origin moving ... 
	position = v_position - EW_widgetProperty.insidePos - EW_widgetProperty.insideSize*0.5;
	position = position / EW_widgetProperty.insideSize;
	
	if (sqrt(dot(position, position)) <= 1.0) {
		gl_FragColor = v_colorInside;
	}
	
}

