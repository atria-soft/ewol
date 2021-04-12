#ifdef GL_ES
#extension GL_OES_standard_derivatives : enable
precision mediump float;
precision mediump int;
#endif

// Input :
uniform sampler2D in_texID;
uniform float     in_SoftEdgeMin;
uniform float     in_SoftEdgeMax;
uniform int       in_SoftEdge;

varying vec2 io_texcoord;
varying vec4 io_color;


void main(void) {
	vec4 color = texture2D(in_texID, io_texcoord );
	float dist  = color.r;
	float width = fwidth(dist);
	float alpha = smoothstep(0.5-width, 0.5+width, dist);
	
	// Smooth
	gl_FragColor = vec4(io_color[0], io_color[1], io_color[2], io_color[3]*alpha);
}
