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

uniform displayProperty EW_widgetProperty;

uniform vec4 EW_background;
uniform vec4 EW_foreground;
uniform vec4 EW_border;

// transmit from the vertex shader
varying vec2  v_position; // interpolated position ...

// internal static define
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
			gl_FragColor = EW_border;
		} else {
			// note : int() is needed for the OpenGL ES platform
			gl_FragColor = EW_foreground;
		}
	} else {
		gl_FragColor = EW_background;
	}
}

