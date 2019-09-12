#version 330 core

uniform sampler2D u_Sampler;
uniform sampler2D u_Sampler1;
uniform sampler2D u_LightBuffer;

uniform float u_Time;
uniform vec3 u_LightDirection;
uniform vec3 u_EyePosition;
uniform mat4 u_LightViewProjection;

in vec3 f_World;
in vec2 f_TexCoord;
in vec3 f_Normal;

const vec3 diffuseColor = vec3(0.8, 0.8, 0.9);
const vec3 ambientColor = vec3(1.0, 0.8, 0.2);
const float specExponent = 20.0;
const float specIntensity = 0.4;
const float shadowBias = 0.0001;

out vec4 o_Color;
void main()
{
    vec2 TexCoord = f_TexCoord;

    vec4 Texture1 = texture(u_Sampler, TexCoord);
    vec4 Texture2 = texture(u_Sampler1, TexCoord);

	// calculate light (diffuse lightning)
	float diffuse = max(-dot(u_LightDirection, f_Normal), 0.0);

	// specular highlighting
	// Blinn-Phong model
	vec3 worldToEye = normalize(u_EyePosition - f_World);
	vec3 halfwayVector = normalize(worldToEye - u_LightDirection);
	float specular = max(dot(halfwayVector, f_Normal), 0.0);
	specular = pow(specular, specExponent) * specIntensity;

	// reflect = vector - 2(normal*(normal(dot)vector))

	// ambient light
	float ambient = 0.2;

	// Shadows
	vec4 lightNDC = u_LightViewProjection * vec4(f_World, 1.0);

	lightNDC = lightNDC * 0.5 + 0.5;
	float lightDepth = texture(u_LightBuffer, lightNDC.xy).x;
	float ourDepth = lightNDC.z; 
	float shadow = step(ourDepth, lightDepth + shadowBias);
	diffuse *= shadow;


    o_Color = mix(Texture1, Texture2, sin(u_Time) * 0.5 + 0.5 );
	o_Color.xyz *= (diffuseColor * diffuse) + (ambientColor * ambient);
	o_Color.xyz += vec3(specular);
} 