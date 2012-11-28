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
vec4  S_colorBg = vec4(0.0);
vec4  S_colorFg[3];
vec4  S_colorBorder = vec4(0.0,0.0,0.0,1.0);
float S_sizePadding = 3.0;
float S_sizeBorder = 1.0;

void main(void) {
	S_colorFg[0] = vec4(0.5,0.5,0.5,0.3);
	S_colorFg[1] = vec4(0.7,0.0,0.0,0.4);
	S_colorFg[2] = vec4(0.0,0.0,0.7,0.4);
	// prevent origin moving ... 
	vec2 position = v_position - EW_widgetProperty.origin;
	
	float specialBorder = S_sizeBorder+S_sizePadding;
	vec2  endStart = EW_widgetProperty.size - vec2(S_sizePadding) - vec2(S_sizeBorder);
	vec2  endStop  = EW_widgetProperty.size - vec2(S_sizePadding);
	if(    position.x>  S_sizePadding
	    && position.y>  S_sizePadding
	    && position.x<= endStop.x
	    && position.y<= endStop.y
	  ) {
		if(    position.x<= specialBorder
		    || position.y<= specialBorder
		    || position.x>  endStart.x
		    || position.y>  endStart.y
		  ) {
			gl_FragColor = S_colorBorder;
		} else {
			gl_FragColor =   S_colorFg[EW_status.stateOld]*(1.0-EW_status.transition)
			               + S_colorFg[EW_status.stateNew]*EW_status.transition;
		}
	} else {
		gl_FragColor = S_colorBg;
	}
}

