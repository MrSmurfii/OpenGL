#version 330 core
#define MAX_POINT_LIGHTS 2

struct DirectionalLight {
	vec3 direction;
	vec3 color;
	sampler2D shadowBuffer;
	mat4 viewProjection;
};

struct PointLight {
	vec3 position;
	vec3 color;
	float radius;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	vec3 color;
	float radius;
	float angle;
};

uniform sampler2D u_MainTexture;

uniform float u_Time;
uniform vec3 u_EyePosition;
uniform DirectionalLight u_DirectionalLight;
uniform PointLight u_PointLights[MAX_POINT_LIGHTS];
uniform SpotLight u_SpotLight;

in vec3 f_World;
in vec2 f_TexCoord;
in vec3 f_Normal;
in float f_CameraDepth;

const vec3 ambientColor = vec3(1.0, 0.8, 0.2);
const float specExponent = 20.0;
const float specIntensity = 0.4;
const float shadowBias = 0.001;
const float attenuationExponent = 3.0;

const float spotLightFalloff = radians(15.f);

const float fogDistance = 60.0;
const float fogMinDistance = 20.0;
const vec3 fogColor = vec3(0.75, 0.8, 1.0);

out vec4 o_Color;

vec3 CalculateDirectionalLightColor(DirectionalLight light, vec3 albedo) {
		// calculate light (diffuse lightning)
	vec3 diffuse = albedo * light.color * max(-dot(light.direction, f_Normal), 0.0);
		/* specular highlighting	 Blinn-Phong model	*/
	vec3 worldToEye = normalize(u_EyePosition - f_World);
	vec3 halfwayVector = normalize(worldToEye - light.direction);
	float specularValue = max(dot(halfwayVector, f_Normal), 0.0);
	specularValue = pow(specularValue, specExponent) * specIntensity;
	
	vec3 specular = light.color * specularValue;
		// Shadows
	vec4 lightNDC = light.viewProjection * vec4(f_World, 1.0);

	lightNDC = lightNDC * 0.5 + 0.5;
	float lightDepth = texture(light.shadowBuffer, lightNDC.xy).x;
	float ourDepth = lightNDC.z; 
	float shadow = step(ourDepth, lightDepth + shadowBias);
	
	diffuse *= shadow;

	return diffuse + specular;
}

vec3 CalculatePointLightColor(PointLight light, vec3 albedo) {
	vec3 lightDirection = normalize(f_World - light.position);
	float attenuation = max(1.0 - length(f_World - light.position) / light.radius, 0.0);
	attenuation = pow(attenuation, attenuationExponent);

	vec3 diffuse = albedo * light.color * attenuation * max(-dot(lightDirection, f_Normal), 0.0);

	vec3 worldToEye = normalize(u_EyePosition - f_World);
	vec3 halfwayVector = normalize(worldToEye - lightDirection);
	float specularValue = max(dot(halfwayVector, f_Normal), 0.0);
	specularValue = pow(specularValue, specExponent) * specIntensity;

	vec3 specular = light.color * attenuation * specularValue;

	return diffuse + specular;
}

vec3 CalculateSpotLightColor(SpotLight light, vec3 albedo) {
	vec3 lightDirection = normalize(f_World - light.position);

	float distance = length(f_World - light.position);
	float attenuation = max(1.0 - distance / light.radius, 0.0);
	attenuation = pow(attenuation, attenuationExponent);

	vec3 diffuse = albedo * light.color * attenuation * max(-dot(lightDirection, f_Normal), 0.0);

	vec3 worldToEye = normalize(u_EyePosition - f_World);
	vec3 halfwayVector = normalize(worldToEye - lightDirection);
	float specularValue = max(dot(halfwayVector, f_Normal), 0.0);
	specularValue = pow(specularValue, specExponent) * specIntensity;
	vec3 specular = light.color * attenuation * specularValue;

	float cosAngle = dot(lightDirection, light.direction);
	float angle = acos(cosAngle);
	float cutoff = 1.0 - smoothstep(light.angle - spotLightFalloff, light.angle, angle);

	return (diffuse + specular) * cutoff;
}

void main()
{
	vec3 albedo = texture(u_MainTexture, f_TexCoord).xyz;
	
	vec3 ambient = ambientColor * albedo * 0.2;

	o_Color = vec4(0.0, 0.0, 0.0, 1.0);
	o_Color.xyz += CalculateDirectionalLightColor(u_DirectionalLight, albedo);
	for (int i = 0; i < MAX_POINT_LIGHTS; ++i) {
		o_Color.xyz += CalculatePointLightColor(u_PointLights[i], albedo);
	}
	o_Color.xyz += CalculateSpotLightColor(u_SpotLight, albedo);
	o_Color.xyz += ambient;

	float fogIntensity = clamp((f_CameraDepth - fogMinDistance) / fogDistance, 0.0, 1.0);
	fogIntensity = pow(fogIntensity, 0.8);
	o_Color.xyz = mix(o_Color.xyz, fogColor, fogIntensity);
} 