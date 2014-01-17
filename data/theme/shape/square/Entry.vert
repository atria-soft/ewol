#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

struct widgetStateProperty {
	int   stateOld;
	int   stateNew;
	float transition;
};

// Input :
attribute vec2 EW_coord2d;
uniform mat4 EW_MatrixTransformation;
uniform widgetStateProperty EW_status;
uniform vec4 EW_foreground;
uniform vec4 EW_foregroundSelected;
uniform vec4 EW_foregroundHover;

// output :
varying vec2  v_position;       // This will be passed into the fragment shader.
varying vec4  v_colorTansition;

void main(void) {
	
	gl_Position = EW_MatrixTransformation * vec4(EW_coord2d, 0.0, 1.0);
	// transmit position of the curent element (intermolated ...)
	v_position = EW_coord2d;
	
	
	vec4 colorOld = EW_foreground;
	if(EW_status.stateOld==1) {
		colorOld = EW_foregroundSelected;
	} else if(EW_status.stateOld==2) {
		colorOld = EW_foregroundHover;
	}
	vec4 colorNew = EW_foreground;
	if(EW_status.stateNew==1) {
		colorNew = EW_foregroundSelected;
	} else if(EW_status.stateNew==2) {
		colorNew = EW_foregroundHover;
	}
	
	// note : int() is needed for the OpenGL ES platform
	v_colorTansition =   colorOld*(1.0-EW_status.transition)
	                   + colorNew*EW_status.transition;
}
