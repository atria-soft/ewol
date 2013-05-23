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


uniform displayProperty     EW_widgetProperty;


// transmit from the vertex shader
varying vec2  v_position; // interpolated position ...
varying vec4  v_colorTansition;

// internal static define
vec4  S_colorBg = vec4(0.0);
vec4  S_colorBorder = vec4(0.0,0.0,0.0,1.0);
float S_sizePadding = 1.0;
float S_sizeBorder = 3.0;

void main(void) {
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
			// note : int() is needed for the OpenGL ES platform
			gl_FragColor = v_colorTansition;
		}
	} else {
		gl_FragColor = S_colorBg;
	}
}

