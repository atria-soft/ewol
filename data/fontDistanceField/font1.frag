#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// Input :
uniform sampler2D EW_texID;
uniform float     EW_SoftEdgeMin;
uniform float     EW_SoftEdgeMax;
uniform int       EW_SoftEdge;

varying vec2 f_texcoord;
varying vec4 f_color;

const float glyph_center   = 0.50;
const float outline_center = 0.55;
const float glow_center    = 1.25;

vec3 glyph_color    = vec3(1.0,1.0,1.0);
vec3 outline_color  = vec3(0.0,0.0,0.0);
vec3 glow_color     = vec3(0.0,1.0,0.0);

void main(void) {
	vec4 color = texture2D(EW_texID, f_texcoord );
	float dist  = color.r;
	float width = fwidth(dist);
	float alpha = smoothstep(glyph_center-width, glyph_center+width, dist);
	
	// Smooth
	
	gl_FragColor = vec4(f_color[0], f_color[1], f_color[2], f_color[3]*alpha);
	
	// Outline
	/*
	float mu = smoothstep(outline_center-width, outline_center+width, dist);
	vec3 rgb = mix(outline_color, glyph_color, mu);
	gl_FragColor = vec4(rgb, max(alpha,mu));
	*/
	// Glow
	/*
	vec3 rgb = mix(glow_color, glyph_color, alpha);
	float mu = smoothstep(glyph_center, glow_center, sqrt(dist));
	gl_FragColor = vec4(rgb, max(alpha,mu));
	*/
	
	// Glow + outline
	/*
	vec3 rgb = mix(glow_color, glyph_color, alpha);
	float mu = smoothstep(glyph_center, glow_center, sqrt(dist));
	color = vec4(rgb, max(alpha,mu));
	float beta = smoothstep(outline_center-width, outline_center+width, dist);
	rgb = mix(outline_color, color.rgb, beta);
	gl_FragColor = vec4(rgb, max(color.a,beta));
	*/
}
