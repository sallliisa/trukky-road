#version 430 core

in vec2 UV;
in vec3 pos_worldspace;
in vec3 normal_cameraspace;
in vec3 eyeDirection_cameraspace;
in vec3 lightDirection_cameraspace;

out vec3 color;

uniform sampler2D tex0Sampler;
uniform mat4 MV;
uniform vec3 lightPos_worldspace;

void main() {
	vec3 lightColor = vec3(1, 1, 1);
	float lightPower = 50.0f;

	vec3 matDiffuseColor = texture(tex0Sampler, UV).rgb;
	vec3 matAmbientColor = vec3(0.1, 0.1, 0.1) * matDiffuseColor;
	vec3 matSpecularColor = vec3(0.3, 0.3, 0.3);

	float distance = length(lightPos_worldspace - pos_worldspace);
	vec3 n = normalize(normal_cameraspace);
	vec3 l = normalize(lightDirection_cameraspace);
	float cosTheta = clamp(dot(n, l), 0, 1);
	vec3 E = normalize(eyeDirection_cameraspace);
	vec3 R = reflect(-l, n);
	float cosAlpha = clamp(dot(E, R), 0, 1);
	color = matAmbientColor + matDiffuseColor * lightColor * lightPower * cosTheta / (distance*distance) + matSpecularColor * lightColor * lightPower * pow(cosAlpha, 5) / (distance*distance);
}