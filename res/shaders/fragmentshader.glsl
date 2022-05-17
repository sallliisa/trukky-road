#version 430 core

in vec2 UV;
in vec3 pos_worldspace;
in vec3 normal_cameraspace;
in vec3 eyeDirection_cameraspace;
in vec3 lightDirection_cameraspace;
in vec3 mVertex;

out vec3 color;

uniform sampler2D tex0Sampler;
uniform mat4 MV;

float getFogFactor(float d) {
	const float fogMax = 40.0;
	const float fogMin = 20.0;
	if (d >= fogMax) return 1;
	if (d <= fogMin) return 0;

	return 1 - (fogMax - d) / (fogMax - fogMin);
}

void main() {
	vec3 lightColor = vec3(1.0, 0.784, 0.184);
	float lightPower = 36.0f;

	vec3 matDiffuseColor = texture(tex0Sampler, UV).rgb;
	vec3 matAmbientColor = vec3(0.194, 0.062, 0.014) * matDiffuseColor;
	vec3 matSpecularColor = vec3(0.3, 0.3, 0.3);

	vec3 fogColor = vec3(0.973, 0.314, 0.072);

	vec3 n = normalize(normal_cameraspace);
	vec3 l = normalize(lightDirection_cameraspace);
	float cosTheta = clamp(dot(n, l), 0, 1);
	vec3 E = normalize(eyeDirection_cameraspace);
	vec3 R = reflect(-l, n);
	float cosAlpha = clamp(dot(E, R), 0, 1);
	vec3 fragColor = matAmbientColor + matDiffuseColor * lightColor * lightPower * cosTheta / 36 + matSpecularColor * lightColor * lightPower * pow(cosAlpha, 5) / 36;
	float d = distance(eyeDirection_cameraspace, mVertex);
	float alpha = getFogFactor(d);
	color = mix(fragColor, fogColor, alpha);
}