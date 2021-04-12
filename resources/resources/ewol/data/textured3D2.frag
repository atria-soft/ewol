#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif
struct DirectionalLight {
    vec3 direction;
    vec3 halfplane;
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 specularColor;
};

struct Material {
    vec4 ambientFactor;
    vec4 diffuseFactor;
    vec4 specularFactor;
    float shininess;
};

// Light
uniform DirectionalLight in_directionalLight;
// Material
uniform Material in_material;

// Input :
uniform sampler2D in_texID;

varying vec2 io_texcoord;
varying vec3 v_ecNormal;

void main(void) {
	vec4 tmpElementColor = texture2D(in_texID, io_texcoord);
	
	// Normalize v_ecNormal
	vec3 ecNormal = v_ecNormal / length(v_ecNormal);
	
	float ecNormalDotLightDirection = max(0.0, dot(ecNormal, in_directionalLight.direction));
	float ecNormalDotLightHalfplane = max(0.0, dot(ecNormal, in_directionalLight.halfplane));
	
	// Calculate ambient light
	vec4 ambientLight = in_directionalLight.ambientColor * in_material.ambientFactor;
	
	// Calculate diffuse light
	vec4 diffuseLight = ecNormalDotLightDirection * in_directionalLight.diffuseColor * in_material.diffuseFactor;
	
	// Calculate specular light
	vec4 specularLight = vec4(0.0);
	
	if (ecNormalDotLightHalfplane > 0.0) {
		specularLight = pow(ecNormalDotLightHalfplane, in_material.shininess) * in_directionalLight.specularColor * in_material.specularFactor;
		specularLight = in_directionalLight.specularColor * in_material.specularFactor;
	}
	vec4 light = ambientLight + diffuseLight + specularLight;
	gl_FragColor = tmpElementColor;// * light;
}
