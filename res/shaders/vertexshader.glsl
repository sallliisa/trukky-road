#version 430 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelspace;

out vec2 UV;
out vec3 pos_worldspace;
out vec3 normal_cameraspace;
out vec3 eyeDirection_cameraspace;
out vec3 lightDirection_cameraspace;
out vec3 mVertex;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 lightDir_worldspace;

void main(){
	gl_Position =  MVP * vec4(vertexPosition_modelspace, 1);
	pos_worldspace = (M * vec4(vertexPosition_modelspace, 1)).xyz;
	vec3 vertexPosition_cameraspace = (V * M * vec4(vertexPosition_modelspace, 1)).xyz;
	eyeDirection_cameraspace = vec3(0, 0, 0) - vertexPosition_cameraspace;
	lightDirection_cameraspace = mat3(V) * lightDir_worldspace;
	normal_cameraspace = (V * M * vec4(vertexNormal_modelspace, 0)).xyz;
	mVertex = vertexPosition_modelspace, 1;
	UV = vertexUV;
}