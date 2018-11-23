#version 460 core


in vec3 position;
in vec2 texCoords;
in float alpha;
in vec3 norms;

layout (location = 10) uniform mat4 model;
layout (location = 11) uniform mat4 view;
layout (location = 12) uniform mat4 proj;

uniform vec3 cameraPos;

out Vertex {
	vec3 pos;
} OUT;


void main(){

	vec4 p = vec4(position,1);
	
	OUT.pos = position;

	//p *= model;


	gl_Position = p;// * model * view * proj;
	
	
}